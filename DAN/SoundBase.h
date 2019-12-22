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
#include "SaiDelayReverb.h"
#include "SaiFadeIn.h"

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

//3D
#define DISTANCE_MAX			(10)		//偽サウンドコン

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


#if(XADUIO2_STATE)
typedef struct	// バッファ構造体
{
	int				soundId;
	XAUDIO2_BUFFER	buffer;
	WAV_FILE			wavFile;
}LIST_BUFFER;
#endif

#if(!XADUIO2_STATE)
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
#if(XADUIO2_STATE)
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
	int						endpointVoiceId;		//エンドポイントボイスID
	int						soundId;				//サウンドID
	bool						loop;				//ループ
	float					speed;				//再生速度
	bool						S3D;					//3D?
	int						playerID;			//プレイヤーID
	bool						filterFlag;			//卍フィルター卍
	FILTER_PARAMETERS		filterParameters;	//卍フィルター卍
	long						voiceID;				//ボイスID
}PLAY_PARAMETERS;

#if(XADUIO2_STATE)
typedef struct //曲のパラメータ
{
	IXAudio2SourceVoice		*SourceVoice;	//ソースボイス
	PLAY_PARAMETERS			playParameters;	//再生パラメータ
	bool						isSpeed;			//再生速度変更した?
	bool						isPlaying;		//再生中?
	long						stopPoint;		//停止位置
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

	//サウンド機能
	void playSound(PLAY_PARAMETERS *playParameters);	//再生
	void stopSound(const PLAY_PARAMETERS playParameters);	//停止
	void updateSound(void);									//更新
	virtual void	 SwitchAudioBuffer(int scene) {};			//ステージ遷移に合わせて必要なサウンドバッファを用意する
	void uninitSoundStop(void);								//停止(全部のサウンド)
	void setEndPointVoiceVolume(float volume);				//エンドポイントボイスのボリューム
	void setDelay(D3DXVECTOR3 pos);							//ディレイ
protected:
#if(XADUIO2_STATE)
	//エンドポイントボイス
	IXAudio2SubmixVoice			*EndpointVoice;				//XAudio2 Submix Vice(Endpoint Voice)
	XAUDIO2_SEND_DESCRIPTOR		SendDescriptor;				//XAudio2 Send Descriptor(BGM/SE Endpoint Voice)
	XAUDIO2_VOICE_SENDS			SendList;					//XAudio2 Send List(BGM/SE Endpoint Voice)

	//バッファ管理
	LIST_BUFFER	*GetBuffer(int endpointVoiceId, int soundId, bool loop);
	virtual void MakeSourceVoice(PLAY_PARAMETERS *playParameters, LIST_BUFFER *listBuffer);
#endif

	//基本機能(読み込み・停止)
	static WAV_FILE	LoadWavChunk(FILE *fp);					//WAVファイルの読み込み処理
	virtual void loadBuffer() {};							//必要なサウンドバッファを用意する

	//リスト
#if(XADUIO2_STATE)
	LinkedList <SOUND_PARAMETERS>*soundParametersList;		//パラーメータリスト
	long			voiceIDCnt;									//ボイスIDカウンター
	LIST_BUFFER *bufferList;									//バッファリスト
	int			bufferMax;									//バッファの最大数
#endif

	//debug
#if _DEBUG
	virtual	void	 outputGUI(void) {};							//ImGUIへの出力
#endif

	//APO
	IUnknown *SaiDelayApo;
	IUnknown *SaiFadeInApo;
	//エフェクト
	SAI_APO_DELAY_REVERB saiApoDelayParameters;
	SAI_APO_FADE_IN saiApoFadeInParameters;
	// エフェクトチェン
	XAUDIO2_EFFECT_CHAIN chain;
	// ディスクリプタ
	XAUDIO2_EFFECT_DESCRIPTOR sourceDescriptor[2];
};