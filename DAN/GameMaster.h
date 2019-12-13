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
#include "LinkedList.h"

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

	const float OPENING_TIME			= 5.0f;					//5秒
	const float GAME_TIME				= 60.0f * 1.0f;			//4分
	const float COUNT_DOWN_TIME			= 3.0f;					//3秒
	const float ENDING_TIME				= 3.0f;					//3秒

	const int	PLAYER_NUM				= 2;

	const int	ACHIEVEMENT_GREENING_RATE_10	= 0x00000001;//緑化率10%
	const int	ACHIEVEMENT_GREENING_RATE_30	= 0x00000002;//緑化率30%
	const int	ACHIEVEMENT_GREENING_RATE_50	= 0x00000004;//緑化率50%
	const int	PASSING_REMAINING_ONE_MINUTE	= 0x00000008;//残り時間1分
	const int	PASSING_GAME_OPENING			= 0x00000010;//OP終了
	const int	PASSING_COUNT_DOWN_THREE		= 0x00000020;//3カウントダウン
	const int	PASSING_COUNT_DOWN_TWO			= 0x00000040;//2カウントダウン
	const int	PASSING_COUNT_DOWN_ONE			= 0x00000080;//1カウントダウン
	const int	PASSING_GAME_START				= 0x00000100;//ゲームスタート
	const int	PASSING_REMAINING_10			= 0x00000200;//残り10描画
	const int	PASSING_REMAINING_9				= 0x00000400;//残り9描画
	const int	PASSING_REMAINING_8				= 0x00000800;//残り8描画
	const int	PASSING_REMAINING_7				= 0x00001000;//残り7描画
	const int	PASSING_REMAINING_6				= 0x00002000;//残り6描画
	const int	PASSING_REMAINING_5				= 0x00004000;//残り5描画
	const int	PASSING_REMAINING_4				= 0x00008000;//残り4描画
	const int	PASSING_REMAINING_3				= 0x00010000;//残り3描画
	const int	PASSING_REMAINING_2				= 0x00020000;//残り2描画
	const int	PASSING_REMAINING_1				= 0x00040000;//残り1描画
	const int	PASSING_GAME_FINISH				= 0x00080000;//ゲーム終了
	const int	PASSING_GAME_ENDING				= 0x00100000;//ED終了
	

	enum EVENT_TYPE
	{
		TO_DEAD,				//枯木へ戻る
		TO_GREEN_WITH_DIGITAL,	//緑化(デジタル)
		TO_GREEN_WITH_ANALOG,	//緑化(アナログ)
	};
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
	int							id;					//ツリーID
	gameMasterNS::EVENT_TYPE	eventType;			//ANALOG|DIGITAL
	int							modelType;			//モデルタイプ
	int							player;				//緑化したプレイヤー
	float						eventTime;			//緑化された時間
	bool						playBacked;			//リザルト再生(完了:true)
	D3DXVECTOR3					position;			//位置
	D3DXQUATERNION				rotation;			//回転
	D3DXVECTOR3					scale;				//スケール
	
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
	float					openingTimer;									//オープニング時間
	float					gameTimer;										//ゲーム時間
	float					countDownTimer;									//カウントダウン時間
	bool					pause;											//ポーズ
	bool					gameTimerStop;									//ゲームタイマーストップ

	//木関係情報
	int						treeNum;										//枯木・緑化木の総計
	LinkedList<TreeTable>	treeTableList;									//変換順番：緑化された順番
	
	//エネミー撃退数
	int						killEnemyNum[gameMasterNS::PLAYER_NUM];			//撃退エネミー数

	//プレイヤー
	PlayerTable				playerInformation[gameMasterNS::PLAYER_NUM];	//プレイヤー情報
	int						progress;										//達成状況


public:
	//基本処理
	GameMaster();
	~GameMaster();
	void initialize();
	void update(float frameTime);

	//ゲーム
	void startGame();												//ゲーム開始関数

	void updateTimer(float frameTime);								//各タイマー更新
	void updateOpeningTime(float frameTime);						//ゲーム時間の更新
	void updateGameTime(float frameTime);							//ゲーム時間の更新
	void updateStartCountDown(float frameTime);						//ゲーム開始カウントダウンの更新
	void updateFinishCountDown(float frameTime);						//ゲーム開始カウントダウンの更新
	bool paused();													//ポーズ処理
	bool playActionRamaining1Min();									//残り1分経過時のアクション
	bool playActionStartCount(int countNum);						//開始カウント3時のアクション
	bool playActionFinishCount(int countNum);						//終了カウント3時のアクション

	//木の設定関数
	void setTreeNum(int num);										//ツリーの数を保存する
	void discardConversionOrder();									//変換順番変数を破棄する
	void recordTreeTable(TreeTable treeTable);						//緑化した木の本数を記録
	int	 getGreeningRate();											//緑化率の取得
	int  getGreeningTreeNum(int playerNo);							//緑化本数
	int  getKillEnemyNum(int playerNo);								//撃退エネミー数
	
	//再生イベントの取得
	//※取得したリストは必ずSAFE_DELETE_ARRAYすること
	//TreeTable* out ：ポインタを代入するとリストが格納される。
	//float		 time：経過時間を代入する
	int getEventList(TreeTable** out,float time);

	//setter
	void setConversionOrder(int* newValue);
	void setProgress(int achievement);

	//getter
	PlayerTable*	getPlayerInfomation();
	float			getGameTime();
	int				getProgress();									//達成状況取得
	bool			whetherAchieved(int achievement);

#ifdef _DEBUG
	bool			showGUI;										//GUIの可視フラグ
	void			createGUI();									//GUIの作成
	bool*			getShowGUI() { return &showGUI; };				//GUIの可視フラグgetter
#endif
};