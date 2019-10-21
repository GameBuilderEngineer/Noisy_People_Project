//===================================================================================================================================
//【SE.h】
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
#include "SoundBase.h"

//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum SPLASH_SE_LIST
{
	SPLASH_SE_01,
	SPLASH_SE_MAX
};

enum TITLE_SE_LIST
{
	TITLE_SE_01,
	TITLE_SE_02,
	TITLE_SE_MAX
};

enum GAME_SE_LIST
{
	GAME_SE_01,
	GAME_SE_02,
	GAME_SE_MAX
};

enum CREATE_SE_LIST
{
	CREATE_SE_MAX
};

//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドのSEクラス
//===================================================================================================================================
class SEManager : public SoundBase
{
public:
	SEManager() {};
	~SEManager();

	//変数
	static const char * const splashSEPathList[];
	static const char * const titleSEPathList[];
	static const char * const gameSEPathList[];
	static int	SEScene;

	//関数
	static void		SwitchAudioBuffer(int scene);	//ステージ遷移に合わせて必要なサウンドバッファを用意する
	void				outputSEGUI(void);				//ImGUIへの出力
};