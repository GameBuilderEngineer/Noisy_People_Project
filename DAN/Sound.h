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
	ENDPOINT_MAX
};

//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドインタフェースクラス
//===================================================================================================================================
class SoundInterface
{
private:
	//変数
	IXAudio2MasteringVoice				*MasteringVoice;											//XAudio2 Mastering Voice
	IXAudio2SubmixVoice					*EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_MAX];		//XAudio2 Submix Vice(Endpoint Voice)
	XAUDIO2_SEND_DESCRIPTOR				SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_MAX];		//XAudio2 Send Descriptor(BGM/SE Endpoint Voice)

public:
	SoundInterface();
	~SoundInterface();

	//クラス
	static SEManager *SE;
	static BGMManager *BGM;
	
	//変数
	static IXAudio2						*XAudio2Interface;										//XAudio2 COM interface
	static XAUDIO2_VOICE_SENDS			SendList[ENDPOINT_VOICE_LIST::ENDPOINT_MAX];				//XAudio2 Send List(BGM/SE Endpoint Voice)

	//関数
	static XAUDIO2_VOICE_SENDS			GetSendList(int endpointVoiceId);						//送信リストを取得する
	static IXAudio2						*GetXAudio2Interface(void);								//XAudio2のインタフェースを取得する
	static void							playSound(int endpointVoiceId, int soundId, bool loop);	//再生
	static void							stopSound(int endpointVoiceId, int soundId, bool loop);	//停止
	void									UpdateSound(void);										//更新処理
	void									outputSoundGUI(void);									//ImGUIへの出力
};