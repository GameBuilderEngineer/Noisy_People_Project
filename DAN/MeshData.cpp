//-----------------------------------------------------------------------------
// メッシュデータクラス [MeshData.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/20
//-----------------------------------------------------------------------------
#include "MeshData.h"
using namespace meshDataNS;

//=============================================================================
// 初期化
//=============================================================================
void MeshData::initialize(StaticMesh* _staticMesh, VertexAccessor* _vtxAccessor)
{
	staticMesh = _staticMesh;
	vtxAccessor = _vtxAccessor;
	numVertices = staticMesh->mesh->GetNumVertices();
	stride = staticMesh->mesh->GetNumBytesPerVertex();
	numFaces = staticMesh->mesh->GetNumFaces();
	staticMesh->mesh->GetFVF();

	// 配列を確保
	vtxArray = new BYTE[numVertices * stride];
	indexArray = new WORD[numFaces * 3];
	adjacencyArray = new DWORD[numFaces * 3];
	faceArray = new Face[getNumFaces()];

	// 頂点バッファを配列にコピーする
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;
	BYTE* pVtx;
	staticMesh->mesh->GetVertexBuffer(&vertexBuffer);
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		memcpy(vtxArray, pVtx, numVertices * stride);
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "頂点バッファのロックに失敗しました", NULL, MB_OK);
		return;
	}

	// インデックスバッファを配列にコピーする
	WORD* pIndex;
	staticMesh->mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);
	memcpy(indexArray, pIndex, sizeof(WORD) * numFaces * 3);
	staticMesh->mesh->UnlockIndexBuffer();

	// 隣接性情報を配列にコピーする
	staticMesh->mesh->GenerateAdjacency(0, adjacencyArray);

	// ポリゴン面配列の情報を埋める
	for (int i = 0; i < getNumFaces(); i++)
	{
		for (int vertexCnt = 0; vertexCnt < 3; vertexCnt++)
		{
			// 頂点データをセット
			faceArray[i].vtx[vertexCnt].data = getVertexPointerFromVertexIndex(faceArray[i].vtx[vertexCnt].index);
			// 頂点インデックスをセット
			faceArray[i].vtx[vertexCnt].index = getVertexIndexFromFaceIndex(i, vertexCnt);
			// 頂点座標をセット
			D3DXVECTOR3* position = (D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
				getVertexPointerFromVertexIndex(faceArray[i].vtx[vertexCnt].index));
			faceArray[i].vtx[vertexCnt].position = *position;
		}
		for (int faceCnt = 0; faceCnt < 3; faceCnt++)
		{
			// 隣接性データをセット
			faceArray[i].adjacency[faceCnt] = getAdjacentFaceIndexFromFaceIndex(i, faceCnt);
		}
		// 法線をセット
		D3DXVECTOR3 vec0to1 = faceArray[i].vtx[1].position - faceArray[i].vtx[0].position;
		D3DXVECTOR3 vec0to2 = faceArray[i].vtx[2].position - faceArray[i].vtx[0].position;
		D3DXVec3Cross(&faceArray[i].nor, &vec0to1, &vec0to2);
		D3DXVec3Normalize(&faceArray[i].nor, &faceArray[i].nor);
	}

#if 0	// 頂点バッファにアクセスしてメッシュを操作するテスト
	D3DCOLOR red = D3DCOLOR_RGBA(255, 120, 0, 100);

	//BYTE* pVtx;
	staticMesh->mesh->GetVertexBuffer(&vertexBuffer);
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		for (int i = 0; i < numVertices; i++)
		{
			D3DCOLOR* pColor = (D3DCOLOR*)vtxAccessor->getPointer(vtxAccess::COLOR, &pVtx[i * stride]);
			*pColor = red;
		}
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "頂点バッファのロックに失敗しました", NULL, MB_OK);
		return;
	}
#endif

#if 1	// 配列の中身をファイルにダンプ
	FILE* fp;
	setDataDirectory();

	// 頂点情報
	fp = fopen("VertexBufferOfNavMesh.txt", "w");
	fprintf(fp, "頂点バッファ配列の中身\n出力元：MeshData.cpp\n");
	for (int i = 0; i < numVertices; i++)
	{
		D3DXVECTOR3* test = (D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION, &vtxArray[i * stride]);
		fprintf(fp, "頂点%d, %f, %f, %f \n", i, test->x, test->y, test->z);
	}
	fclose(fp);

	// 頂点宣言
	fp = fopen("DeclarationOfNavMesh.txt", "w");
	fprintf(fp, "頂点宣言構造体の中身\n出力元：MeshData.cpp\n");
	for (int i = 0; i < 65; i++)
	{
		fprintf(fp, "\n[%d]",i);
		fprintf(fp, "Type : %d\n", staticMesh->vertexElement[i].Type);
		fprintf(fp, "Usage : %d\n", staticMesh->vertexElement[i].Usage);
	}
	fclose(fp);

	// インデックス情報
	fp = fopen("IndexBufferOfNavMesh.txt", "w");
	fprintf(fp, "インデックスバッファ配列の中身\n出力元：MeshData.cpp\n");
	fprintf(fp, "ポリゴンインデックス : ポリゴンを構成する頂点インデックス1, 2, 3\n\n");
	for (int i = 0; i < numFaces * 3; i++)
	{
		int unko = indexArray[i];
		fprintf(fp, "%d,", indexArray[i]);
		if(i % 3 == 2) fprintf(fp, "\n");
	}
	fclose(fp);

	// 隣接性情報
	fp = fopen("AdjacencyOfNavMesh.txt", "w");
	fprintf(fp, "隣接性情報配列の中身\n出力元：MeshData.cpp\n");
	fprintf(fp, "ポリゴンインデックス : 隣接するポリゴンインデックス1, 2, 3\n\n");
	for (int i = 0, j = 0; i < numFaces * 3; i++)
	{
		if (i % 3 == 0) fprintf(fp, "%4d : ", j++);
		fprintf(fp, "%d,", adjacencyArray[i]);
		if (i % 3 == 2) fprintf(fp, "\n");
	}
	fclose(fp);

	// ポリゴン面配列
	fp = fopen("FaceArray.txt", "w");
	fprintf(fp, "ポリゴン面配列の中身\n出力元：MeshData.cpp\n");
	for (int i = 0; i < getNumFaces(); i++)
	{
		// 面インデックス
		fprintf(fp, "【ポリゴンIndex%d】\n", i);
		for (int k = 0; k < 3; k++)
		{
			// 頂点インデックスと頂点データ
			fprintf(fp, "頂点Ind：%d　Pos(%f, %f, %f)\n",
				faceArray[i].vtx[k].index, faceArray[i].vtx[k].position.x,
				faceArray[i].vtx[k].position.y, faceArray[i].vtx[k].position.z);
		}
		// 隣接面インデックス
		fprintf(fp, "隣接面Ind: %d, %d, %d\n",
			faceArray[i].adjacency[0], faceArray[i].adjacency[1], faceArray[i].adjacency[2]);
		// 法線ベクトル
		fprintf(fp, "法線ベクトル(%f, %f, %f)\n\n",
			faceArray[i].nor.x, faceArray[i].nor.y, faceArray[i].nor.z);
	}
	fclose(fp);
#endif
}


//=============================================================================
// 終了処理
//=============================================================================
void MeshData::uninitialize()
{
	SAFE_DELETE_ARRAY(vtxArray);
	SAFE_DELETE_ARRAY(indexArray);
	SAFE_DELETE_ARRAY(adjacencyArray);
	SAFE_DELETE_ARRAY(faceArray);
}


//=============================================================================
// 頂点インデックスから頂点を取得(1)
//[引数1] DWORD number 頂点インデックスを指定
//=============================================================================
BYTE* MeshData::getVertexPointerFromVertexIndex(WORD number)
{
	return &vtxArray[stride * number];
}


//=============================================================================
// 頂点インデックスから頂点を取得(2)
//[引数1] DWORD number 頂点インデックスを指定
//[引数2] BYTE* vtx 探索する頂点配列を指定
//=============================================================================
BYTE* MeshData::getVertexPointerFromVertexIndex(WORD number, BYTE* vtx)
{
	return &vtx[stride * number];
}


//=============================================================================
// 面インデックスから頂点インデックスを取得(1)
//[引数1] DWORD number 面インデックスを指定
//[引数2] int selecter (0,1,2)で何番目の頂点か指定
//=============================================================================
WORD MeshData::getVertexIndexFromFaceIndex(DWORD number, int selecter)
{
	return indexArray[number * 3 + selecter];
}


//=============================================================================
// 面インデックスから頂点インデックスを取得(2)
//[引数1] DWORD number 面インデックスを指定
//[引数2] int selecter (0,1,2)で何番目の頂点か指定
//[引数3] WORD* vetexIndex 探索する頂点インデックス配列を指定
//=============================================================================
WORD MeshData::getVertexIndexFromFaceIndex(DWORD number, int selecter, WORD* vertexIndex)
{
	return indexArray[number * 3 + selecter];
}


//=============================================================================
// 面インデックスから隣接面インデックスを取得(1)
//[引数1] DWORD number 面インデックスを指定
//[引数2] int selecter (0,1,2)で何番目の隣接面か指定
//=============================================================================
DWORD MeshData::getAdjacentFaceIndexFromFaceIndex(DWORD number, int selecter)
{
	return adjacencyArray[number * 3 + selecter];
}


//=============================================================================
// 面インデックスから隣接面インデックスを取得(2)
//[引数1] DWORD number 面インデックスを指定
//[引数2] int selecter (0,1,2)で何番目の隣接面か指定
//[引数3] WORD* vetexIndex 探索する隣接性情報配列を指定
//=============================================================================
DWORD MeshData::getAdjacentFaceIndexFromFaceIndex(DWORD number, int selecter, DWORD* adjacency)
{
	return adjacency[number * 3 + selecter];
}


//=============================================================================
// 2つの面インデックスから共通の頂点インデックスを取り出す
//[引数1] DWORD number1 面インデックスを指定
//[引数2] DWORD number2 面インデックスを指定
//=============================================================================
Index2 MeshData::getSharingVertexIndexFromTwoFace(DWORD number1, DWORD number2)
{
	Index2 ans;
	ans.index[0] = ans.index[1] = -1;	// = 65535
	int cnt = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			if (getVertexIndexFromFaceIndex(number1, i) == getVertexIndexFromFaceIndex(number2, k))
			{
				ans.index[cnt++] = getVertexIndexFromFaceIndex(number1, i);
			}
		}
	}

	return ans;
}


//=============================================================================
// Getter
//=============================================================================
StaticMesh* MeshData::getStaticMesh() { return staticMesh; }
BYTE* MeshData::getVtxArray() { return vtxArray; };
WORD* MeshData::getIndexArray() { return indexArray; };
DWORD* MeshData::getAdjacencyArray() { return adjacencyArray; };
Face* MeshData::getFaceArray() { return faceArray; }
DWORD MeshData::getNumVertices() { return numVertices; };
DWORD MeshData::getStride() { return stride; };
DWORD MeshData::getNumFaces() { return numFaces; };
DWORD MeshData::getFVF() { return fvf; }