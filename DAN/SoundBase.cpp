//===================================================================================================================================
//【SoundBase.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/04
// [更新日]2019/10/04
//===================================================================================================================================
#include "SoundBase.h"
#include "Sound.h"
#include "AbstractScene.h"

//===================================================================================================================================
//【コンストラクタ】
//・ノードの準備
//===================================================================================================================================
SoundBase::SoundBase()
{
#if(XADUIO2_STATE)
	//リストの作成
	soundParametersList = new LinkedList <SOUND_PARAMETERS>;
	soundParametersList->insertFront();
	soundParametersList->listUpdate();
	voiceIDCnt = 0;

	//エンドポイントボイスの作成
	if (FAILED(SoundInterface::GetXAudio2Interface()->CreateSubmixVoice(
		&EndpointVoice,										//サブミックスボイス
		ENDPOINT_INPUT_CHANNEL,								//チャンネル数(入力)
		ENDPOINT_SAMPLE_RATE,								//サンプリングレート(入力)
		XAUDIO2_VOICE_USEFILTER,								//フィルター機能
		1,													//プロセスステージ
		NULL)))												//送信リスト(NULL:Mastering Voiceへの単一の出力となる)
	{														//エフェクトチェーン
		return;
	}

	//エンドポイントボイスへの送信リストの作成
	SendDescriptor = { XAUDIO2_SEND_USEFILTER,EndpointVoice };
	SendList = { 1,&SendDescriptor };

#if _DEBUG
	//シーンの初期化
	scene = SceneList::SPLASH;
#endif
#endif
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
SoundBase::~SoundBase()
{
#if(XADUIO2_STATE)
	uninitSoundStop();
	soundParametersList->terminate();
	SAFE_DELETE(soundParametersList);

	SAFE_DESTROY_VOICE(EndpointVoice)
#endif
}
//===================================================================================================================================
//【停止(全部のサウンド)】
//===================================================================================================================================
void SoundBase::uninitSoundStop(void)
{
#if(XADUIO2_STATE)
	int backup = soundParametersList->nodeNum - 1;
	for (int i = 0; i < backup; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
		
		if (tmpSoundParameters->isPlaying)		//再生している
		{
			tmpSoundParameters->SourceVoice->Stop();
			tmpSoundParameters->isPlaying = false;
			SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice);
			soundParametersList->remove(soundParametersList->getNode(i));
		}
	}
	soundParametersList->listUpdate();
#endif
}

//===================================================================================================================================
//【再生】
//===================================================================================================================================
void SoundBase::playSound(PLAY_PARAMETERS *playParameters)
{
#if(XADUIO2_STATE)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		//if ((!tmpSoundParameters->isPlaying) &&											//再生していない
		//	(tmpSoundParameters->playParameters.soundId == playParameters->soundId) &&	//IDが一致する
		//	(tmpSoundParameters->playParameters.loop == playParameters->loop)&&			//ループ情報も一致する
		//	(tmpSoundParameters->playParameters.playerID == playParameters->playerID))	//プレイヤーID
		if (tmpSoundParameters->playParameters.voiceID == playParameters->voiceID)
		{
			tmpSoundParameters->SourceVoice->Start();
			if (tmpSoundParameters->playParameters.filterFlag)
			{
				tmpSoundParameters->SourceVoice->SetFilterParameters(&tmpSoundParameters->playParameters.filterParameters.Parameters);
			}
			tmpSoundParameters->isPlaying = true;
			return;
		}
	}

	//リスト中に存在しない、再生していないボイスがない時は新しく追加する
	MakeSourceVoice(playParameters,
		GetBuffer(playParameters->endpointVoiceId, playParameters->soundId, playParameters->loop));
#endif
}

//===================================================================================================================================
//【停止】
//===================================================================================================================================
void SoundBase::stopSound(const PLAY_PARAMETERS playParameters)
{
#if(XADUIO2_STATE)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		//if ((tmpSoundParameters->isPlaying) &&											//再生していない
		//	(tmpSoundParameters->playParameters.soundId == playParameters.soundId) &&	//ID
		//	(tmpSoundParameters->playParameters.loop == playParameters.loop)&&			//ループ情報
		//	(tmpSoundParameters->playParameters.playerID == playParameters.playerID))	//プレイヤーID
		if (tmpSoundParameters->playParameters.voiceID == playParameters.voiceID)
		{			
			tmpSoundParameters->SourceVoice->Stop();
			SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice);
			soundParametersList->remove(soundParametersList->getNode(i));
			soundParametersList->listUpdate();
			return;
		}
	}
#endif
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void SoundBase::updateSound(void)
{
#if(XADUIO2_STATE)
	//再生状態をチェック
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		if ((tmpSoundParameters != NULL) && (!tmpSoundParameters->playParameters.loop))
		{
			XAUDIO2_VOICE_STATE voiceState;
			tmpSoundParameters->SourceVoice->GetState(&voiceState);

			//再生し終わった
			if (voiceState.BuffersQueued == 0)
			{
				SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice);
				soundParametersList->remove(soundParametersList->getNode(i));
				soundParametersList->listUpdate();
			}
		}
	}
#endif
}

#if(XADUIO2_STATE)
//===================================================================================================================================
//【サウンドバッファを取得】
//===================================================================================================================================
LIST_BUFFER *SoundBase::GetBuffer(int endpointVoiceId, int soundId, bool loop)
{
	if (loop)
	{
		bufferList[soundId].buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	return &bufferList[soundId];
}

//===================================================================================================================================
//【ソースボイスの作成】
//===================================================================================================================================
void SoundBase::MakeSourceVoice(PLAY_PARAMETERS *playParameters, LIST_BUFFER *listBuffer)
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
		tmpSoundParameters->isSpeed = false;
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
		&SendList,
		NULL);

	//バッファの提出
	tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&listBuffer->buffer);

	//ノードの更新処理
	soundParametersList->insertAfter(soundParametersList->getNode(soundParametersList->nodeNum - 1));
	soundParametersList->listUpdate();

	//voiceID
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
//【WAVファイルの読み込み処理】
//===================================================================================================================================
WAV_FILE SoundBase::LoadWavChunk(FILE *fp)
{
	WAV_FILE tmpWavFile;	// WAVファイル構造体
	memset(&tmpWavFile, 0, sizeof(tmpWavFile));

	// riffの読み込み
	fread(&tmpWavFile.riff, sizeof(RIFF_CHUNK), 1, fp);

	int	chunkFlag = NULL;	// チャンクを探すループのフラグ

	// fmt_とdataの読み込み
	while (chunkFlag != FLAG_CHUNK_END)
	{
		char	 chunk[CHUNK_ID_SIZE];
		long	 size = 0;

		// チャンクとサイズの読み込み
		fread(&chunk, sizeof(chunk), 1, fp);
		fread(&size, sizeof(size), 1, fp);

		// fmt_チャンク
		if (memcmp(chunk, CHUNK_FMT, CHUNK_ID_SIZE) == 0)
		{
			// チャンクとサイズを設定
			memcpy(tmpWavFile.fmt.fmtChunk, chunk, CHUNK_ID_SIZE);
			tmpWavFile.fmt.fmtSize = size;

			// フォーマットIDから読み込み
			fread(&tmpWavFile.fmt.fmtFormatTag,
				sizeof(FMT_CHUNK) - (sizeof(tmpWavFile.fmt.fmtChunk) + sizeof(tmpWavFile.fmt.fmtSize)), 1, fp);

			// フラグ処理
			chunkFlag += FLAG_CHUNK_FMT;

			continue;
		}
		
		// dataチャンク
		if (memcmp(chunk, CHUNK_DATA, CHUNK_ID_SIZE) == 0)
		{
			// チャンクとサイズを設定
			memcpy(tmpWavFile.data.dataChunk, CHUNK_DATA, CHUNK_ID_SIZE);
			tmpWavFile.data.waveSize = size;

			// データの読み込み
			tmpWavFile.data.waveData = new (short[tmpWavFile.data.waveSize / sizeof(short)]);
			fread(tmpWavFile.data.waveData, tmpWavFile.data.waveSize, 1, fp);

			// フラグ処理
			chunkFlag += FLAG_CHUNK_DATA;

			continue;
		}

		// 他のチャンク
		else
		{
			// いらない分をスキップ!
			fseek(fp, size, SEEK_CUR);
		}
	}

	return tmpWavFile;
}
