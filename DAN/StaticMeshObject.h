//===================================================================================================================================
//【Object.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/16
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
//【スタティックメッシュオブジェクト】
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
	int														fillMode;				//

	//セットストリーム用バッファ
	LPDIRECT3DVERTEXBUFFER9				positionBuffer;		//位置バッファ

	//copy用配列
	D3DXVECTOR3*									position;				//位置配列

	//オブジェクトリスト
	staticMeshObjectNS::ObjectList* objectList;

public:
	//Method
	StaticMeshObject(StaticMesh* _staticMesh);
	~StaticMeshObject();

	void update();
	void updateBuffer();
	void updatePosition();
	void updateArray();
	void render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//リスト操作
	void generateObject(Object* newObject);							//新たなオブジェクトの生成
	void deleteObject(int instanceNo);										//描画するインスタンスを削除
	void updateAccessList();														//ランダムアクセス用配列の更新：インスタンスの生成または削除を行った時に更新を行う。

#ifdef _DEBUG
	virtual void outputGUI();
#endif // _DEBUG

	//getter
	StaticMesh* getStaticMesh();
};


