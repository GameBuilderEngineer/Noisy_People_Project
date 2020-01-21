//============================
//[UIcharacter.h]
// 染矢　晃介
//============================

//============================
//インクルード
//============================
#include "Result_chara.h"
#include "Player_Chara.h"
#include "Greenig_Chara.h"
#include "Greening_Moji.h"
#include "Defeat_Chara.h"
#include "Defeat_Moji.h"
#include "WholeGreening.h"
#include "Persent.h"

//============================
//名前空間 定数
//============================
namespace uiCharacterNS
{
	//UIの種類
	enum UICHARACTER_TYPE
	{
		RESULT,
		PLAYER1,
		PLAYER2,
		GREENIG,
		RYOKUKA,
		PERSENT,
		DEFEAT,
		GEKITAI,
		UICHARACTER_MAX,	//UI(文字)の総数
	};

	//リザルトのフェイズ
	enum RESULT_PHASE
	{
		PHASE_01,		//リザルトロゴのスライドイン
		PHASE_02,		//島表示
		PHASE_03,		//結果表示(数字など)
		PHASE_04,		//タイトルに戻るフェイズ
		PHASE_05,
		PHASE_MAX,		//フェイズの総数
	};

	
}
//============================
//クラス定義
//============================
class UIcharacter:public BasicUI
{
public: //メンバー変数
	Result_Chara* result;
	Player_Chara* player;
	Greening_Chara* greenChara;
	Greening_Moji* greenMoji;
	Defeat_Chara* defeatChara;
	Defeat_Moji* defeatMoji;
	WholeGreening* greening;
	Persent* persent;

public: //メンバー関数
	UIcharacter();						//コンストラクタ
	~UIcharacter();						//デストラクタ
	void initialize(int playerType);	//初期化
	void render(int resultPhase);		//描画
	void update(int resultPhase, int playerType,float flametime);		//更新
	
};


