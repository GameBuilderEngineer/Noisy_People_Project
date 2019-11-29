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
#define S3D_PATH_LIST_TAIL(scene,num)			scene##S3DPathList##num

//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum SPLASH_S3D_LIST
{
	SPLASH_S3D_MAX
};

enum TITLE_S3D_LIST
{
	TITLE_S3D_MAX
};

enum CREDIT_S3D_LIST
{
	CREDIT_S3D_MAX
};

enum GAME_S3D_LIST
{
	S3D_GAME_FOOTSTEP_01,
	S3D_GAME_FOOTSTEP_02,
	S3D_GAME_FOOTSTEP_03,
	GAME_S3D_MAX
};

enum TUTORIAL_S3D_LIST
{
	S3D_TUTORIAL_FOOTSTEP_01,
	S3D_TUTORIAL_FOOTSTEP_02,
	S3D_TUTORIAL_FOOTSTEP_03,
	TUTORIAL_S3D_MAX
};

enum CREATE_S3D_LIST
{
	CREATE_S3D_MAX
};

enum RESULT_S3D_LIST
{
	RESULT_S3D_MAX
};

//===================================================================================================================================
//【定数定義】
//===================================================================================================================================
static const char *gameS3DPathList[] = { "3D_ADD_TREE01.wav","3D_Enemy_Walk00.wav","3D_Enemy_Walk01.wav" };
static const char *tutorialS3DPathList[] = { "3D_ADD_TREE01.wav","3D_Enemy_Walk00.wav","3D_Enemy_Walk01.wav" };

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
	//void	 SwitchAudioBuffer(int scene)override;							//ステージ遷移に合わせて必要なサウンドバッファを用意する
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
#endif
};