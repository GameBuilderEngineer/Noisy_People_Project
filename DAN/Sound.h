//===================================================================================================================================
//【Sound.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/26
// [更新日]2019/10/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "SE.h"
#include "BGM.h"
#include "3DSound.h"
//===================================================================================================================================
//【マクロ定義】
//===================================================================================================================================
//サウンド情報
#define ENDPOINT_INPUT_CHANNEL	(2)			//エンドポイントボイスのチャンネル数(入力)
#define ENDPOINT_SAMPLE_RATE		(44100)		//エンドポイントボイスのサンプリング周波数(入力)

//ボイス解放処理
#define SAFE_DESTROY_VOICE(p)	if(p){ p->DestroyVoice(); p=NULL; }
//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum ENDPOINT_VOICE_LIST
{
	ENDPOINT_BGM,
	ENDPOINT_SE,
	ENDPOINT_S3D,
	ENDPOINT_MAX
};

//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドインタフェースクラス
//===================================================================================================================================
class SoundInterface
{
private:
	//マスタリングボイス
#if(XADUIO2_STATE)
	IXAudio2MasteringVoice				*MasteringVoice;					//XAudio2 Mastering Voice
#endif

public:
	SoundInterface();
	~SoundInterface();

	//マネージャー
	static SEManager		*SE;
	static BGMManager	*BGM;
	static S3DManager	*S3D;

	//インタフェース
#if(XADUIO2_STATE)
	static IXAudio2						*XAudio2Interface;				//XAudio2 COM Interface
	static IXAudio2						*GetXAudio2Interface(void);		//XAudio2のインタフェースを取得する
#endif

	//基本機能
	static void							StopAllSound(void);				//音の停止処理(全部)
	void									UpdateSound(void);				//更新処理
	void									outputSoundGUI(void);			//ImGUIへの出力
};
