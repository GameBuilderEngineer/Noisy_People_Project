//===================================================================================================================================
//【3DSound.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/29
// [更新日]2019/10/29
//===================================================================================================================================
#include "3DSound.h"
#include "Sound.h"
#include "ImguiManager.h"
#include "AbstractScene.h"
#include "Player.h"


//仮
const D3DXVECTOR3 tunnalPos = D3DXVECTOR3(0, 0, 0);

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
S3DManager::S3DManager()
{
#if(XADUIO2_STATE)
	//ミッドポイントボイスの作成(P1)
	if (FAILED(SoundInterface::GetXAudio2Interface()->CreateSubmixVoice(
		&MidpointVoice[playerNS::PLAYER1],									//サブミックスボイス
		ENDPOINT_INPUT_CHANNEL,												//チャンネル数(入力)
		ENDPOINT_SAMPLE_RATE,												//サンプリングレート(入力)
		XAUDIO2_VOICE_USEFILTER,												//フィルター機能
		0,																	//プロセスステージ
		&SendList,															//送信リスト(S3D)
		NULL)))																//エフェクトチェーン
	{
		return;
	}
	//ミッドポイントボイスの作成(P2)
	if (FAILED(SoundInterface::GetXAudio2Interface()->CreateSubmixVoice(
		&MidpointVoice[playerNS::PLAYER2],									//サブミックスボイス
		ENDPOINT_INPUT_CHANNEL,												//チャンネル数(入力)
		ENDPOINT_SAMPLE_RATE,												//サンプリングレート(入力)
		XAUDIO2_VOICE_USEFILTER,												//フィルター機能
		0,																	//プロセスステージ
		&SendList,															//送信リスト(S3D)
		NULL)))																//エフェクトチェーン
	{
		return;
	}

	//ミッドポイントボイスへの送信リストの作成
	SendDescriptor[playerNS::PLAYER1] = { XAUDIO2_SEND_USEFILTER,MidpointVoice[playerNS::PLAYER1] };
	MidSendList[playerNS::PLAYER1] = { 1,&SendDescriptor[playerNS::PLAYER1] };
	SendDescriptor[playerNS::PLAYER2] = { XAUDIO2_SEND_USEFILTER,MidpointVoice[playerNS::PLAYER2] };
	MidSendList[playerNS::PLAYER2] = { 1,&SendDescriptor[playerNS::PLAYER2] };

	//ミッドポイントボイスの初期化
	float p1Matrix[] = { 1.0f,1.0f,0.0f,0.0f };
	float p2Matrix[] = { 0.0f,0.0f,1.0f,1.0f };
	MidpointVoice[playerNS::PLAYER1]->SetOutputMatrix(EndpointVoice, ENDPOINT_INPUT_CHANNEL, ENDPOINT_INPUT_CHANNEL, p1Matrix);
	MidpointVoice[playerNS::PLAYER2]->SetOutputMatrix(EndpointVoice, ENDPOINT_INPUT_CHANNEL, ENDPOINT_INPUT_CHANNEL, p2Matrix);

	this->loadBuffer();
#endif
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
S3DManager::~S3DManager()
{
#if(XADUIO2_STATE)
	//ミッドポイントボイス
	uninitSoundStop();
	SAFE_DESTROY_VOICE(MidpointVoice[playerNS::PLAYER1])
	SAFE_DESTROY_VOICE(MidpointVoice[playerNS::PLAYER2])
#endif
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
#ifdef _DEBUG
void S3DManager::outputGUI(void)
{
#if(XADUIO2_STATE)
	if (!ImGui::CollapsingHeader("S3DInformation"))
	{
		//使用中のバッファ数
		ImGui::Text("Number of buffers:%d", bufferMax);

		//使用中のボイス数
		ImGui::Text("Number of voice:%d", soundParametersList->nodeNum - 1);

		for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
		{
			//サウンドのパラメーター
			SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

			//バッファ
			LIST_BUFFER *tmpBuffer = GetBuffer(
				tmpSoundParameters->playParameters.endpointVoiceId,
				tmpSoundParameters->playParameters.soundId,
				tmpSoundParameters->playParameters.loop);

			//再生位置&情報取得
			XAUDIO2_VOICE_STATE state = { 0 };
			tmpSoundParameters->SourceVoice->GetState(&state);
			long playPoint = (long)state.SamplesPlayed;
			while (playPoint >= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel))
			{
				playPoint -= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel);
			}
			
			float volume = 0.0f;
			tmpSoundParameters->SourceVoice->GetVolume(&volume);
			if ((tmpSoundParameters->isPlaying) &&		//再生している
				(volume > 0))							//聞こえる
			{
				//ボイスID
				ImGui::Text("Voice ID:%d", tmpSoundParameters->playParameters.voiceID);

				//名
				ImGui::Text("%s", S3D_PATH_LIST_TAIL(tmpSoundParameters->playParameters.soundId));

				//音量
				ImGui::Text("volume:%f", volume);

				//波形の描画
				int saveDataMax = 5512;	//取得するデータ数
				int dataMax = (saveDataMax / tmpBuffer->wavFile.fmt.fmtChannel) + 2;	 //セーブしたいデータの数/チャンネル数 + 2
				float *fData = new (float[dataMax]);
				memset(fData, 0, sizeof(float)*dataMax);
				fData[0] = (float)SHRT_MIN;
				fData[dataMax - 1] = (float)SHRT_MAX;
				int wtPos = dataMax - 2;
				for (int j = saveDataMax - 1; j > 0; j -= tmpBuffer->wavFile.fmt.fmtChannel)
				{
					for (int k = 0; k < tmpBuffer->wavFile.fmt.fmtChannel; k++)
					{
						if (playPoint - j < 0)
						{
							fData[wtPos] = 0.0f;
							continue;
						}

						fData[wtPos] += ((float)tmpBuffer->wavFile.data.waveData[((playPoint - j) * tmpBuffer->wavFile.fmt.fmtChannel) - k] / (float)tmpBuffer->wavFile.fmt.fmtChannel);
					}
					wtPos--;
				}
				ImVec2 plotextent(ImGui::GetContentRegionAvailWidth(), 100);
				ImGui::PlotLines("", fData, dataMax, 0, "Sound wave", FLT_MAX, FLT_MAX, plotextent);
				SAFE_DELETE_ARRAY(fData);

				//再生位置
				ImGui::ProgressBar(playPoint / (float)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel));

			}
		}
	}
#endif
}
#endif

#if(XADUIO2_STATE)
//===================================================================================================================================
//【ソースボイスの作成】
//===================================================================================================================================
void S3DManager::MakeSourceVoice(PLAY_PARAMETERS *playParameters, LIST_BUFFER *listBuffer)
{
		//波形のフォーマット
		WAVEFORMATEX pcm;
		memset(&pcm, 0, sizeof(pcm));

		//再生のパラメータ
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(soundParametersList->nodeNum - 1);
		playParameters->voiceID = voiceIDCnt;
		tmpSoundParameters->playParameters = *playParameters;
		tmpSoundParameters->stopPoint = 0;
		tmpSoundParameters->isPlaying = false;
		tmpSoundParameters->isSpeed = false;

		//再生速度の変更
		if (tmpSoundParameters->playParameters.speed != NULL)
		{
			//後でtrueにする
			tmpSoundParameters->isSpeed = true;
		}

		//波形のフォーマット
		pcm.cbSize = 0;
		pcm.nChannels = listBuffer->wavFile.fmt.fmtChannel;
		pcm.wBitsPerSample = listBuffer->wavFile.fmt.fmtBitPerSample;
		pcm.nSamplesPerSec = listBuffer->wavFile.fmt.fmtSampleRate;
		pcm.wFormatTag = WAVE_FORMAT_PCM;
		pcm.nBlockAlign = (listBuffer->wavFile.fmt.fmtChannel*listBuffer->wavFile.fmt.fmtBitPerSample) / 8;
		pcm.nAvgBytesPerSec = listBuffer->wavFile.fmt.fmtSampleRate*pcm.nBlockAlign;

		//ソースボイスの作成
		SoundInterface::GetXAudio2Interface()->CreateSourceVoice(
			&tmpSoundParameters->SourceVoice,
			&pcm,
			XAUDIO2_VOICE_USEFILTER,
			XAUDIO2_MAX_FREQ_RATIO,
			NULL,
			&MidSendList[tmpSoundParameters->playParameters.playerID],
			NULL);

		////デフォールトボリューム
		//tmpSoundParameters->SourceVoice->SetVolume(0.0f);

		//バッファの提出
		tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&listBuffer->buffer);

		//ノードの更新処理
		soundParametersList->insertAfter(soundParametersList->getNode(soundParametersList->nodeNum - 1));
		soundParametersList->listUpdate();

		//ボイスID
		voiceIDCnt++;
		if (voiceIDCnt > LONG_MAX)
		{
			voiceIDCnt -= LONG_MAX;
		}

		//再生
		playSound(playParameters);
}
#endif

//===================================================================================================================================
//【ソースボイスの作成】
//===================================================================================================================================
void S3DManager::SetVolume(const PLAY_PARAMETERS playParameters, float volume)
{
#if(XADUIO2_STATE)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		if (tmpSoundParameters->playParameters.voiceID == playParameters.voiceID)
		{
			//ボリューム
			tmpSoundParameters->SourceVoice->SetVolume(volume);
		}
	}
#endif
}
//===================================================================================================================================
//【必要なサウンドバッファを用意する】
//===================================================================================================================================
void S3DManager::loadBuffer(void)
{
#if(XADUIO2_STATE)
	//サウンドディレクトリに設定する
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_S3D);

	bufferMax = S3D_LIST::S3D_MAX;
	S3DManager::bufferList = new LIST_BUFFER[bufferMax];
	for (int i = 0; i < bufferMax; i++)
	{
		S3DManager::bufferList[i].buffer = { 0 };

		FILE *fp = nullptr;
		fp = fopen(S3D_PATH_LIST_TAIL(i), "rb");
		S3DManager::bufferList[i].wavFile = LoadWavChunk(fp);
		S3DManager::bufferList[i].soundId = i;
		S3DManager::bufferList[i].buffer.pAudioData = (BYTE*)S3DManager::bufferList[i].wavFile.data.waveData;
		S3DManager::bufferList[i].buffer.AudioBytes = S3DManager::bufferList[i].wavFile.data.waveSize;
		S3DManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

		fclose(fp);
	}
#endif
}
