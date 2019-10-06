//===================================================================================================================================
//【Sound.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/26
// [更新日]2019/09/26
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【ライブラリのロード】
//===================================================================================================================================
#include <xaudio2.h>
#include <x3daudio.h>

//===================================================================================================================================
//【必要なライブラリファイルのロード】
//===================================================================================================================================
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"x3daudio.lib")

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//【マクロ定義】
//===================================================================================================================================
//チャンクを探すループ用
#define FLAG_CHUNK_FMT		(1)			//fmtのフラグ
#define FLAG_CHUNK_DATA		(1<<1)		//dataのフラグ
#define FLAG_CHUNK_END		(3)			//endポイント

//チャンク
#define CHUNK_ID_SIZE		(4)			//チャンクIDのバイト数(4 byte)
#define CHUNK_FMT			("fmt ")	//fmtのチャンク
#define CHUNK_DATA			("data")	//dataのチャンク

//サウンドの制約
#define SOUND_SAMPLE_RATE	(44100)		//共通のサンプリング周波数
#define INPUT_CHANNEL		(2)			//共通のチャンネル数(IN)
#define OUTPUT_CHANNEL		(8)			//共通のチャンネル数(OUT)
#define SOUND_SE_MAX		(10)		//同時再生の最大数[SE]

//ボイスのボイス解放処理
#define SAFE_DESTROY_VOICE(p) if(p){ p->DestroyVoice(); p=NULL; }

//テスト
#define SOUND_PATH_TEST		("BGM_Character_Select.wav")
//===================================================================================================================================
//【構造体】
//===================================================================================================================================
typedef struct	// RIFFチャンク 
{
	char	riffChunk[CHUNK_ID_SIZE];
	long	riffSize;
	char	waveChunk[CHUNK_ID_SIZE];
}RIFF_CHUNK;

typedef struct	// fmt チャンク 
{
	char	fmtChunk[CHUNK_ID_SIZE];
	long	fmtSize;
	short	fmtFormatTag;
	short	fmtChannel;
	long	fmtSampleRate;
	long	fmtAvgBytesPerSec;
	short	fmtBlockAlign;
	short	fmtBitPerSample;
}FMT_CHUNK;

typedef struct	// dataチャンク 
{
	char	dataChunk[CHUNK_ID_SIZE];
	long	waveSize;
	short	*waveDataMain;
	//short	*waveDataSub;
}DATA_CHUNK;

typedef struct	// WAVファイル 
{
	RIFF_CHUNK	riff;
	FMT_CHUNK	fmt;
	DATA_CHUNK	data;
}WAV_FILE;

//===================================================================================================================================
//【サウンドクラス(XAudio2)】
//3Dサウンドのテスト用クラス
//===================================================================================================================================
class SOUND
{
public:
	SOUND();
	~SOUND();

	void updateSound(D3DXVECTOR3 pos, D3DXVECTOR3 front);

private:
	IXAudio2MasteringVoice	*masteringVoice;	//マスターボイス
	IXAudio2				*xAudio2;			//XAudio2のインターフェイス
	XAUDIO2_VOICE_DETAILS	details;			//ボイス情報

	X3DAUDIO_HANDLE			x3DAudio;			//3DAudioのハンドル
	X3DAUDIO_LISTENER		listener;			//リスナー
	X3DAUDIO_EMITTER		emitter;			//オーディオソース
	X3DAUDIO_CONE			emitterCone;		//オーディオソースのコン
	FLOAT32					emitterAzimuths[INPUT_CHANNEL];

	IXAudio2SourceVoice		*sourceVoice;		//ソースボイス
	IXAudio2SubmixVoice		*submixVoice;		//サブミックスボイス

	FILE					*fp;				//ファイル
	WAV_FILE				wavFile;			//音のファイル

	//関数
	WAV_FILE				LoadWavChunk(FILE *fp);
	IXAudio2SourceVoice		*setSourceVoice(const XAUDIO2_VOICE_SENDS sendList);
	void					updateSoundIMGUI(void);
};
