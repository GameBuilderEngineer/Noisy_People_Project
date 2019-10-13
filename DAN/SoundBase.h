//===================================================================================================================================
//【Soundbase.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/04
// [更新日]2019/10/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "LinkedList.h"

//===================================================================================================================================
//【マクロ定義】
//===================================================================================================================================
//チャンク処理
#define FLAG_CHUNK_FMT			(1)			//fmtのフラグ
#define FLAG_CHUNK_DATA			(1<<1)		//dataのフラグ
#define FLAG_CHUNK_END			(3)			//endポイント
#define CHUNK_ID_SIZE			(4)			//チャンクIDのバイト数(4 byte)
#define CHUNK_FMT				("fmt ")		//fmtのチャンク
#define CHUNK_DATA				("data")		//dataのチャンク

//===================================================================================================================================
//【構造体】
//===================================================================================================================================
typedef struct	// RIFFチャンク 
{
	char		riffChunk[CHUNK_ID_SIZE];
	long		riffSize;
	char		waveChunk[CHUNK_ID_SIZE];
}RIFF_CHUNK;

typedef struct	// fmt チャンク 
{
	char		fmtChunk[CHUNK_ID_SIZE];
	long		fmtSize;
	short	fmtFormatTag;
	short	fmtChannel;
	long		fmtSampleRate;
	long		fmtAvgBytesPerSec;
	short	fmtBlockAlign;
	short	fmtBitPerSample;
}FMT_CHUNK;

typedef struct	// dataチャンク 
{
	char		dataChunk[CHUNK_ID_SIZE];
	long		waveSize;
	short	*waveData;
}DATA_CHUNK;

typedef struct	// WAVファイル 
{
	RIFF_CHUNK	riff;
	FMT_CHUNK	fmt;
	DATA_CHUNK	data;
}WAV_FILE;

typedef struct	// バッファ構造体
{
	int				soundId;
	XAUDIO2_BUFFER	buffer;
	WAV_FILE			wavFile;
}LIST_BUFFER;

typedef struct //再生パラメータ
{
	IXAudio2SourceVoice *SourceVoice;	//ソースボイス
	bool					isPlaying;		//再生中?
	int					soundId;			//サウンドID
	bool					loop;			//ループ
	short				stopPoint;		//停止位置
}SOUND_PARAMETERS;

//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドのベースクラス
//===================================================================================================================================
class SoundBase
{
public:
	SoundBase();
	~SoundBase();

	void	 playSound(int soundId, bool loop);					//再生
	void	 stopSound(int soundId, bool loop);					//停止
	void	 updateSound(void);									//更新処理

protected:
	static WAV_FILE	LoadWavChunk(FILE *fp);								//WAVファイルの読み込み処理
	void				MakeSourceVoice(int soundId, bool loop, LIST_BUFFER *listBuffer);
	void				uninitSoundStop(void);									//停止(全部のサウンド)
	LIST_BUFFER		*GetBuffer(int soundId, bool loop);

	//バッファリスト
	static LIST_BUFFER *bufferList;
	static int			bufferMax;

	//ボイスリスト
	LinkedList <SOUND_PARAMETERS>*soundParametersList;
};