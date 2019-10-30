//===================================================================================================================================
//【3DSound.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/29
// [更新日]2019/10/29
//===================================================================================================================================
#pragma once
//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "GameMaster.h"
#include "LinkedList.h"
#include "SoundBase.h"

//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドの3DSoundクラス
//===================================================================================================================================
class X3DSoundManager:public SoundBase
{
public:
	X3DSoundManager();
	~X3DSoundManager();

private:
	IXAudio2SubmixVoice		*MidpointVoice[gameMasterNS::PLAYER_NUM];	//XAudio2 Midpoint Voice[2 player]
	XAUDIO2_VOICE_SENDS		SendList[gameMasterNS::PLAYER_NUM];			//XAudio2 Send List(P1/P2 Midpoint Voice)
	XAUDIO2_SEND_DESCRIPTOR	SendDescriptor[gameMasterNS::PLAYER_NUM];	//XAudio2 Send Descriptor(BGM/SE Endpoint Voice)
};