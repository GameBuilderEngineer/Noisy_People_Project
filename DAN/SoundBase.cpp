//===================================================================================================================================
//【SoundBase.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/04
// [更新日]2019/10/04
//===================================================================================================================================
#include "SoundBase.h"
#include "Sound.h"

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
#if(_MSC_VER >= GAME_MSC_VER)
LIST_BUFFER *SoundBase::SEBufferList = nullptr;
LIST_BUFFER *SoundBase::BGMBufferList = nullptr;
int	SoundBase::SEBufferMax = 0;
int	SoundBase::BGMBufferMax = 0;
#endif
//===================================================================================================================================
//【コンストラクタ】
//・ノードの準備
//===================================================================================================================================
SoundBase::SoundBase()
{
#if(_MSC_VER >= GAME_MSC_VER)
	//リストの作成
	soundParametersList = new LinkedList <SOUND_PARAMETERS>;
	soundParametersList->insertFront();
	soundParametersList->listUpdate();
#endif
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
SoundBase::~SoundBase()
{
#if(_MSC_VER >= GAME_MSC_VER)
	uninitSoundStop();
	soundParametersList->terminate();
	SAFE_DELETE(soundParametersList);
#endif
}
//===================================================================================================================================
//【停止(全部のサウンド)】
//===================================================================================================================================
void SoundBase::uninitSoundStop(void)
{
#if(_MSC_VER >= GAME_MSC_VER)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
		
		if (tmpSoundParameters->isPlaying)		//再生している
		{
			tmpSoundParameters->SourceVoice->Stop();
			tmpSoundParameters->isPlaying = false;
			SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice);
		}
	}
#endif
}

//===================================================================================================================================
//【再生】
//===================================================================================================================================
void SoundBase::playSound(const PLAY_PARAMETERS playParameters)
{
#if(_MSC_VER >= GAME_MSC_VER)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		if ((!tmpSoundParameters->isPlaying) &&				//再生していない
			(tmpSoundParameters->playParameters.soundId == playParameters.soundId) &&		//IDが一致する
			(tmpSoundParameters->playParameters.loop == playParameters.loop))				//ループ情報も一致する
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
		GetBuffer(playParameters.endpointVoiceId, playParameters.soundId, playParameters.loop));
#endif
}

//===================================================================================================================================
//【停止】
//===================================================================================================================================
void SoundBase::stopSound(const PLAY_PARAMETERS playParameters)
{
#if(_MSC_VER >= GAME_MSC_VER)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		if ((tmpSoundParameters->isPlaying) &&							//再生していない
			(tmpSoundParameters->playParameters.soundId == playParameters.soundId) &&					//ID
			(tmpSoundParameters->playParameters.loop == playParameters.loop)&&							//ループ情報
			(tmpSoundParameters->playParameters.endpointVoiceId == playParameters.endpointVoiceId))	//エンドポイントボイスID
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
#if(_MSC_VER >= GAME_MSC_VER)
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

#if(_MSC_VER >= GAME_MSC_VER)
//===================================================================================================================================
//【サウンドバッファを取得】
//===================================================================================================================================
LIST_BUFFER *SoundBase::GetBuffer(int endpointVoiceId, int soundId, bool loop)
{
	if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_BGM)
	{
		if (loop)
		{
			BGMBufferList[soundId].buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		return &BGMBufferList[soundId];
	}
	else if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_SE)
	{
		if (loop)
		{
			SEBufferList[soundId].buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		return &SEBufferList[soundId];
	}
	return nullptr;
}

//===================================================================================================================================
//【ソースボイスの作成】
//===================================================================================================================================
void SoundBase::MakeSourceVoice(const PLAY_PARAMETERS playParameters, LIST_BUFFER *listBuffer)
{
	//波形のフォーマット
	WAVEFORMATEX pcm;
	memset(&pcm, 0, sizeof(pcm));

	//再生のパラメータ
	SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(soundParametersList->nodeNum - 1);
	tmpSoundParameters->playParameters = playParameters;
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
	if (playParameters.endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_BGM)
	{
		SoundInterface::GetXAudio2Interface()->CreateSourceVoice(
			&tmpSoundParameters->SourceVoice,
			&pcm,
			XAUDIO2_VOICE_USEFILTER,
			XAUDIO2_MAX_FREQ_RATIO,
			NULL,
			&SoundInterface::GetSendList(ENDPOINT_VOICE_LIST::ENDPOINT_BGM),
			NULL);
	}
	else if (playParameters.endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_SE)
	{
		SoundInterface::GetXAudio2Interface()->CreateSourceVoice(
			&tmpSoundParameters->SourceVoice,
			&pcm,
			XAUDIO2_VOICE_USEFILTER,
			XAUDIO2_MAX_FREQ_RATIO,
			NULL,
			&SoundInterface::GetSendList(ENDPOINT_VOICE_LIST::ENDPOINT_SE),
			NULL);
	}
	//バッファの提出
	tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&listBuffer->buffer);

	//ノードの更新処理
	soundParametersList->insertAfter(soundParametersList->getNode(soundParametersList->nodeNum - 1));
	soundParametersList->listUpdate();

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
