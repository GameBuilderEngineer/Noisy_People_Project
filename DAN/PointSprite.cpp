//===================================================================================================================================
//【PointSprite.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "PointSprite.h"
#include "Direct3D9.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace pointSpriteNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
PointSprite::PointSprite()
{
	pTexture = NULL;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
PointSprite::~PointSprite()
{
	SAFE_RELEASE(pTexture);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT PointSprite::initilaize()
{
	setTextureDirectory();
	//ポイントスプライトに貼るテクスチャオブジェクトを作成
	if (FAILED(D3DXCreateTextureFromFileEx(getDevice(), "Game_BackGround_Star.jpg", (UINT)WIDTH, (UINT)HEIGHT, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000000, NULL, NULL, &pTexture)))
	{
		MessageBox(0, "テクスチャの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < POINT_NUM; i++)
	{
		vertex[i].coord = D3DXVECTOR3((float)((rand() % 1000) - 500), (float)((rand() % 1000) - 500), (float)((rand() % 1000) - 500));
		vertex[i].color = D3DCOLOR_ARGB(255, 255, 255, 255);
		vertex[i].scale = 0.001f;
		size[i] = (float)((rand() % 200) / 1000);
	}

	return S_OK;
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void PointSprite::render(D3DXVECTOR3 cameraPositon)
{
	for (int i = 0; i < POINT_NUM; i++)
	{
		float rate = 
			between2VectorLength(cameraPositon, vertex[i].coord) /
			between2VectorLength(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f));
		float scale = max(size[i] - size[i]*rate, 0.001f);
		vertex[i].scale = scale;

		size[i] += 0.001;
		if (size[i] > 0.2)size[i] = 0.001f;
	}

	//次のレンダーステートにより、"単なる色付きのポイント"それぞれが1頂点のみでスプライトになってくれる
	getDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	getDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	//getDevice()->SetRenderState(D3DRS_POINTSIZE, GET_ADDRESS(fScale[i]));//なお、頂点にスケール情報（FVF_SCALE）を持たせている場合は、この指定は無視されます。（本サンプルでは有効です）
	//見栄えを良くする為にアルファブレンディングを設定する
	getDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	getDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	getDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//ライティング計算はしない。深度バッファには書き込まない
	getDevice()->SetRenderState(D3DRS_LIGHTING, false);
	getDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	//ポイントスプライト用のテクスチャをセット
	getDevice()->SetTexture(0, pTexture);
	//頂点の型をセット
	getDevice()->SetFVF(D3DFVF_XYZ| D3DFVF_PSIZE | D3DFVF_DIFFUSE);
	//レンダリング　今回は2つのポイントスプライトのみ
	getDevice()->DrawPrimitiveUP(D3DPT_POINTLIST, POINT_NUM, &vertex, sizeof(COLOR_VERTEX));
	
	//ライティング計算はしない。　深度バッファには書き込まない
	getDevice()->SetRenderState(D3DRS_LIGHTING, true);
	getDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//次のレンダーステートにより、"単なる色付きのポイント"それぞれが1頂点のみでスプライトになってくれる
	getDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	getDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, false);

}