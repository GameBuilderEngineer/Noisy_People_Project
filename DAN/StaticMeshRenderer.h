//===================================================================================================================================
//【StaticMeshRenderer.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/23
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
namespace staticMeshRendererNS
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
//【スタティックメッシュレンダラークラス】
//===================================================================================================================================
//StaticMeshRendererへ名称変更予定
class StaticMeshRenderer:public Base
{
protected:
	//Data
	LPDIRECT3DDEVICE9					device;					//DirectX9デバイス
	StaticMesh*							staticMesh;				//スタティックメッシュ
	LPDIRECT3DVERTEXDECLARATION9		declaration;			//頂点宣言
	bool								onRender;				//描画フラグ
	bool								onTransparent;			//透過フラグ
	bool								onLight;				//ライトフラグ
	bool								didRegister;			//登録フラグ
	bool								didUnRegister;			//解除フラグ
	int									objectNum;				//オブジェクトの数
	int									fillMode;				//描画モード

	//セットストリーム用バッファ
	LPDIRECT3DVERTEXBUFFER9				matrixBuffer;			//ワールドマトリックスバッファ
	LPDIRECT3DVERTEXBUFFER9				colorBuffer;			//カラーバッファ
	LPDIRECT3DVERTEXBUFFER9				uvBuffer;				//UVバッファ

	//copy用配列
	D3DXMATRIX*							worldMatrix;			//ワールドマトリックス配列
	D3DXCOLOR*							color;					//カラー配列
	D3DXVECTOR2*						uv;						//カラー配列

	//オブジェクトリスト
	staticMeshRendererNS::ObjectList*		objectList;

public:
	//Method
	StaticMeshRenderer(StaticMesh* _staticMesh);
	~StaticMeshRenderer();

	//基本関数
	void update();
	void render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//リスト操作（登録/解除/更新）
	void registerObject(Object* newObject);						//描画オブジェクトを登録
	void unRegisterObjectByID(int ID);							//描画するオブジェクトをIDにより解除
	void unRegisterObject(int instanceNo);						//描画するオブジェクトを解除
	void allUnRegister();										//描画するオブジェクトを全解除
	void updateAccessList();									//ランダムアクセス用配列の更新：インスタンスの生成または削除を行った時に更新を行う。

	//各バッファ値更新
	void updateWorldMatrix();									//ワールドマトリックスバッファを更新する
	void updateColor();											//カラーバッファを更新する

	//バッファのリソース更新
	void updateBuffer();
	//copy用配列のメモリ更新
	void updateArray();

#ifdef _DEBUG
	virtual void outputGUI();
#endif // _DEBUG

	//setter
	void enableLight();
	void disableLight();
	void enableTransparent();
	void disableTransparent();


	//getter
	StaticMesh* getStaticMesh();
	int			getObjectNum();

};