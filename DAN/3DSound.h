//===================================================================================================================================
//y3DSound.hz
// [ì¬ŽÒ]HAL“Œ‹žGP12A332 16 äï —F„
// [ì¬“ú]2019/10/29
// [XV“ú]2019/10/29
//===================================================================================================================================
#pragma once
//===================================================================================================================================
//yƒCƒ“ƒNƒ‹[ƒhz
//===================================================================================================================================
#include "Base.h"
#include "GameMaster.h"
#include "LinkedList.h"
#include "SoundBase.h"

//===================================================================================================================================
//yƒTƒEƒ“ƒh(XAudio2)z
//ƒTƒEƒ“ƒh‚Ì3DSoundƒNƒ‰ƒX
//===================================================================================================================================
class X3DSoundManager:public SoundBase
{
public:
	X3DSoundManager();
	~X3DSoundManager();

private:
#if(_MSC_VER >= GAME_MSC_VER)
	IXAudio2SubmixVoice		*MidpointVoice[gameMasterNS::PLAYER_NUM];	//XAudio2 Midpoint Voice[2 player]
	XAUDIO2_VOICE_SENDS		SendList[gameMasterNS::PLAYER_NUM];			//XAudio2 Send List(P1/P2 Midpoint Voice)
	XAUDIO2_SEND_DESCRIPTOR	SendDescriptor[gameMasterNS::PLAYER_NUM];	//XAudio2 Send Descriptor(BGM/SE Endpoint Voice)
#endif
};