//===================================================================================================================================
//【MapObjectTool.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/11/25
// [更新日]2019/11/25
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【ライブラリのロード】
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshRenderer.h"
#include "MapObject.h"

//===================================================================================================================================
//【マクロ】
//===================================================================================================================================
#define MPOJ_CHUNK_ID		(4)
#define MPOJ_CHUNK			("MPOJ")
#define MPOJ_CHUNK			("MFMT")
#define MPOJ_FILE_PATH		("mpoj.mpoj")
#define MPOJ_GUI_ID			(3)

//===================================================================================================================================
//【構造体】
//===================================================================================================================================
typedef struct //MPOJチャンク
{
	char chunkId[MPOJ_CHUNK_ID];
	short size;
	short mpojMax;	
}MPOJ_MPOJ;

typedef struct //MFMTチャンク
{
	char chunkId[MPOJ_CHUNK_ID];	
	short size;
	short mpojId;
	short mpojType;
	float posX;
	float posY;
	float posZ;
	float dirX;
	float dirY;
	float dirZ;

}MPOJ_MFMT;

typedef struct	//MPOJファイル構造体
{
	MPOJ_MPOJ mpoj;		//MPOJチャンク	
	MPOJ_MFMT *mfmt;		//MFMTチャンク
}MPOJ_FILE;

//===================================================================================================================================
//【マップオブジェクトツール】
//マップオブジェクトツール用クラス
//===================================================================================================================================
class MPOJ_TOOLS : public Base
{
public:
	MPOJ_TOOLS();
	~MPOJ_TOOLS();

	//ファイル
	int GetMpojMax(void);									//マップオブジェクトの数を取得
	mapObjectNS::MAPOBJECTSET GetMpojSet(short mpojId);				//マップオブジェクトセット構造体を取得

#ifdef _DEBUG
	//変数
	int MpojListboxType;									//マップオブジェクトの種類(リストボックス)
	BoundingSphere *bodyCollide;							//当たり判定
	Object** object;										//オブジェクト
	int Model[mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX];
	//GUI
	void outputMpojToolsGUI(int GUIid,						//GUI
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//レンダラー
	void initRender();										//レンダラーの初期化
	void initObject();										//オブジェクトの初期化
	void update();											//更新
	void render(D3DXMATRIX view,							//描画
		D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generateMapobj(Object *object, short mpojType,		//作成
		D3DXVECTOR3 position, D3DXVECTOR3 dir);
#endif

private:

	//変数
	MPOJ_FILE mpojFile;										//マップオブジェクト構造体

	//マップオブジェクトファイル
	void OutputMpojFile(void);								//マップオブジェクトファイルの書き出し処理
	void CreatNewMpojFile(void);							//マップオブジェクトファイルの新規作成

#ifdef _DEBUG
	//変数	
	StaticMeshRenderer** mapobjRenderer;						//レンダラー
	bool needUpdate;										//更新フラグ
	int MpojListboxCurrent;									//マップオブジェクト選択用(リストボックス)

	//レンダラー
	void ResetRenderer(void);								//レンダラーをリセット

	//マップオブジェクトフォーマット構造体
	void UpdateMfmt(int oldMpojMax);						//マップオブジェクトのフォーマット構造体を整理
	void DeleteMpojFormat(short mpojId);					//マップオブジェクトのフォーマット構造体を消す
	void AddMpojFormat(short mpojType,
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//マップオブジェクトの設置
	void SetMpojType(short mpojId, short mpojType);			//マップオブジェクトの種類を設定
	void SetMpojPos(short mpojId, const D3DXVECTOR3 pos);	//マップオブジェクトの位置を設定
	void SetMpojDir(short mpojId, const D3DXVECTOR3 dir);	//マップオブジェクトの向きを設定
	void SetMpoj(short mpojId, short mpojType,				//マップオブジェクトの設置
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

#endif
};