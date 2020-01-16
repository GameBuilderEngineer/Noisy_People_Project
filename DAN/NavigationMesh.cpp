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
	Object::initialize(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
// デストラクタ
//=============================================================================
NavigationMesh::~NavigationMesh()
{
	uninitialize();
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
	//position = D3DXVECTOR3(0.0f, FLOATING_HEIGHT, 0.0f);	// 少し浮かせる
	//renderer = new StaticMeshRenderer(staticMesh);		// 描画オブジェクトを作成
	//renderer->registerObject(this);						// 描画オブジェクトに登録する
	//changeColor();										// メッシュの色を変える
#endif //_DEBUG
}


//=============================================================================
// 終了処理
//=============================================================================
void NavigationMesh::uninitialize()
{
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
	Object::update();
}


//=============================================================================
// 経路探索
// [引数1] LinkedList<meshDataNS::Index2>** edgeList　エッジリストダブルポインタ
// ※エッジリストにここで加える変更内容を呼び出し元に反映するため
// [引数2] DWORD* faceIndex　面インデックス
// [引数3] D3DXVECTOR3 from　現在地座標
// [引数4] D3DXVECTOR3 dest　目的地座標
//=============================================================================
NAVIRESULT NavigationMesh::pathSearch(LinkedList<meshDataNS::Index2>** edgeList,
	DWORD* faceIndex, D3DXVECTOR3 from, D3DXVECTOR3 dest)
{
	DWORD startFaceIndex = -1;			// Aスター開始面インデックス
	DWORD destinationFaceIndex = -1;	// Aスター目的地面インデックス

	// 現在地がナビメッシュに接地するかチェック
	if (isHitGrounding(NULL, &startFaceIndex, from))
	{
		*faceIndex = startFaceIndex;	// 呼び出し側のインデックスを更新
	}
	else
	{
		return CURRENT_NOT_ON_MESH;		// 現在地がナビメッシュ上ではない
	}

	// 目的地がナビメッシュに接地するかチェック
	if (isHitGrounding(NULL, &destinationFaceIndex, dest) == false)
	{
		return DESTINATION_NOT_ON_MESH;	// 目的地がナビメッシュ上ではない
	}

	// A*アルゴリズムによる経路探索でエッジリストを取得する
	LinkedList<meshDataNS::Index2>* temp = NULL;
	temp = aStar.pathSearch(startFaceIndex, destinationFaceIndex, from, dest);
	if (temp == NULL)
	{
		return IMPASSE;					// 袋小路のためパス検索失敗
	}

	// NULLで引数に渡されていなけば古いエッジリストを破棄
	if(*edgeList != NULL)
	{
		(*edgeList)->terminate();
		SAFE_DELETE(*edgeList);
	}

	// エッジリストを更新（コピー）し返却する
	*edgeList = temp;
	(*edgeList)->listUpdate();
	cntEdge = 0;
	return NAVI_OK;
}


//=============================================================================
// ステアリング（移動ベクトルの作成）
//=============================================================================
NAVIRESULT NavigationMesh::steering(D3DXVECTOR3* out, DWORD* faceIndex, D3DXVECTOR3 from, D3DXVECTOR3 dest,
	LinkedList<meshDataNS::Index2>** edgeList)
{
	//-------------------------------------------------------------------
	// エッジが0個(= 同一面に目的地がある)
	// エッジが1個(= 次の面にある目的地までの直線は、他の面をまたがない)
	// 以上のケースでは目的地までの直線ベクトルで移動すればよい
	// よって面に平行にスリップした直線正規化ベクトルを返却する
	//-------------------------------------------------------------------
	//if (*edgeList != NULL && (*edgeList)->nodeNum < 2)
	//{
	//	(*edgeList)->terminate();
	//	SAFE_DELETE(*edgeList);
	//}
	//if (*edgeList == NULL)
	//{
		D3DXVECTOR3 straightDirection = dest - from;
		slip(straightDirection, meshData.getFaceArray()->nor);
		D3DXVec3Normalize(&straightDirection, &straightDirection);
		*out = straightDirection;
		return NAVI_OK;
	//}

	//// 現在地の面インデックスと面までの距離を取得する
	//DWORD currentIndex;
	//float distance;
	//if (isHitGrounding(&distance, &currentIndex, from) == false)
	//{
	//	return CURRENT_NOT_ON_MESH;			// 現在地がナビメッシュ上ではない
	//}

	//// ポリゴン面インデックスが変わればエッジをリストからノードを削除後に更新
	//if (currentIndex != *faceIndex)
	//{
	//	*faceIndex = currentIndex;
	//	//cntEdge++;
	//	//if (cntEdge >= 2)
	//	{
	//		//(*edgeList)->removeFront();
	//		(*edgeList)->removeFront();
	//		(*edgeList)->listUpdate();
	//	}

	//	if (*edgeList != NULL && (*edgeList)->nodeNum < 2) return NAVI_OK;	// ●
	//}

	// ポリゴン面インデックスがおかしくなったら再度経路探索して再実行
	// 落下の押し戻し
	// A*の経路確認

	//// 接地座標をもとにパスフォローイングを実行
	//D3DXVECTOR3 surfaceIntersection = from + gravityDirection * distance;
	//pathFollowing.createVector(out, surfaceIntersection, faceIndex, *edgeList);

	//return NAVI_OK;


	////--------------------------------------------------------------------
	//// エッジリストが空(= 同一面に目的地がある)場合に目的地に直線移動する
	//// 面に平行にスリップした正規化ベクトルを返却する
	////--------------------------------------------------------------------
	//if (*edgeList != NULL && (*edgeList)->isEnpty())
	//{
	//	(*edgeList)->terminate();
	//	SAFE_DELETE(*edgeList);
	//}
	//if (*edgeList == NULL)
	//{
	//	D3DXVECTOR3 straightDirection = dest - from;
	//	slip(straightDirection, meshData.getFaceArray()->nor);
	//	D3DXVec3Normalize(&straightDirection, &straightDirection);
	//	*out = straightDirection;
	//	return NAVI_OK;
	//}

	//// 現在地の面インデックスと面までの距離を取得する
	//DWORD currentIndex;
	//float distance;
	//if (isHitGrounding(&distance, &currentIndex, from) == false)
	//{
	//	return CURRENT_NOT_ON_MESH;			// 現在地がナビメッシュ上ではない
	//}

	//// ポリゴン面インデックスが変わればエッジをリストからノードを削除後に更新
	//if (currentIndex != *faceIndex)
	//{
	//	*faceIndex = currentIndex;
	//	(*edgeList)->removeFront();
	//	(*edgeList)->listUpdate();
	//}

	//// ポリゴン面インデックスがおかしくなったら再度経路探索して再実行
	//// 落下の押し戻し
	//// A*の経路確認
	//  
	//// 接地座標をもとにパスフォローイングを実行
	//D3DXVECTOR3 surfaceIntersection = from + gravityDirection * distance;
	//pathFollowing.createVector(out, surfaceIntersection, faceIndex, *edgeList);
	//

	//return NAVI_OK;
}


//=============================================================================
// 接地チェック
//=============================================================================
bool NavigationMesh::isHitGrounding(float *distance, DWORD* faceIndex, D3DXVECTOR3 _position)
{
	D3DXMATRIX Inv;
	D3DXMatrixInverse(&Inv, NULL, &matrixWorld);
	D3DXVECTOR3 rayPos = _position;
	BOOL hit;

	// レイでナビメッシュに接地するかチェック
	rayPos = _position;
	D3DXVec3TransformCoord(&rayPos, &rayPos, &Inv);
	D3DXIntersect(staticMesh->mesh, &rayPos, &gravityDirection,
		&hit, faceIndex, NULL, NULL, distance, NULL, NULL);

	return hit;
}


#ifdef _DEBUG
//=============================================================================
// エッジリストをファイルにダンプ
//=============================================================================
void NavigationMesh::dumpEdgeList(LinkedList<meshDataNS::Index2>* _edgeList)
{
	FILE* fp;
	setDataDirectory();

	fp = fopen("EdgeList.txt", "w");
	fprintf(fp, "エッジリストの中身\n出力元：NavigationMesh.cpp\n");

	_edgeList->listUpdate();
	for (int i = 0; i < _edgeList->nodeNum; i++)
	{
		Index2 vtx = *_edgeList->getValue(i);
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

#if 0	// エッジ関係を描画する
	if (debugEdgeList != NULL)
	{
		affectToEdgeVertex(debugEdgeList);
		debugRenderEdge(debugEdgeList);
	}
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
void NavigationMesh::debugRenderEdge(LinkedList<meshDataNS::Index2>** edgeList)
{
	if ((*edgeList) == NULL) return;

	(*edgeList)->listUpdate();

	// レイの配列を作成
	Ray* line = new Ray[(*edgeList)->nodeNum];

	for (int i = 0; i < (*edgeList)->nodeNum; i++)
	{
		// インデックスの頂点座標を取得
		BYTE* p1 = meshData.getVertexPointerFromVertexIndex((*edgeList)->getValue(i)->index[0]);
		BYTE* p2 = meshData.getVertexPointerFromVertexIndex((*edgeList)->getValue(i)->index[1]);
		D3DXVECTOR3 tempPos1 = *(D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p1);
		D3DXVECTOR3 tempPos2 = *(D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p2);
		tempPos1 += D3DXVECTOR3(0.0f, FLOATING_HEIGHT + 0.1f, 0.0f);	
		tempPos2 += D3DXVECTOR3(0.0f, FLOATING_HEIGHT + 0.1f, 0.0f);
		// レイにパラメータをセット
		line[i].start = tempPos1;
		line[i].direction = tempPos2 - tempPos1;
		D3DXVec3Normalize(&line[i].direction, &line[i].direction);
		float len = D3DXVec3Length(&(tempPos2 - tempPos1));
		if (i % 2 == 1)
		{
			line[i].color = D3DXCOLOR(0, 0, 255, 255);
		}
		else
		{
			line[i].color = D3DXCOLOR(255, 255, 255, 255);
		}
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
	D3DCOLOR setting = D3DCOLOR_RGBA(255, 255, 255, 255);

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
void NavigationMesh::affectToEdgeVertex(LinkedList<meshDataNS::Index2>** edgeList)
{
	if ((*edgeList) == NULL) return;
	(*edgeList)->listUpdate();

	D3DCOLOR red = D3DCOLOR_RGBA(255, 0, 0, 255);

	// 頂点バッファの準備
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;
	staticMesh->mesh->GetVertexBuffer(&vertexBuffer);
	BYTE* pVtx;

	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		for (int i = 0; i < (*edgeList)->nodeNum; i++)
		{
			// インデックスから頂点を取得
			WORD index1 = (*edgeList)->getValue(i)->index[0];
			WORD index2 = (*edgeList)->getValue(i)->index[1];
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
	}
}
#endif // _DEBUG
