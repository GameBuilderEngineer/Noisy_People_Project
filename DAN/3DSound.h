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
//【マクロ定義】
//===================================================================================================================================
#define S3D_PATH_LIST_TAIL(num)			S3DPathList##[num]

//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum S3D_LIST
{
	S3D_ShiftStart,
	S3D_ShiftFinish,
	S3D_Vision,
	S3D_VisionStart,
	S3D_VisionFinish,
	S3D_SkyVisionStart,
	S3D_SkyVisionFinish,
	S3D_PLAYER_WALK,
	Voice_Male_Start,
	Voice_Male_Finish,
	Voice_Female_Start,
	Voice_Female_Finish,
	S3D_MAX
};

//===================================================================================================================================
//【定数定義】
//===================================================================================================================================
static const char *S3DPathList[] = { "3D_ShiftStart.wav", "3D_ShiftFinish.wav","3D_Vision.wav",
									"3D_VisionStart.wav","3D_VisionFinish.wav",	"3D_SkyVisionStart.wav",
									"3D_SkyVisionFinish.wav","3D_PLAYER_WALK.wav" ,"Voice_Male_Start.wav" ,
									"Voice_Male_Finish.wav","Voice_Female_Start.wav","Voice_Female_Finish.wav" };

//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドの3DSoundクラス
//===================================================================================================================================
class S3DManager:public SoundBase
{
public:
	S3DManager();
	~S3DManager();

	//基本機能
	void SetVolume(const PLAY_PARAMETERS playParameters, float volume);	//ボリューム

	//debug用
#if _DEBUG
	void	 outputGUI(void)override;				//ImGUIへの出力
#endif

private:
#if(XADUIO2_STATE)
	//ミッドポイントボイス
	IXAudio2SubmixVoice		*MidpointVoice[gameMasterNS::PLAYER_NUM];	//XAudio2 Midpoint Voice[2 player]
	XAUDIO2_VOICE_SENDS		MidSendList[gameMasterNS::PLAYER_NUM];		//XAudio2 Send List(P1/P2 Midpoint Voice)
	XAUDIO2_SEND_DESCRIPTOR	SendDescriptor[gameMasterNS::PLAYER_NUM];	//XAudio2 Send Descriptor(BGM/SE Endpoint Voice)

	//ソースボイスの作成
	void MakeSourceVoice(PLAY_PARAMETERS *playParameters, LIST_BUFFER *listBuffer)override;

	void loadBuffer(void)override;
#endif
};