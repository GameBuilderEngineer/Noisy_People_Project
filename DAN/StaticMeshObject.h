//===================================================================================================================================
//【Object.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/17
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Object.h"
#include "LinkedList.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace staticMeshObjectNS
{
	enum FILLMODE
	{
		SOLID = (int)D3DFILL_SOLID,
		WIREFRAME = (int)D3DFILL_WIREFRAME,
		POINT = (int)D3DFILL_POINT,
	};

	class ObjectList :public LinkedList<Object*>
	{

	};
}

//===================================================================================================================================
//【スタティックメッシュオブジェクトクラス】
//===================================================================================================================================
class StaticMeshObject:public Base
{
protected:
	//Data
	LPDIRECT3DDEVICE9							device;					//DirectX9デバイス
	StaticMesh*										staticMesh;			//スタティックメッシュ
	LPDIRECT3DVERTEXDECLARATION9	declaration;			//頂点宣言
	bool													onRender;				//描画フラグ
	bool													onTransparent;		//透過フラグ
	bool													didGenerate;			//生成フラグ
	bool													didDelete;				//削除フラグ
	int														objectNum;			//オブジェクトの数
	int														fillMode;				//描画モード

	//セットストリーム用バッファ
	LPDIRECT3DVERTEXBUFFER9				positionBuffer;		//位置バッファ
	LPDIRECT3DVERTEXBUFFER9				rotationBuffer;		//回転バッファ
	LPDIRECT3DVERTEXBUFFER9				scaleBuffer;			//スケールバッファ
	LPDIRECT3DVERTEXBUFFER9				colorBuffer;			//カラーバッファ
	LPDIRECT3DVERTEXBUFFER9				uvBuffer;				//UVバッファ

	//copy用配列
	D3DXVECTOR3*									position;				//位置配列
	D3DXVECTOR3*									rotation;				//回転配列
	D3DXVECTOR3*									scale;					//スケール配列
	D3DXCOLOR*									color;					//カラー配列

	//オブジェクトリスト
	staticMeshObjectNS::ObjectList* objectList;

public:
	//Method
	StaticMeshObject(StaticMesh* _staticMesh);
	~StaticMeshObject();

	//基本関数
	void update();
	void render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//リスト操作
	void generateObject(Object* newObject);							//新たなオブジェクトの生成
	void deleteObject(int instanceNo);										//描画するインスタンスを削除
	void updateAccessList();														//ランダムアクセス用配列の更新：インスタンスの生成または削除を行った時に更新を行う。

	//各バッファ値更新
	void updatePosition();																				//位置バッファを更新する
	void updateRotation();																			//回転バッファを更新する
	void updateScale();																				//スケールバッファを更新する
	void updateColor();																				//カラーバッファを更新する

	//バッファのリソース更新
	void updateBuffer();
	//copy用配列のメモリ更新
	void updateArray();
	
#ifdef _DEBUG
	virtual void outputGUI();
#endif // _DEBUG

	//getter
	StaticMesh* getStaticMesh();

};