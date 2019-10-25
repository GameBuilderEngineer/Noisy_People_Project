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


#if(_MSC_VER >= GAME_MSC_VER)
typedef struct	// バッファ構造体
{
	int				soundId;
	XAUDIO2_BUFFER	buffer;
	WAV_FILE		wavFile;
}LIST_BUFFER;
#endif

#if(_MSC_VER < GAME_MSC_VER)
typedef struct //フィルター(殻構造体)
{
	int Type;
	float Frequency;
	float OneOverQ;
}
FAKE_FILTER_PARAMETERS;
#endif

typedef struct
{
#if(_MSC_VER >= GAME_MSC_VER)
	XAUDIO2_FILTER_PARAMETERS Parameters;
#else
	FAKE_FILTER_PARAMETERS Parameters;
#endif
}FILTER_PARAMETERS;

typedef enum //フィルターの種類
{
	SAI_LowPassFilter,
	SAI_BandPassFilter,
	SAI_HighPassFilter,
	SAI_NotchFilter,
	SAI_LowPassOnePoleFilter,
	SAI_HighPassOnePoleFilter
}FILTER_TYPE;

typedef struct //再生パラメータ
{
	int								endpointVoiceId;		//エンドポイントボイスID
	int								soundId;				//サウンドID
	bool							loop;				//ループ
	float							speed;				//再生速度
	bool							filterFlag;			//卍フィルター卍
	FILTER_PARAMETERS				filterParameters;	//卍フィルター卍
}PLAY_PARAMETERS;

#if(_MSC_VER >= GAME_MSC_VER)
typedef struct //曲のパラメータ
{
	IXAudio2SourceVoice *SourceVoice;	//ソースボイス
	PLAY_PARAMETERS		playParameters;	//再生パラメータ
	bool					isSpeed;			//再生速度変更した?
	bool					isPlaying;		//再生中?
	long					stopPoint;		//停止位置
}SOUND_PARAMETERS;
#endif

//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドのベースクラス
//===================================================================================================================================
class SoundBase
{
public:
	SoundBase();
	~SoundBase();

	void	 playSound(const PLAY_PARAMETERS playParameters);	//再生
	void	 stopSound(const PLAY_PARAMETERS playParameters);	//停止
	void	 updateSound(void);										//更新処理

protected:
	static WAV_FILE	LoadWavChunk(FILE *fp);								//WAVファイルの読み込み処理
#if(_MSC_VER >= GAME_MSC_VER)
	void				MakeSourceVoice(const PLAY_PARAMETERS playParameters, LIST_BUFFER *listBuffer);
#endif
	void				uninitSoundStop(void);								//停止(全部のサウンド)
#if(_MSC_VER >= GAME_MSC_VER)
	LIST_BUFFER		*GetBuffer(int endpointVoiceId, int soundId, bool loop);
#endif

	//バッファリスト
#if(_MSC_VER >= GAME_MSC_VER)
	static LIST_BUFFER *SEBufferList;
	static int			SEBufferMax;
	static LIST_BUFFER *BGMBufferList;
	static int			BGMBufferMax;
#endif

	//ボイスリスト
#if(_MSC_VER >= GAME_MSC_VER)
	LinkedList <SOUND_PARAMETERS>*soundParametersList;
#endif
};
