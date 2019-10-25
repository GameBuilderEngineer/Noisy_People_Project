//-----------------------------------------------------------------------------
// ナビゲーションメッシュ処理クラス [NavigationMesh.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/5
//-----------------------------------------------------------------------------
#include "NavigationMesh.h"
#include "ImguiManager.h"
#include "Direct3D9.h"


//=============================================================================
// コンストラクタ
//=============================================================================
NavigationMesh::NavigationMesh(StaticMesh* _staticMesh)
{
	staticMesh = _staticMesh;
}


//=============================================================================
// 初期化処理
//=============================================================================
void NavigationMesh::initialize()
{
	meshData.initialize(staticMesh);
	vtxAccessor.initialize(staticMesh);
	aStar.initialize(&meshData, &vtxAccessor);
}


//=============================================================================
// 終了処理
//=============================================================================
void NavigationMesh::uninitialize()
{

}


//=============================================================================
// 更新処理
//=============================================================================
void NavigationMesh::update()
{

}


//=============================================================================
// 経路探索
// [機能] 2地点をつなぐ経路を求める
// [引数] from：現在地座標　dest：目的地座標　flag：現在地/目的地の決定方法
// STANDARD 引数座標に最も近いポリゴン
// GRAVITY  引数座標から重力方向にレイキャストして衝突したポリゴン
//=============================================================================
void NavigationMesh::pathSearch(D3DXVECTOR3 from, D3DXVECTOR3 dest, SEARCH_FLAG flag)
{
	// ワールドマトリクスの逆マトリクスで指示座標をローカル座標に変換する

	// 重力方向
	//D3DXIntersect(targetMesh, &start, &direction, &hit, &index, NULL, NULL, &distance, NULL, NULL);
	// 重力方向は元々持っておく
	
	DWORD startFaceIndex, destFaceIndex;
	
}


#ifdef _DEBUG
//=============================================================================
// デバッグ描画
//=============================================================================
void NavigationMesh::debugRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{

	//renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);

	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// デバイスの取得
	LPDIRECT3DDEVICE9 device = getDevice();

	// ワールドマトリックスの設定
	device->SetTransform(D3DTS_WORLD, &matrixWorld);

	// レンダーステートの設定
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_FILLMODE, objectNS::SOLID);

	// 現在のマテリアルを取得
	device->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*) staticMesh->bufferMaterial->GetBufferPointer();

	// αブレンドを有効化
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// テクスチャのαが低い場合デフォルトのブレンドだとポリゴンが消えるためポリゴンのαだけを採用する
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// ポリゴンとテクスチャの色を混ぜる
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	for (int nCntMat = 0; nCntMat < (int)staticMesh->numMaterial; nCntMat++)
	{
		// マテリアルの設定
		device->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		device->SetTexture(0, *staticMesh->textures);

		// 描画
		staticMesh->mesh->DrawSubset(nCntMat);
	}

	// αブレンドを無効化
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// マテリアルをデフォルトに戻す
	device->SetMaterial(&matDef);
}


//=============================================================================
// ImGUIへの出力
//=============================================================================
void NavigationMesh::outputGUI()
{
	//ImGui::Text(sceneName.c_str());

	if (ImGui::CollapsingHeader("NaviAIInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;

		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));


		ImGui::Checkbox("onGravity", &onGravity);										//重力有効化フラグ
		ImGui::Checkbox("onActive", &onActive);											//アクティブ化フラグ
	}
}
#endif // _DEBUG


// Astarではエッジの情報を検索して、そのエッジのどこを目的地とするか、は別処理（AI資料に書いてある）