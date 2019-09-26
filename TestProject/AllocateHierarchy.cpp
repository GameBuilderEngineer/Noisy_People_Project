//============================================================================================================================================
// Document
//============================================================================================================================================
// AllocateHierarchy.cpp
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/09/03
//============================================================================================================================================
#include "AllocateHierarchy.h"
//============================================================================================================================================
// Constructor
// コンストラクタ
//============================================================================================================================================
AllocateHierarchy::AllocateHierarchy(LPDIRECT3DDEVICE9 _device)
{
	this->device = _device;

	return;
}
//============================================================================================================================================
// Destructor
// デストラクタ
//============================================================================================================================================
AllocateHierarchy::~AllocateHierarchy(void)
{
	return;
}
//============================================================================================================================================
// allocateName
// フレームまたはメッシュの名前を保持する文字列に、メモリを割り当てる
//============================================================================================================================================
HRESULT AllocateHierarchy::allocateName(LPCSTR _name, LPSTR* _newName)
{
	UINT length = NULL;	//	長さ

	if (_name != NULL)
	{
		length = ((UINT)strlen(_name) + 1);
		*_newName = new CHAR[length];

		if (*_newName == NULL) { return E_OUTOFMEMORY; }

		memcpy(*_newName, _name, (length * sizeof(CHAR)));
	}
	else { *_newName = NULL; }

	return S_OK;
}
//============================================================================================================================================
// createSkinMesh
// スキンメッシュの作成
//============================================================================================================================================
HRESULT AllocateHierarchy::createSkinMesh(D3DXMeshContainerDerived* _meshContainer)
{
	D3DCAPS9 caps;	//	キャップ

	device->GetDeviceCaps(&caps);

	if (_meshContainer->pSkinInfo == NULL) { return S_OK; }

	SAFE_RELEASE(_meshContainer->MeshData.pMesh);
	SAFE_RELEASE(_meshContainer->boneCombinationBuffer);

	_meshContainer->pSkinInfo->ConvertToBlendedMesh		//	ConvertToBlendedMesh関数：メッシュを受け取り、頂点単位のブレンドの重みと、ボーンの組み合わせテーブルを適用した新しいメッシュを返す( このテーブルは、ボーンとメッシュのサブセットの関係を表す )
	(
		_meshContainer->originalMesh,					//	入力メッシュ
		(D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE),	//	オプション
		_meshContainer->pAdjacency,						//	メッシュの隣接データ( 入力 )
		NULL, 											//	メッシュの隣接データ( 出力 )
		NULL, 											//	ポリゴンの新規インデックスのバッファ
		NULL,											//	頂点の新規インデックスのバッファ
		&_meshContainer->boneWeightMax,					//	１つの頂点に影響を及ぼすウェイトの数
		&_meshContainer->boneMax,						//	ボーンの数
		&_meshContainer->boneCombinationBuffer,			//	ボーンデータが格納されたバッファ
		&_meshContainer->MeshData.pMesh					//	変換後のメッシュ
	);

	return S_OK;
}
//============================================================================================================================================
// CreateFrame
// フレームワークの作成・メモリの割り当て・初期化
//============================================================================================================================================
HRESULT AllocateHierarchy::CreateFrame(LPCSTR _name, LPD3DXFRAME* _newFrame)
{
	HRESULT result = S_OK;					//	リザルト
	D3DXFrameDerived* framePointer = NULL;	//	フレームポインタ

	*_newFrame = NULL;

	framePointer = new D3DXFrameDerived;

	if (framePointer == NULL)
	{
		// フレームポインタの解放
		//framePointer = NULL;		//NULLを代入してからデリートしては意味がないのでは？@tatsuki
		//SAFE_DELETE(framePointer);	

		result = E_OUTOFMEMORY;

		return result;
	}

	result = allocateName(_name, &framePointer->Name);

	if (FAILED(result))
	{
		// フレームポインタの解放
		//framePointer = NULL;		//NULLを代入してからデリートしては意味がないのでは？@tatsuki
		//SAFE_DELETE(framePointer);

		return result;
	}

	// フレームの、他データメンバを初期化
	D3DXMatrixIdentity(&framePointer->TransformationMatrix);
	D3DXMatrixIdentity(&framePointer->combinedTransformationMatrix);

	framePointer->pMeshContainer = NULL;
	framePointer->pFrameSibling = NULL;
	framePointer->pFrameFirstChild = NULL;

	*_newFrame = framePointer;

	// フレームポインタの解放
	//framePointer = NULL;		//NULLを代入してからデリートしては意味がないのでは？@tatsuki
	//SAFE_DELETE(framePointer);

	return result;
}
//============================================================================================================================================
// CreateMeshContainer
// メッシュコンテナオブジェクトを作成し、メッシュモデルデータを保存
//============================================================================================================================================
HRESULT AllocateHierarchy::CreateMeshContainer(LPCSTR _name, CONST D3DXMESHDATA* _meshData, CONST D3DXMATERIAL* _material, CONST D3DXEFFECTINSTANCE* _effectInstance, DWORD _materialNumber, CONST DWORD* _adjacency, LPD3DXSKININFO _skinInformation, LPD3DXMESHCONTAINER* _newMeshContainer)
{
	LPD3DXMESH mesh = NULL;									//	メッシュ
	HRESULT result = NULL;									//	リザルト
	UINT face = NULL;										//	フェイス数
	UINT boneCount = NULL;									//	ボーンカウント
	D3DXMeshContainerDerived* meshContainer = NULL;			//	メッシュコンテナ
	D3DXEFFECTINSTANCE errorElimination = *_effectInstance;	//	エラー排除

	*_newMeshContainer = NULL;

	// このサンプルはパッチメッシュを処理しないので、パッチメッシュを発見した場合は失敗
	if (_meshData->Type != D3DXMESHTYPE_MESH)
	{
		result = E_FAIL;

		// 割り当てられたメモリを適切にクリーンアップする
		if (meshContainer != NULL) { DestroyMeshContainer(meshContainer); }

		return result;
	}

	// メッシュデータ構造体から"pMesh"インターフェイスポインタを取得
	mesh = _meshData->pMesh;

	// このサンプルは"FVF"と互換性のあるメッシュでは無いので、発見した場合は失敗
	if (mesh->GetFVF() == 0)
	{
		result = E_FAIL;

		// 割り当てられたメモリを適切にクリーンアップする
		if (meshContainer != NULL) { DestroyMeshContainer(meshContainer); }

		return result;
	}

	// オーバーロードされた構造体を、"D3DXMESHCONTAINER"として返すように割り当てる
	meshContainer = new D3DXMeshContainerDerived;
	if (meshContainer == NULL)
	{
		result = E_OUTOFMEMORY;

		// 割り当てられたメモリを適切にクリーンアップする
		if (meshContainer != NULL) { DestroyMeshContainer(meshContainer); }

		return result;
	}

	memset(meshContainer, 0, sizeof(D3DXMeshContainerDerived));

	// 確認して名前をコピー( 入力として、全てのメモリは呼び出し元に属しているが、インターフェースは追加参照することができる )
	result = allocateName(_name, &meshContainer->Name);
	if (FAILED(result))
	{
		// 割り当てられたメモリを適切にクリーンアップする
		if (meshContainer != NULL) { DestroyMeshContainer(meshContainer); }

		return result;
	}

	mesh->GetDevice(&device);

	face = mesh->GetNumFaces();

	// メッシュに法線ベクトルが無い場合は追加
	if (!(mesh->GetFVF() & D3DFVF_NORMAL))
	{
		meshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// 法線ベクトルのスペースを確保するために、メッシュを複製
		result = mesh->CloneMeshFVF
		(
			mesh->GetOptions(),
			(mesh->GetFVF() | D3DFVF_NORMAL),
			device,
			&meshContainer->MeshData.pMesh
		);
		if (FAILED(result))
		{
			// 割り当てられたメモリを適切にクリーンアップする
			if (meshContainer != NULL) { DestroyMeshContainer(meshContainer); }

			return result;
		}

		// 新しいメッシュポインタを、メッシュコンテナから取り出して使用
		mesh = meshContainer->MeshData.pMesh;

		// メッシュの法線ベクトルを生成します
		D3DXComputeNormals(mesh, NULL);
	}
	// 法線ベクトルが無い場合は、メッシュコンテナのメッシュへの参照を追加する
	else
	{
		meshContainer->MeshData.pMesh = mesh;
		meshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		mesh->AddRef();
	}

	// 材料情報を格納するためのメモリを割り当てる：このサンプルは、"Effect_Instance"スタイルマテリアルの代わりに、マテリアルとテクスチャ名を使用する
	meshContainer->NumMaterials = max(1, _materialNumber);
	meshContainer->pMaterials = new D3DXMATERIAL[meshContainer->NumMaterials];
	meshContainer->texture = new LPDIRECT3DTEXTURE9[meshContainer->NumMaterials];
	meshContainer->pAdjacency = new DWORD[face * 3];

	if ((meshContainer->pAdjacency == NULL) || (meshContainer->pMaterials == NULL))
	{
		result = E_OUTOFMEMORY;

		// 割り当てられたメモリを適切にクリーンアップする
		if (meshContainer != NULL) { DestroyMeshContainer(meshContainer); }

		return result;
	}

	memcpy(meshContainer->pAdjacency, _adjacency, sizeof(DWORD) * face * 3);
	memset(meshContainer->texture, 0, sizeof(LPDIRECT3DTEXTURE9) * meshContainer->NumMaterials);

	if (_materialNumber > 0)	//	資料が提供されている場合は、それらをコピー
	{
		memcpy(meshContainer->pMaterials, _material, sizeof(D3DXMATERIAL) * _materialNumber);

		for (UINT i = 0; i < _materialNumber; i++)
		{
			if (meshContainer->pMaterials[i].pTextureFilename != NULL)
			{
				char textureName[256];
				ZeroMemory(textureName, sizeof(textureName));
				sprintf_s(textureName, "%s", meshContainer->pMaterials[i].pTextureFilename);

				if (FAILED(D3DXCreateTextureFromFile
				(
					device,
					textureName,
					&meshContainer->texture[i]
				)))
				{
					meshContainer->texture[i] = NULL;
				}
				meshContainer->pMaterials[i].pTextureFilename = NULL;
			}
		}
	}
	else	//	資料が提供されていない場合は、デフォルトの資料を使用する
	{
		meshContainer->pMaterials[0].pTextureFilename = NULL;

		memset(&meshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));

		meshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		meshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		meshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		meshContainer->pMaterials[0].MatD3D.Specular = meshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	// スキニング情報がある場合は、必要なデータを保存してから"HW"スキニングを設定
	if (_skinInformation != NULL)
	{
		// 最初にスキン情報と、オリジナルのメッシュデータを保存
		meshContainer->pSkinInfo = _skinInformation;
		_skinInformation->AddRef();

		meshContainer->originalMesh = mesh;
		mesh->AddRef();

		// 頂点をフィギュア空間からボーンの空間に移動するには、オフセット行列の配列が必要
		boneCount = _skinInformation->GetNumBones();
		meshContainer->boneOffsetMatrix = new D3DXMATRIX[boneCount];

		if (meshContainer->boneOffsetMatrix == NULL)
		{
			result = E_OUTOFMEMORY;

			// 割り当てられたメモリを適切にクリーンアップする
			if (meshContainer != NULL) { DestroyMeshContainer(meshContainer); }

			return result;
		}

		// 後で取得する必要がないように、各ボーンオフセット行列を取得
		for (UINT i = 0; i < boneCount; i++)
		{
			meshContainer->boneOffsetMatrix[i] = *(meshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
		}

		// 一般的なスキニング情報を受け取り、それをハードウェアに適したバージョンに変換
		result = createSkinMesh(meshContainer);

		if (FAILED(result))
		{
			// 割り当てられたメモリを適切にクリーンアップする
			if (meshContainer != NULL) { DestroyMeshContainer(meshContainer); }

			return result;
		}
	}

	*_newMeshContainer = meshContainer;

	meshContainer = NULL;

	// 割り当てられたメモリを適切にクリーンアップする
	if (meshContainer != NULL) { DestroyMeshContainer(meshContainer); }

	return result;
}
//============================================================================================================================================
// DestroyFrame
// フレームの解放
//============================================================================================================================================
HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME _frame)
{
	SAFE_DELETE_ARRAY(_frame->Name);

	//if (_frame->pFrameFirstChild)
	//{
	//	DestroyFrame(_frame->pFrameFirstChild);
	//}
	//
	//if (_frame->pFrameSibling)
	//{
	//	DestroyFrame(_frame->pFrameSibling);
	//}

	//SAFE_DELETE(_frame);

	return S_OK;
}
//============================================================================================================================================
// DestroyMeshContainer
// メッシュコンテナの解放
//============================================================================================================================================
HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER _baseMeshContainer)
{
	D3DXMeshContainerDerived* meshContainer = (D3DXMeshContainerDerived*)_baseMeshContainer;	//	メッシュコンテナ

	SAFE_DELETE_ARRAY(meshContainer->Name);
	SAFE_DELETE_ARRAY(meshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(meshContainer->pMaterials);

	// 割り当てられた全てのテクスチャを解放
	if (meshContainer->texture != NULL)
	{
		for (UINT i = 0; i < meshContainer->NumMaterials; i++)
		{
			SAFE_RELEASE(meshContainer->texture[i]);
		}
	}

	SAFE_DELETE_ARRAY(meshContainer->texture);
	SAFE_RELEASE(meshContainer->pSkinInfo);
	SAFE_RELEASE(meshContainer->boneCombinationBuffer);
	SAFE_DELETE_ARRAY(meshContainer->boneMatrix);
	SAFE_DELETE_ARRAY(meshContainer->boneOffsetMatrix);
	SAFE_RELEASE(meshContainer->MeshData.pMesh);
	SAFE_RELEASE(meshContainer->originalMesh);
	SAFE_DELETE(meshContainer);

	return S_OK;
}