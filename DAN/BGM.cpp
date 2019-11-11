//===================================================================================================================================
//【BGM.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/14
// [更新日]2019/10/14
//===================================================================================================================================
#include "BGM.h"
#include "Sound.h"
#include "ImguiManager.h"
#include "AbstractScene.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
BGMManager::BGMManager()
{
#if(XADUIO2_STATE)
#endif
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
BGMManager::~BGMManager()
{
#if(XADUIO2_STATE)
#endif
}

//===================================================================================================================================
//【再生速度の設定】
//===================================================================================================================================
void BGMManager::SetSpeed(void)
{
#if(XADUIO2_STATE)

	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
		if(tmpSoundParameters->isSpeed)
		{
			//バッファ
			LIST_BUFFER *tmpBuffer = GetBuffer(tmpSoundParameters->playParameters.endpointVoiceId, tmpSoundParameters->playParameters.soundId, tmpSoundParameters->playParameters.loop);

			//停止処理
			tmpSoundParameters->SourceVoice->Stop(XAUDIO2_PLAY_TAILS);
			XAUDIO2_VOICE_STATE state = { 0 };
			tmpSoundParameters->SourceVoice->Discontinuity();
			tmpSoundParameters->SourceVoice->FlushSourceBuffers();
			tmpSoundParameters->SourceVoice->GetState(&state);

			//キューに何でもないければ
			if (state.BuffersQueued == 0)
			{
				//再生速度
				tmpSoundParameters->SourceVoice->SetSourceSampleRate(
					(UINT32)(tmpBuffer->wavFile.fmt.fmtSampleRate * tmpSoundParameters->playParameters.speed));

				//停止位置
				tmpSoundParameters->stopPoint = (long)state.SamplesPlayed;
				while (tmpSoundParameters->stopPoint >= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel))
				{
					tmpSoundParameters->stopPoint -= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel);
				}

				//再生位置
				tmpBuffer->buffer.PlayBegin = tmpSoundParameters->stopPoint;

				//バッファの提出
				tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&tmpBuffer->buffer);

				//再生処理
				tmpSoundParameters->SourceVoice->Start();

				//再生速度
				tmpSoundParameters->isSpeed = false;
			}
		}
	}
#endif
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
#ifdef _DEBUG
void BGMManager::outputGUI(void)
{
#if(XADUIO2_STATE)
	if (!ImGui::CollapsingHeader("BGMInformation"))
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

			//再生している
			if (tmpSoundParameters->isPlaying)
			{
				//ボイスID
				ImGui::Text("Voice ID:%d", tmpSoundParameters->playParameters.voiceID);

				//速度のバックアップ
				float backUpSpeed = tmpSoundParameters->playParameters.speed;

				//サウンド名
				switch (scene)
				{
				case SceneList::SPLASH:
					break;
				case SceneList::TITLE:
					//サウンド名
					ImGui::Text("%s", BGM_PATH_LIST_TAIL(title, [tmpSoundParameters->playParameters.soundId]));
					break;
				case SceneList::TUTORIAL:
					//サウンド名
					ImGui::Text("%s", BGM_PATH_LIST_TAIL(tutorial, [tmpSoundParameters->playParameters.soundId]));
					break;
				case SceneList::CREDIT:
					//サウンド名
					ImGui::Text("%s", BGM_PATH_LIST_TAIL(credit, [tmpSoundParameters->playParameters.soundId]));
					break;
				case SceneList::GAME:
					//サウンド名
					ImGui::Text("%s", BGM_PATH_LIST_TAIL(game, [tmpSoundParameters->playParameters.soundId]));
					break;
				case SceneList::RESULT:
					break;
				case SceneList::NONE_SCENE:
					break;
				default:
					break;
				}

				//波形の描画
				int saveDataMax = 2756;	//取得するデータ数
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

				//速度調整
				ImGui::Text("%d", ((int)state.SamplesPlayed / 44100));
				ImGui::SliderFloat("Speed", &tmpSoundParameters->playParameters.speed, 0.1f, 3.0f);

				if (backUpSpeed != tmpSoundParameters->playParameters.speed)
				{
					tmpSoundParameters->isSpeed = true;
					BGMManager::SetSpeed();
				}
			}
		}
	}
#endif
}
#endif

//===================================================================================================================================
//【ステージ遷移に合わせて必要なサウンドバッファを用意する】
//===================================================================================================================================
void BGMManager::SwitchAudioBuffer(int scene)
{
#if(XADUIO2_STATE)
	//サウンドディレクトリに設定する
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_BGM);

#if _DEBUG
	//シーンの更新
	BGMManager::scene = scene;
#endif

	//解放
	for (int i = 0; i < BGMManager::bufferMax; i++)
	{
		SAFE_DELETE_ARRAY(BGMManager::bufferList[i].wavFile.data.waveData);
	}
	SAFE_DELETE_ARRAY(BGMManager::bufferList);

	switch (scene)
	{
	case SceneList::SPLASH:
		bufferMax = SPLASH_BGM_LIST::SPLASH_BGM_MAX;
		break;
	case SceneList::TITLE:
		BGMManager::bufferList = new LIST_BUFFER[TITLE_BGM_LIST::TITLE_BGM_MAX];
		for (int i = 0; i < TITLE_BGM_LIST::TITLE_BGM_MAX; i++)
		{
			BGMManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(BGM_PATH_LIST_TAIL(title,[i]), "rb");
			BGMManager::bufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::bufferList[i].soundId = i;
			BGMManager::bufferList[i].buffer.pAudioData = (BYTE*)BGMManager::bufferList[i].wavFile.data.waveData;
			BGMManager::bufferList[i].buffer.AudioBytes = BGMManager::bufferList[i].wavFile.data.waveSize;
			BGMManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = TITLE_BGM_LIST::TITLE_BGM_MAX;
		break;
	case SceneList::TUTORIAL:
		BGMManager::bufferList = new LIST_BUFFER[TUTORIAL_BGM_LIST::TUTORIAL_BGM_MAX];
		for (int i = 0; i < TUTORIAL_BGM_LIST::TUTORIAL_BGM_MAX; i++)
		{
			BGMManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(BGM_PATH_LIST_TAIL(tutorial, [i]), "rb");
			BGMManager::bufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::bufferList[i].soundId = i;
			BGMManager::bufferList[i].buffer.pAudioData = (BYTE*)BGMManager::bufferList[i].wavFile.data.waveData;
			BGMManager::bufferList[i].buffer.AudioBytes = BGMManager::bufferList[i].wavFile.data.waveSize;
			BGMManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = TUTORIAL_BGM_LIST::TUTORIAL_BGM_MAX;
		break;
	case SceneList::CREDIT:
		BGMManager::bufferList = new LIST_BUFFER[CREDIT_BGM_LIST::CREDIT_BGM_MAX];
		for (int i = 0; i < CREDIT_BGM_LIST::CREDIT_BGM_MAX; i++)
		{
			BGMManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(BGM_PATH_LIST_TAIL(credit, [i]), "rb");
			BGMManager::bufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::bufferList[i].soundId = i;
			BGMManager::bufferList[i].buffer.pAudioData = (BYTE*)BGMManager::bufferList[i].wavFile.data.waveData;
			BGMManager::bufferList[i].buffer.AudioBytes = BGMManager::bufferList[i].wavFile.data.waveSize;
			BGMManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = CREDIT_BGM_LIST::CREDIT_BGM_MAX;
		break;
	case SceneList::GAME:
		BGMManager::bufferList = new LIST_BUFFER[GAME_BGM_LIST::GAME_BGM_MAX];
		for (int i = 0; i < GAME_BGM_LIST::GAME_BGM_MAX; i++)
		{
			BGMManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(BGM_PATH_LIST_TAIL(game, [i]), "rb");
			BGMManager::bufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::bufferList[i].soundId = i;
			BGMManager::bufferList[i].buffer.pAudioData = (BYTE*)BGMManager::bufferList[i].wavFile.data.waveData;
			BGMManager::bufferList[i].buffer.AudioBytes = BGMManager::bufferList[i].wavFile.data.waveSize;
			BGMManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = GAME_BGM_LIST::GAME_BGM_MAX;
		break;
	case SceneList::RESULT:
		BGMManager::bufferList = new LIST_BUFFER[RESULT_BGM_LIST::RESULT_BGM_MAX];
		for (int i = 0; i < RESULT_BGM_LIST::RESULT_BGM_MAX; i++)
		{
			BGMManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(BGM_PATH_LIST_TAIL(result, [i]), "rb");
			BGMManager::bufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::bufferList[i].soundId = i;
			BGMManager::bufferList[i].buffer.pAudioData = (BYTE*)BGMManager::bufferList[i].wavFile.data.waveData;
			BGMManager::bufferList[i].buffer.AudioBytes = BGMManager::bufferList[i].wavFile.data.waveSize;
			BGMManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = RESULT_BGM_LIST::RESULT_BGM_MAX;
		break;
	case SceneList::NONE_SCENE:
		bufferMax = 0;
		break;
	case SceneList::CREATE:
		bufferMax = CREATE_BGM_LIST::CREATE_BGM_MAX;
		break;
	default:
		break;
	}
#endif
}
