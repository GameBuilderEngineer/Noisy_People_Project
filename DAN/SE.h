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
//【マクロ定義】
//===================================================================================================================================
#define SE_PATH_LIST_TAIL(num)			SEPathList##[num]

//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum SE_LIST
{
	SE_Cursor,
	SE_Decision,
	SE_Cancel,
	SE_AddTree,
	SE_Score,
	SE_Shot,
	SE_Reload,
	SE_Getlem,
	SE_RecoverPower,
	SE_BanOperation,
	SE_HitBulletTree,
	SE_Convert,
	SE_Greening,
	SE_EnemyActive,
	SE_EnemyAttack,
	SE_EnemyAttackHit,
	SE_EnemyDefeated,
	SE_StartGame,
	SE_AnnounceTelop,
	SE_Telop,
	SE_TimeUp,
	SE_BIRD_0,
	SE_BIRD_1,
	SE_WIND,
	SE_Result,
	SE_HurryUp,
	Voice_Female_Damage1,
	Voice_Female_Damage2,
	Voice_Female_Jump1,
	Voice_Female_Jump2,
	Voice_Female_Shift1,
	Voice_Female_Shift2,
	Voice_Male_Damage1,
	Voice_Male_Damage2,
	Voice_Male_Jump1,
	Voice_Male_Jump2,
	Voice_Male_Shift1,
	Voice_Male_Shift2,
	SE_MAX
};

//===================================================================================================================================
//【定数定義】
//===================================================================================================================================
static const char *SEPathList[SE_LIST::SE_MAX] = {
	"SE_Cursor.wav","SE_Decision.wav" ,"SE_Cancel.wav",
	"SE_AddTree.wav","SE_Score.wav",
	"SE_Shot.wav","SE_Reload.wav" ,"SE_Getlem.wav","SE_RecoverPower.wav",
	"SE_BanOperation.wav",
	"SE_HitBulletTree.wav",
	"SE_Convert.wav","SE_Greening.wav","SE_EnemyActive.wav",
	"SE_EnemyAttack.wav","SE_EnemyAttackHit.wav","SE_EnemyDefeated.wav",
	"SE_StartGame.wav","SE_AnnounceTelop.wav","SE_Telop.wav","SE_TimeUp.wav",
	"SE_Bird0.wav","SE_Bird1.wav","SE_Wind.wav","SE_Result.wav","SE_HurryUp.wav",
	"Voice_Female_Damage1.wav","Voice_Female_Damage2.wav","Voice_Female_Jump1.wav",
	"Voice_Female_Jump2.wav","Voice_Female_Shift1.wav","Voice_Female_Shift2.wav",
	"Voice_Male_Damage1.wav","Voice_Male_Damage2.wav","Voice_Male_Jump1.wav",
	"Voice_Male_Jump2.wav","Voice_Male_Shift1.wav","Voice_Male_Shift2.wav",
};

//===================================================================================================================================
//【サウンド(XAudio2)】
//サウンドのSEクラス
//===================================================================================================================================
class SEManager : public SoundBase
{
public:
	SEManager() { loadBuffer(); };
	~SEManager() {};


#if _DEBUG
	void	 outputGUI(void)override;				//ImGUIへの出力
#endif

private:
	//関数
	void loadBuffer()override;	//必要なサウンドバッファを用意する

};
