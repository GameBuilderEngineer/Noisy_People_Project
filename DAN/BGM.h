//===================================================================================================================================
//【BGM.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/14
// [更新日]2019/10/14
//===================================================================================================================================
#pragma once
//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "LinkedList.h"
#include "SoundBase.h"

//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum SPLASH_BGM_LIST
{
	SPLASH_BGM_MAX
};

enum TITLE_BGM_LIST
{
	BGM_Title,
	TITLE_BGM_MAX
};

enum CREDIT_BGM_LIST
{
	BGM_Credit,
	CREDIT_BGM_MAX
};

enum GAME_BGM_LIST
{
	BGM_Game,
	GAME_BGM_MAX
};

enum TUTORIAL_BGM_LIST
{
	BGM_Tutorial,
	TUTORIAL_BGM_MAX
};

enum CREATE_BGM_LIST
{
	CREATE_BGM_MAX
};

enum RESULT_BGM_LIST
{
	BGM_Clear,
	BGM_Failed,
	RESULT_BGM_MAX
};

//===================================================================================================================================
//【定数定義】
//===================================================================================================================================
static const char *creditBGMPathList[] = { "BGM_Credit.wav" };
static const char *titleBGMPathList[] = { "BGM_Title.wav" }; 
static const char *tutorialBGMPathList[] = { "BGM_Tutorial.wav" };
static const char *gameBGMPathList[] = { "BGM_Game.wav" };
static const char *resultBGMPathList[] = { "BGM_Clear.wav","BGM_Failed.wav" };
//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドのBGMクラス
//===================================================================================================================================
class BGMManager : public SoundBase
{
public:
	BGMManager();
	~BGMManager();

	//関数
	void	 SwitchAudioBuffer(int scene)override;	//ステージ遷移に合わせて必要なサウンドバッファを用意する
	void	 SetSpeed(void);							//再生速度の設定
	
	//debug用
#if _DEBUG
	void	 outputGUI(void)override;				//ImGUIへの出力
#endif
};