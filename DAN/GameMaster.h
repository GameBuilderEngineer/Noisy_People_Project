//===================================================================================================================================
//【GameMaster.h】
// [作成者] HAL東京GP12A332 11 菅野 樹
// [作成日] 2019/09/20
// [更新日] 2019/10/24
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshLoader.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace gameMasterNS {

	enum PLAYER_TYPE
	{
		PLAYER_1P,
		PLAYER_2P,
		TYPE_NUM
	};

	const int MODEL_MALE		= staticMeshNS::YAMADA_ROBOT;
	const int MODEL_FEMALE		= staticMeshNS::YAMADA_ROBOT2;
	const int MODEL_TYPE_MAX = 2;

	enum RESULT_LIST
	{
		CLEAR,
		FAILED,
		RESULT_NUM,
	};

	enum PLAYER_RANK
	{
		RANK_S,
		RANK_A,
		RANK_B,
		RANK_C,
		RANK_NUM
	};



	const float GAME_TIME				= 60.0f * 4.0f;			//4分
	const float COUNT_DOWN_TIME			= 3.0f;					//3秒

	const int	PLAYER_NUM				= 2;

}

//===================================================================================================================================
//【構造体定義】
//===================================================================================================================================
//プレイヤー情報
struct PlayerTable
{
	int	playerType;		//1P|2P
	int	modelType;		//プレイヤーのモデルタイプ
	int greeningTree;	//緑化した本数
	int sedationEnemy;	//鎮静化したエネミー
};
//ツリー情報
struct TreeTable
{
	int				treeType;			//ANALOG|DIGITAL
	int				modelType;			//モデルタイプ
	int				player;				//緑化したプレイヤー
	int				greeningOrder;		//緑化された順位
	float			greeningTime;		//緑化された時間
	D3DXVECTOR3		position;			//位置
	D3DXQUATERNION	rotation;			//回転
	D3DXVECTOR3		scale;				//スケール

};

//===================================================================================================================================
//【ゲーム管理クラス】
// [用途]	このクラスは、シーンをまたいで情報の橋渡しを行う。
//			更新や設定は、各シーンで行う。
//===================================================================================================================================
class GameMaster :public Base
{
private:
	//Data
	//ゲーム
	float			gameTimer;										//ゲーム時間
	float			countDownTimer;									//カウントダウン時間
	bool			pause;											//ポーズ

	//木関係情報
	int				treeNum;										//枯木・緑化木の総計
	int*			conversionOrder;								//変換順番：緑化された順番
	
	//プレイヤー
	PlayerTable		playerInformation[gameMasterNS::PLAYER_NUM];	//プレイヤー情報


public:
	//基本処理
	GameMaster();
	~GameMaster();
	void initialize();
	void update(float frameTime);



	//ゲーム
	void startGame();												//ゲーム開始関数
	void updateGameTime(float frameTime);							//ゲーム時間の更新
	bool paused();													//ポーズ処理


	//木の設定関数
	void readyConversionOrder(int treeNum);							//変換順番変数を準備する
	void discardConversionOrder();									//変換順番変数を破棄する
	void recordGreeningTree(int treeNo,int orderNo);				//緑化した木の本数を記録

	//setter
	void setConversionOrder(int* newValue);

	//getter
	PlayerTable*	getPlayerInfomation();
	float			getGameTime();

#ifdef _DEBUG
	bool			showGUI;										//GUIの可視フラグ
	void			createGUI();									//GUIの作成
	bool*			getShowGUI() { return &showGUI; };				//GUIの可視フラグgetter
#endif
};