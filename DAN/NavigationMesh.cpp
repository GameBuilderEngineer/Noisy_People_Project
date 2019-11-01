//-----------------------------------------------------------------------------
// ナビゲーションメッシュ処理クラス [NavigationMesh.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/5
//-----------------------------------------------------------------------------
#include "NavigationMesh.h"
#include "ImguiManager.h"
#include "Direct3D9.h"
using namespace meshDataNS;
using namespace navigationMeshNS;

NavigationMesh* NavigationMesh::pointer = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
NavigationMesh::NavigationMesh(StaticMesh* _staticMesh)
{
	pointer = this;
	staticMesh = _staticMesh;
}

//=============================================================================
// 初期化処理
//=============================================================================

void NavigationMesh::initialize()
{
	vtxAccessor.initialize(staticMesh);
	meshData.initialize(staticMesh, &vtxAccessor);
	aStar.initialize(&meshData, &vtxAccessor);
	pathFollowing.initialize(&meshData, &vtxAccessor);	
	gravityDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	// 重力方向を設定

#ifdef _DEBUG
	position = D3DXVECTOR3(0.0f, FLOATING_HEIGHT, 0.0f);// 少し浮かせる
	renderer = new StaticMeshRenderer(staticMesh);		// 描画オブジェクトを作成
	renderer->registerObject(this);						// 描画オブジェクトに登録する
	changeColor();										// メッシュの色を変える
#endif //_DEBUG
}


//=============================================================================
// 終了処理
//=============================================================================
void NavigationMesh::uninitialize()
{
	// edgeListはNavigationMeshクラスではなく
	// pathSearch()を呼んだクラスが破棄を行う

#ifdef _DEBUG
	renderer->allUnRegister();
	SAFE_DELETE(renderer);
#endif //_DEBUG
}


//=============================================================================
// 更新処理
//=============================================================================
void NavigationMesh::update()
{

}


//=============================================================================
// 経路探索
// [引数1] D3DXVECTOR3 from　現在地座標
// [引数2] D3DXVECTOR3 dest　目的地座標
//=============================================================================
NAVIRESULT NavigationMesh::pathSearch(LinkedList<meshDataNS::Index2>** pOut,
	DWORD* faceIndex, D3DXVECTOR3 _from, D3DXVECTOR3 _dest)
{
	// 経路情報を更新
	from = _from;
	dest = _dest;
	startIndex = -1;
	destIndex = -1;
	edgeList = NULL;

	// レイの準備（逆マトリクスでメッシュの位置・回転をレイに適用）
	D3DXMATRIX Inv;
	D3DXVECTOR3 rayPos;
	BOOL hit;
	D3DXMatrixInverse(&Inv, NULL, &matrixWorld);

	// 現在地がナビメッシュに接地するかチェック
	rayPos = _from;
	D3DXVec3TransformCoord(&rayPos, &rayPos, &Inv);
	D3DXIntersect(staticMesh->mesh, &rayPos, &gravityDirection,
		&hit, &startIndex, NULL, NULL, NULL, NULL, NULL);
	*faceIndex = startIndex;								// 呼び出し側のインデックスを更新
	if (hit == false) { return CURRENT_NOT_ON_MESH; }		// 現在地がナビメッシュ上ではない

	// 目的地がナビメッシュに接地するかチェック
	rayPos = _dest;
	D3DXVec3TransformCoord(&rayPos, &rayPos, &Inv);
	D3DXIntersect(staticMesh->mesh, &rayPos, &gravityDirection,
		&hit, &destIndex, NULL, NULL, NULL, NULL, NULL);
	if (hit == false) { return DESTINATION_NOT_ON_MESH; }	// 目的地がナビメッシュ上ではない

	// A*アルゴリズムによる経路探索でエッジリストを取得する
	edgeList = aStar.pathSearch(startIndex, destIndex, from, dest);
	if (edgeList == NULL) { return IMPASSE; }				// 袋小路のためパス検索失敗

	// エッジリストを更新し返却する
	edgeList->listUpdate();
	*pOut = edgeList;

	return NAVI_OK;
}


//=============================================================================
// ステアリング（移動ベクトルの作成）
//=============================================================================
NAVIRESULT NavigationMesh::steering(D3DXVECTOR3* out, DWORD* faceIndex, D3DXVECTOR3 _position,
	LinkedList<meshDataNS::Index2>* _edgeList)
{
	// レイの準備（逆マトリクスでメッシュの位置・回転をレイに適用）
	D3DXMATRIX Inv;
	D3DXVECTOR3 rayPos;
	BOOL hit;
	DWORD currentIndex;
	float distance;
	D3DXMatrixInverse(&Inv, NULL, &matrixWorld);

	// レイでナビメッシュに接地するかチェック
	rayPos = _position;
	D3DXVec3TransformCoord(&rayPos, &rayPos, &Inv);
	D3DXIntersect(staticMesh->mesh, &rayPos, &gravityDirection,
		&hit, &currentIndex, NULL, NULL, &distance, NULL, NULL);
	if (hit == false) { return CURRENT_NOT_ON_MESH; }	// 現在地がナビメッシュ上ではない

	// ポリゴン面インデックスが変わればエッジをリストからノードを削除後に更新
	if (currentIndex != *faceIndex)
	{
		*faceIndex = currentIndex;
		_edgeList->removeFront();
		_edgeList->listUpdate();
	}

	// ポリゴン面インデックスがおかしくなったら再度経路探索して再実行
	// 落下の押し戻し
	// A*の経路確認
	  
	// 接地座標をもとにパスフォローイングを実行
	D3DXVECTOR3 surfaceIntersection = _position + gravityDirection * distance;
	pathFollowing.createVector(out, surfaceIntersection, faceIndex, edgeList);
	return NAVI_OK;
}


#ifdef _DEBUG
//=============================================================================
// エッジリストをファイルにダンプ
//=============================================================================
void NavigationMesh::dumpEdgeList()
{
	FILE* fp;
	setDataDirectory();

	fp = fopen("EdgeList.txt", "w");
	fprintf(fp, "エッジリストの中身\n出力元：NavigationMesh.cpp\n");

	edgeList->listUpdate();
	for (int i = 0; i < edgeList->nodeNum; i++)
	{
		Index2 vtx = *edgeList->getValue(i);
		fprintf(fp, "%d, %d\n", vtx.index[0], vtx.index[1]);
	}
	fclose(fp);
}


//=============================================================================
// デバッグ描画
//=============================================================================
void NavigationMesh::debugRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#if 1	// メッシュを描画する
	debugRenderMesh(view, projection, cameraPosition);
#endif
}


//=============================================================================
// メッシュを描画
//=============================================================================
void NavigationMesh::debugRenderMesh(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#if 0	// StaticMeshRendererによる描画
	renderer->update();
	renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
#endif

#if 1	// DrowSubsetによる描画
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// デバイスの取得
	LPDIRECT3DDEVICE9 device = getDevice();

	// ワールドマトリックスの設定
	D3DXMatrixTranslation(&matrixWorld, position.x, position.y, position.z);
	device->SetTransform(D3DTS_WORLD, &matrixWorld);

	// レンダーステートの設定
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_FILLMODE, objectNS::SOLID);
	//device->SetRenderState(D3DRS_FILLMODE, objectNS::WIREFRAME);
	//device->SetRenderState(D3DRS_FILLMODE, objectNS::POINT);

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
#endif
}


//=============================================================================
// エッジを描画
//=============================================================================
void NavigationMesh::debugRenderEdge(LinkedList<meshDataNS::Index2>* _edgeList)
{
	if (_edgeList == NULL) return;
	_edgeList->listUpdate();

	// レイの配列を作成
	Ray* line = new Ray[_edgeList->nodeNum];

	for (int i = 0; i < _edgeList->nodeNum; i++)
	{
		// インデックスの頂点座標を取得
		BYTE* p1 = meshData.getVertexPointerFromVertexIndex(edgeList->getValue(i)->index[0]);
		BYTE* p2 = meshData.getVertexPointerFromVertexIndex(edgeList->getValue(i)->index[1]);
		D3DXVECTOR3 tempPos1 = *(D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p1);
		D3DXVECTOR3 tempPos2 = *(D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p2);
		tempPos1 += D3DXVECTOR3(0.0f, FLOATING_HEIGHT + 0.1f, 0.0f);	
		tempPos2 += D3DXVECTOR3(0.0f, FLOATING_HEIGHT + 0.1f, 0.0f);
		// レイにパラメータをセット
		line[i].start = tempPos1;
		line[i].direction = tempPos2 - tempPos1;
		D3DXVec3Normalize(&line[i].direction, &line[i].direction);
		float len = D3DXVec3Length(&(tempPos2 - tempPos1));
		line[i].color = D3DXCOLOR(0, 0, 255, 255);
		// 描画
		line[i].render(len);
	}

	// レイの配列を破棄
	SAFE_DELETE_ARRAY(line);
}


//=============================================================================
// メッシュの色を変える
//=============================================================================
void NavigationMesh::changeColor()
{
	D3DCOLOR setting = D3DCOLOR_RGBA(255, 255, 255, 155);

	// 頂点バッファの準備
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;
	staticMesh->mesh->GetVertexBuffer(&vertexBuffer);
	BYTE* pVtx;

	// 頂点カラーを書き換える
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		for (int i = 0; i < meshData.getNumVertices(); i++)
		{
			D3DCOLOR* pColor = (D3DCOLOR*)vtxAccessor.getPointer(vtxAccess::COLOR, &pVtx[i * meshData.getStride()]);
			*pColor = setting;
		}
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "頂点バッファのロックに失敗しました", NULL, MB_OK);
		return;
	}
}


//=============================================================================
// エッジの頂点に影響を与える
//=============================================================================
void NavigationMesh::affectToEdgeVertex()
{
	if (edgeList == NULL) return;
	edgeList->listUpdate();
	D3DCOLOR red = D3DCOLOR_RGBA(255, 0, 0, 255);

	// 頂点バッファの準備
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;
	staticMesh->mesh->GetVertexBuffer(&vertexBuffer);
	BYTE* pVtx;

	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		for (int i = 0; i < edgeList->nodeNum; i++)
		{
			// インデックスから頂点を取得
			WORD index1 = edgeList->getValue(i)->index[0];
			WORD index2 = edgeList->getValue(i)->index[1];
			BYTE* p1 = meshData.getVertexPointerFromVertexIndex(index1, pVtx);
			BYTE* p2 = meshData.getVertexPointerFromVertexIndex(index2, pVtx);

			// 頂点カラーを書き換える
			D3DCOLOR* pColor1 = (D3DCOLOR*)vtxAccessor.getPointer(vtxAccess::COLOR, p1);
			D3DCOLOR* pColor2 = (D3DCOLOR*)vtxAccessor.getPointer(vtxAccess::COLOR, p2);
			*pColor1 = *pColor2 = red;

			//// 頂点がボコボコ隆起
			//D3DXVECTOR3* pPos1= (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p1);
			//D3DXVECTOR3* pPos2 = (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p2);
			//*pPos1 += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//*pPos2 += D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//// UV座標が壊れる
			//D3DXVECTOR2* pUv1 = (D3DXVECTOR2*)vtxAccessor.getPointer(vtxAccess::UV, p1);
			//D3DXVECTOR2* pUv2 = (D3DXVECTOR2*)vtxAccessor.getPointer(vtxAccess::UV, p2);
			//*pUv1 *= 0;
			//*pUv2 *= 0;

		}
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "頂点バッファのロックに失敗しました", NULL, MB_OK);
		return;
	}
}


//=============================================================================
// ImGUIへの出力
//=============================================================================
void NavigationMesh::outputGUI()
{
	if (ImGui::CollapsingHeader("NaviAIInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("from(%.3f, %.3f, %.3f)\n", from.x, from.y, from.z);
		ImGui::Text("from-->faceIndex:%d\n", startIndex);
		ImGui::Text("dest(%.3f, %.3f, %.3f)\n", dest.x, dest.y, dest.z);
		ImGui::Text("dest-->faceIndex:%d\n", destIndex);
		if(edgeList) ImGui::Text("edgeList->nodeNum:%d\n", edgeList->nodeNum);
	}
}
#endif // _DEBUG
