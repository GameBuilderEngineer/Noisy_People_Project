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

//===================================================================================================================================
//【マクロ】
//===================================================================================================================================
#define CHUNK_ID				(4)
#define ENMY_CHUNK			("ENMY")
#define EFMT_CHUNK			("EFMT")
#define ENEMY_FILE_PATH		("enemy.enemy")

//===================================================================================================================================
//【構造体】
//===================================================================================================================================
typedef struct	//ENMYチャンク
{
	char chunkId[CHUNK_ID];	//チャンクID
	short size;				//以後のサイズ(ファイルのサイズ - 8byte)
	short enemyMax;			//敵の数
}ENEMY_ENMY;

typedef struct	//EFMTチャンク
{
	char chunkId[CHUNK_ID];	//チャンクID
	short size;				//このチャンクのサイズ
	short enemyId;			//敵のID
	short enemyType;		//敵の種類
	short enemyState;		//敵の状態
	float posX;				//敵の生成位置(X)
	float posY;				//敵の生成位置(Y)
	float posZ;				//敵の生成位置(Z)
	float dirX;				//敵の向き(X)
	float dirY;				//敵の向き(Y)
	float dirZ;				//敵の向き(Z)
}ENEMY_EFMT;

typedef struct	//ENEMYファイル構造体
{
	ENEMY_ENMY enmy;			//ENMYチャンク	
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

	//関数
	enemyNS::ENEMYSET GetEnemySet(short enemyId);
	int GetEnemyMax(void);
	void SetEnemy(short enemyId, short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);
	void outputEnemyToolsGUI(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//描画用
	void initialize();
	void update();
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generate(D3DXVECTOR3 position);

private:

	//変数
	ENEMY_FILE enemyFile;					//エネミー構造体
	int EnemyListboxCurrent;				//エネミー選択用(リストボックス)
	int EnemyListboxType;					//エネミーの種類(リストボックス)
	int EnemyListboxState;					//エネミーの状態(リストボックス)

	//描画用
	StaticMeshRenderer* renderer;
	bool needUpdate;

	//関数
	void OutputEnemyFile(void);				//エネミーファイルの書き出し処理
	void CreatNewEnemyFile(void);			//エネミーファイルの新規作成
	void AddEnemyFormat(short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);
	void DeleteEnemyFormat(short enemyId);	//エネミーのフォーマット構造体を消す
	void UpdateEfmt(int oldEnemyMax);		//エネミーのフォーマット構造体を整理
	//void SpinEnemy(short enemyId);
	void SetEnemyType(short enemyId, short enemyType);
	void SetEnemyState(short enemyId, short enemyState);
	void SetEnemyPos(short enemyId,const D3DXVECTOR3 pos);
	void SetEnemyDir(short enemyId, const D3DXVECTOR3 dir);

};