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
LIST_BUFFER *SoundBase::bufferList = nullptr;
int	SoundBase::bufferMax = 0;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
SoundBase::SoundBase()
{
	//リストの作成
	soundParametersList = new LinkedList <SOUND_PARAMETERS>;
	soundParametersList->insertFront();
	soundParametersList->listUpdate();
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
SoundBase::~SoundBase()
{
	uninitSoundStop();
	soundParametersList->terminate();
	SAFE_DELETE(soundParametersList);
}
//===================================================================================================================================
//【停止(全部のサウンド)】
//===================================================================================================================================
void	 SoundBase::uninitSoundStop(void)
{
	SOUND_PARAMETERS *tmpSoundParameters = nullptr;

	for (int i = 0; i < soundParametersList->nodeNum; i++)
	{
		tmpSoundParameters = soundParametersList->getValue(i);

		if (tmpSoundParameters->isPlaying)		//再生している
		{
			XAUDIO2_VOICE_STATE voiceState;
			tmpSoundParameters->SourceVoice->GetState(&voiceState);

			tmpSoundParameters->stopPoint = (short)voiceState.SamplesPlayed;

			tmpSoundParameters->SourceVoice->Stop();
			tmpSoundParameters->isPlaying = false;
			SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice)
		}
	}
}

//===================================================================================================================================
//【再生】
//===================================================================================================================================
void SoundBase::playSound(int soundId, bool loop)
{
	SOUND_PARAMETERS *tmpSoundParameters = nullptr;

	for (int i = 0; i < soundParametersList->nodeNum; i++)
	{
		tmpSoundParameters = soundParametersList->getValue(i);

		if ((!tmpSoundParameters->isPlaying) &&				//再生していない
			(tmpSoundParameters->soundId == soundId))		//IDが一致する
		{
			tmpSoundParameters->SourceVoice->Start();
			tmpSoundParameters->isPlaying = true;
			return;
		}
	}

	//リスト中に存在しない、再生していないボイスがない時は新しく追加する
	MakeSourceVoice(tmpSoundParameters, GetBuffer(soundId, loop));
}

//===================================================================================================================================
//【停止】
//===================================================================================================================================
void	 SoundBase::stopSound(int soundId, bool loop)
{
	SOUND_PARAMETERS *tmpSoundParameters = nullptr;

	for (int i = 0; i < soundParametersList->nodeNum; i++)
	{
		tmpSoundParameters = soundParametersList->getValue(i);

		if ((tmpSoundParameters->isPlaying) &&				//再生している
			(tmpSoundParameters->soundId == soundId))		//IDが一致する
		{
			XAUDIO2_VOICE_STATE voiceState;
			tmpSoundParameters->SourceVoice->GetState(&voiceState);

			tmpSoundParameters->stopPoint = (short)voiceState.SamplesPlayed;

			tmpSoundParameters->SourceVoice->Stop();
			tmpSoundParameters->isPlaying = false;
			return;
		}
	}
}

//===================================================================================================================================
//【更新処理】
//===================================================================================================================================
void	 SoundBase::updateSound(void)
{
	//再生状態をチェック
	for (int i = 0; i < soundParametersList->nodeNum; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		if (tmpSoundParameters != NULL)
		{
			XAUDIO2_VOICE_STATE voiceState;
			tmpSoundParameters->SourceVoice->GetState(&voiceState);

			//再生し終わった
			if (voiceState.BuffersQueued == 0)
			{
				SAFE_DELETE(tmpSoundParameters);
				soundParametersList->listUpdate();
			}
		}
	}
}

//===================================================================================================================================
//【サウンドバッファを取得】
//===================================================================================================================================
LIST_BUFFER *SoundBase::GetBuffer(int soundId, bool loop)
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
void SoundBase::MakeSourceVoice(SOUND_PARAMETERS *soundParameters, LIST_BUFFER *listBuffer)
{
	//波形のフォーマット
	WAVEFORMATEX pcm;
	memset(&pcm, 0, sizeof(pcm));

	//再生のパラメータ
	SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(soundParametersList->nodeNum - 1);
	tmpSoundParameters->stopPoint = soundParameters->stopPoint;
	tmpSoundParameters->soundId = soundParameters->soundId;
	tmpSoundParameters->loop = soundParameters->loop;
	tmpSoundParameters->isPlaying = false;

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
		&SoundInterface::GetSendList(ENDPOINT_VOICE_LIST::ENDPOINT_SE),
		NULL);

	//バッファの提出
	tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&listBuffer->buffer);

	//ノードの更新処理
	soundParametersList->listUpdate();

	//再生
	playSound(tmpSoundParameters->soundId, tmpSoundParameters->loop);
}

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
			tmpWavFile.data.waveData = (short *)malloc(tmpWavFile.data.waveSize);
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

	fclose(fp);

	return tmpWavFile;
}
