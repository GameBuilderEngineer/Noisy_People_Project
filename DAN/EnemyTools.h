//===================================================================================================================================
//【EnemyTools.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/26
// [更新日]2019/09/26
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【ライブラリのロード】
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshRenderer.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Input.h"

//===================================================================================================================================
//【マクロ】
//===================================================================================================================================
#define ENEMY_CHUNK_ID		(4)
#define ENMY_CHUNK			("ENMY")
#define EFMT_CHUNK			("EFMT")
#define ENEMY_FILE_PATH		("enemy.enemy")
#define ENEMY_GUI_ID		(1)

//===================================================================================================================================
//【構造体】
//===================================================================================================================================
typedef struct	//ENMYチャンク
{
	char chunkId[ENEMY_CHUNK_ID];	//チャンクID
	short size;				//サイズ
	short enemyMax;			//敵の数
}ENEMY_ENMY;

typedef struct	//EFMTチャンク
{
	char	chunkId[ENEMY_CHUNK_ID];	//チャンクID
	short	size;						//このチャンクのサイズ
	short	enemyId;					//敵のID
	short	enemyType;					//敵の種類
	short	enemyState;					//敵の状態
	float	posX;						//敵の生成位置(X)
	float	posY;						//敵の生成位置(Y)
	float	posZ;						//敵の生成位置(Z)
	float	dirX;						//敵の向き(X)
	float	dirY;						//敵の向き(Y)
	float	dirZ;						//敵の向き(Z)
	short	rootSize;					//ルートの利用数
	D3DXVECTOR3	root[8];				//ルート
}ENEMY_EFMT;

typedef struct	//ENEMYファイル構造体
{
	ENEMY_ENMY enmy;		//ENMYチャンク	
	ENEMY_EFMT *efmt;		//EFMTチャンク
}ENEMY_FILE;

//===================================================================================================================================
//【エネミーツール】
//エネミーツール用クラス
//===================================================================================================================================
class ENEMY_TOOLS : public Base
{
public:
	ENEMY_TOOLS();
	~ENEMY_TOOLS();

	//ファイル
	int GetEnemyMax(void);									//エネミーの数を取得
	enemyNS::ENEMYSET GetEnemySet(short enemyId);			//エネミーセット構造体を取得

#ifdef _DEBUG
	//変数	
	int EnemyListboxType;									//エネミーの種類(リストボックス)
	BoundingSphere *bodyCollide;							//当たり判定
	Object** object;										//オブジェクト
	int Model[enemyNS::TYPE_MAX];

	//GUI
	void outputEnemyToolsGUI(int GUIid,						//GUI
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);		
	void outputRootToolGUI(int enemyId, D3DXVECTOR3 pos);	//ルートツール

	//レンダラー
	void initRender();										//レンダラーの初期化
	void initObject();										//オブジェクトの初期化
	void update();											//更新
	void render(D3DXMATRIX view,							//描画
		D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);			
	void generate(Object *object, short enemyType,			//作成
		D3DXVECTOR3 position, D3DXVECTOR3 dir);
	void collideDraw(int ID, bool use);
#endif

private:
	//変数	
	ENEMY_FILE enemyFile;									//エネミー構造体
	Input *input;

	//エネミーファイル
	void OutputEnemyFile(void);								//エネミーファイルの書き出し処理
	void CreatNewEnemyFile(void);							//エネミーファイルの新規作成

#ifdef _DEBUG
	//変数	
	StaticMeshRenderer** renderer;							//レンダラー
	bool rootEdit;											//ルートの編集フラグ
	bool needUpdate;										//更新フラグ
	int EnemyListboxRootCurrent;							//エネミー選択用(リストボックス)
	int EnemyListboxCurrent;								//エネミー選択用(リストボックス)
	int EnemyListboxState;									//エネミーの状態(リストボックス)

	//レンダラー
	void ResetRenderer(void);								//レンダラーをリセット

	//エネミーの設置
	void SetEnemyType(short enemyId, short enemyType);		//エネミーの種類を設定	
	void SetEnemyState(short enemyId, short enemyState);	//エネミーの状態を設定
	void SetEnemyPos(short enemyId, const D3DXVECTOR3 pos);	//エネミーの位置を設定
	void SetEnemyDir(short enemyId, const D3DXVECTOR3 dir);	//エネミーの向きを設定
	void SetEnemy(short enemyId, short enemyType,			//エネミーの設置
		short enemyState, const D3DXVECTOR3 pos,
		const D3DXVECTOR3 dir);

	//エネミーフォーマット構造体
	void UpdateEfmt(int oldEnemyMax);						//エネミーフォーマットの整理
	void DeleteEnemyFormat(short enemyId);					//エネミーフォーマットの削除
	void AddEnemyFormat(short enemyType,					//エネミーフォーマットの追加
		short enemyState, const D3DXVECTOR3 pos,
		const D3DXVECTOR3 dir);
#endif		
};