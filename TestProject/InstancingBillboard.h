//===================================================================================================================================
//【InstancingBillboard.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/27
// [更新日]2019/09/28
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Object.h"
#include "LinkedList.h"

//===================================================================================================================================
//【インスタンシングビルボード頂点構造体】
//===================================================================================================================================
namespace InstancingBillboardNS {
	//===================================================================================================================================
	//【インスタンシングビルボード頂点構造体】
	//===================================================================================================================================
	struct Vertex
	{
		D3DXVECTOR2 coord;
		D3DXVECTOR2 uv;
	};

	//===================================================================================================================================
	//【インスタンスクラス】
	//===================================================================================================================================
	class Instance
	{
	public:
		D3DXVECTOR3 position;
		D3DXVECTOR3 rotation;
		D3DXVECTOR3 scale;
		D3DXCOLOR color;
		D3DXVECTOR2 uv;

		float existenceTimer;
		Instance() {
			ZeroMemory(this, sizeof(Instance));
			existenceTimer = 1.0f;
		}
		void update(float frameTime)
		{

		}

	};
	//===================================================================================================================================
	//【インスタンスリストクラス】
	//===================================================================================================================================
	class InstanceList:public LinkedList<InstancingBillboardNS::Instance>
	{
	};
}


//===================================================================================================================================
//【インスタンシングビルボードクラス】
//===================================================================================================================================
class InstancingBillboard : public Base
{
protected:
	LPDIRECT3DDEVICE9 device;
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;
	LPDIRECT3DVERTEXBUFFER9 positionBuffer;
	LPDIRECT3DVERTEXBUFFER9 colorBuffer;
	IDirect3DVertexDeclaration9* declation;
	LPD3DXEFFECT effect;

	D3DXVECTOR3* position;
	D3DXCOLOR* color;
	LPDIRECT3DTEXTURE9 texture;

	InstancingBillboardNS::InstanceList* instanceList;

	bool onRender;

public:
	InstancingBillboard();
	~InstancingBillboard();

	virtual HRESULT initialize(LPD3DXEFFECT effect,LPDIRECT3DTEXTURE9 texture);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void offRender();

	void update(float frameTime);

	//描画するインスタンスを生成
	void generateInstance(InstancingBillboardNS::Instance newInstance);
	//描画するインスタンスを削除
	void deleteInstance(int instanceNo);

	//位置情報を更新する
	void updatePosition();

	//描画数のカラー情報を設定する
	void updateColor();

	//getter
	int getInstanceNum();									//インスタンスの数を取得する
	InstancingBillboardNS::InstanceList getList();	//リストを取得する
};
