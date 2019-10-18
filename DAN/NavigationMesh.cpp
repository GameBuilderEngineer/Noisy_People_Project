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
NavigationMesh::NavigationMesh(StaticMesh* staticMesh)//:StaticMeshObject(staticMesh)
{
	mesh = staticMesh->mesh;	// 参照の長さを縮めるためにポインタをコピー
}


//=============================================================================
// 初期化処理
//=============================================================================
void NavigationMesh::initialize()
{
	position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	StaticMeshObject::initialize(&position);

	numVertices = mesh->GetNumVertices();
	stride = mesh->GetNumBytesPerVertex();
	numFaces = mesh->GetNumFaces();
	//vtxAccessor.initialize(staticMesh);※菅野コメントアウト

	//// 頂点バッファを配列にコピーする
	//vtx = new BYTE[numVertices * stride];
	//BYTE* pVtx;
	//mesh->GetVertexBuffer(&vertexBuffer);
	//if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	//{
	//	memcpy(vtx, pVtx, numVertices * stride);
	//	vertexBuffer->Unlock();
	//}
	//else
	//{
	//	MessageBox(0, "頂点バッファのロックに失敗しました", NULL, MB_OK);
	//	return;
	//}

	//// インデックスバッファを配列にコピーする
	//index = new WORD[numFaces * 3];
	//WORD* pIndex;
	//mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);
	//memcpy(index, pIndex, sizeof(WORD) * numFaces * 3);
	//mesh->UnlockIndexBuffer();

	//// 隣接性情報を配列にコピー
	//adjacency = new DWORD[numFaces * 3];
	//mesh->GenerateAdjacency(0, adjacency);


	D3DCOLOR white = D3DCOLOR_RGBA(255, 255, 255, 155);
	D3DCOLOR red = D3DCOLOR_RGBA(255, 0, 0, 155);

	BYTE* pVtx;
	mesh->GetVertexBuffer(&vertexBuffer);
 	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		bool isRed = false;
		for (int i = 0; i < numVertices; i++)
		{
			D3DCOLOR* pColor = (D3DCOLOR*)vtxAccessor.getPointer(vtxAccess::COLOR, &pVtx[i * stride]);
			//if (i % 3 == 0)
			//{
			//	//if ((i * 3) % (5 * 3) == 0)
			//	//{
			//	//	isRed = true;
			//	//}
			//	//else
			//	//{
			//	//	isRed = false;
			//	//}

			//	isRed = isRed ? false : true;
			//}

			if (i == 5 || i == 14 || i == 15)
			{
				*pColor = red;
			}
			else
			{
				*pColor = white;
			}


			//D3DXVECTOR3* pos = (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, &pVtx[i * stride]);
			//*pos = *pos +  D3DXVECTOR3(rand() % 3, rand() % 3, rand() % 3);
		}
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "頂点バッファのロックに失敗しました", NULL, MB_OK);
		return;
	}



#ifdef _DEBUG
	////------------------
	//// ファイルにダンプ
	////------------------
	//FILE* fp;
	//setDataDirectory();

	//// 頂点情報
	//fp = fopen("VertexBufferOfNavMesh.txt", "w");
	//fprintf(fp, "頂点バッファ配列の中身\n出力元：NavigationMesh.cpp\n");
	//for (int i = 0; i < numVertices; i++)
	//{
	//	D3DXVECTOR3* test = (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, &vtx[i * stride]);
	//	fprintf(fp, "%f, %f, %f \n", test->x, test->y, test->z);
	//}
	//fclose(fp);


	//// 頂点宣言
	//fp = fopen("DeclarationOfNavMesh.txt", "w");
	//fprintf(fp, "頂点宣言構造体の中身\n出力元：NavigationMesh.cpp\n");
	//for (int i = 0; i < 65; i++)
	//{
	//	fprintf(fp, "\n[%d]",i);
	//	fprintf(fp, "Type : %d\n", staticMesh->vertexElement[i].Type);
	//	fprintf(fp, "Usage : %d\n", staticMesh->vertexElement[i].Usage);
	//}
	//fclose(fp);

	//// インデックス情報
	//fp = fopen("IndexBufferOfNavMesh.txt", "w");
	//fprintf(fp, "インデックスバッファ配列の中身\n出力元：NavigationMesh.cpp\n");
	//fprintf(fp, "ポリゴンインデックス : ポリゴンを構成する頂点インデックス1, 2, 3\n\n");
	//for (int i = 0; i < numFaces * 3; i++)
	//{
	//	fprintf(fp, "%d,", index[i]);
	//	if(i % 3 == 2) fprintf(fp, "\n");
	//}
	//fclose(fp);

	//// 隣接性情報
	//fp = fopen("AdjacencyOfNavMesh.txt", "w");
	//fprintf(fp, "隣接性情報配列の中身\n出力元：NavigationMesh.cpp\n");
	//fprintf(fp, "ポリゴンインデックス : 隣接するポリゴンインデックス1, 2, 3\n\n");
	//for (int i = 0, j = 0; i < numFaces * 3; i++)
	//{
	//	if (i % 3 == 0) fprintf(fp, "%4d : ", j++);
	//	fprintf(fp, "%d,", adjacency[i]);
	//	if (i % 3 == 2) fprintf(fp, "\n");
	//}
	//fclose(fp);
#endif
}


//=============================================================================
// 終了処理
//=============================================================================
void NavigationMesh::uninitialize()
{
	SAFE_DELETE_ARRAY(vtx);
	SAFE_DELETE_ARRAY(index);
	SAFE_DELETE_ARRAY(adjacency);
}


//=============================================================================
// 更新処理
//=============================================================================
void NavigationMesh::update()
{













	// 自分のいるポリゴンのインデックスを取得

	// 開始ポリゴンに設定

	// 目的地ポリゴンを設定

	// 基準ノードに開始ポリゴンを設定する

	// オープンリストに開始ノードを追加する

	// A*----------------------------
	// ******** 基準ノードが目的地のノードである場合
	// 経路完成
	// ******** 基準ノードが目的地のノードでない場合
	// 基準ノードの周りをオープンしていく
	//（隣接性データをもとに）

	// 経路探索後、ポリゴン上での挙動も必要
}


//=============================================================================
// 経路探索
// [機能] 2地点をつなぐ経路を求める
// [引数] from：現在地座標　dest：目的地座標　flag：現在地/目的地の決定方法
// STANDARD 引数座標に最も近いポリゴン
// GRAVITY  引数座標から重力方向にレイキャストして衝突したポリゴン
//=============================================================================
void NavigationMesh::searchRoute(D3DXVECTOR3 from, D3DXVECTOR3 dest, SEARCH_FLAG flag)
{
	// ワールドマトリクスの逆マトリクスで指示座標をローカル座標に変換する

	// 重力方向
}


//
//BYTE* NavigationMesh::getVertexFromIndex(BYTE* pVtx, WORD index)
//{
//	return &pVtx[index];
//}


#ifdef _DEBUG
//=============================================================================
// デバッグ描画
//=============================================================================
void NavigationMesh::debugRender()
{
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// デバイスの取得
	LPDIRECT3DDEVICE9 device = getDevice();

	// ワールドマトリックスの設定
	device->SetTransform(D3DTS_WORLD, &matrixWorld);

	// レンダーステートの設定
	device->SetRenderState(D3DRS_LIGHTING, onLighting);
	device->SetRenderState(D3DRS_FILLMODE, fillMode);

	// 現在のマテリアルを取得
	device->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)staticMesh->bufferMaterial->GetBufferPointer();

	// テクスチャのαが低い場合デフォルトのブレンドだとポリゴンが消えるためポリゴンのαだけを採用する
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);// 色を掛け算してください

	for (int nCntMat = 0; nCntMat < (int)staticMesh->numMaterial; nCntMat++)
	{
		// マテリアルの設定
		device->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		device->SetTexture(0, *staticMesh->textures);

		// 描画
		staticMesh->mesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	device->SetMaterial(&matDef);}


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