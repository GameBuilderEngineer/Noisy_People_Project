//===================================================================================================================================
//【InstancingBillboard.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/08/07
// [更新日]2019/09/06
//===================================================================================================================================
#pragma once
#include "Base.h"

struct InstancingBillboardVertex
{
	D3DXVECTOR2 coord;
	D3DXVECTOR2 uv;
};

class InstancingBillboard : public Base
{
protected:
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;
	LPDIRECT3DVERTEXBUFFER9 positionBuffer;
	LPDIRECT3DVERTEXBUFFER9 colorBuffer;
	IDirect3DVertexDeclaration9* declation;
	LPD3DXEFFECT effect;
	D3DXVECTOR3* position;
	D3DXCOLOR* color;
	LPDIRECT3DTEXTURE9 texture;
	int renderNum;
	bool onRender;


public:
	InstancingBillboard();
	~InstancingBillboard();

	virtual HRESULT initialize(LPDIRECT3DDEVICE9 device,LPD3DXEFFECT effect,LPDIRECT3DTEXTURE9 texture);
	virtual void render(LPDIRECT3DDEVICE9 device, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void offRender();

	//球状にランダム配置表示する
	void createPositionSpherical(LPDIRECT3DDEVICE9 device, int num, float radius);

	//描画数をセットする
	void setNumOfRender(LPDIRECT3DDEVICE9 device, int num, D3DXVECTOR3* positionList);

	//描画数のカラー情報を設定する
	void setColorBuffer(LPDIRECT3DDEVICE9 device, int num, D3DXCOLOR* colorList);
};