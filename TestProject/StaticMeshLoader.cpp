//===================================================================================================================================
//【StaticMeshLoader.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/09/23
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "StaticMeshLoader.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace staticMeshNS;

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
static StaticMeshLoader* pointerStaticMeshLoader = NULL;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
StaticMeshLoader::StaticMeshLoader()
{
	//外部参照用ポインタへ代入
	pointerStaticMeshLoader = this;

	fileName[FIELD]														= { "testField.x" };
	fileName[CUBE]														= { "cube.x" };
	fileName[SAMPLE_HIERARCHY_MESH]						= { "HierarchyMesh.x" };
	fileName[PLANET]													= { "planet.x" };
	fileName[SAMPLE_ROBOT001]								= { "RobotB.x" };
	fileName[SAMPLE_ROBOT002]								= { "robotB_still_back.x" };
	fileName[SAMPLE_SKINNED_MESH]						= { "SkinnedMesh.x" };
	fileName[STAR_REGULAR_POLYHEDRON]				= { "starRegularPolyhedron.x" };
	fileName[STAR_REGULAR_POLYHEDRON_X10]			= { "x10starRegularPolyhedron.x" };
	fileName[STAR_REGULAR_POLYHEDRON_X100]		= { "x100starRegularPolyhedron.x" };
	fileName[SAMPLE_STATIC_MESH]							= { "StaticMesh.x" };
	fileName[SAMPLE_TOON_MESH]								= { "Toon_6Color.x" };
	fileName[SAMPLE_BUNNY]										= { "toybunny.x" };
	fileName[SAMPLE_PAPER]										= { "paper.x" };
	fileName[SAMPLE_REDBULL]									= { "redbull.x" };
	fileName[SAMPLE_SHOES]										= { "shoes.x" };
	fileName[SAMPLE_TEAPOT]									= { "teapot.x" };
	fileName[SAMPLE_LIGHT]										= { "light.x" };
	fileName[SAMPLE_BALL]											= { "soccerball.x" };
	fileName[SAMPLE_SCISSORS]									= { "scissors.x" };
	fileName[SAMPLE_PLAYSTATION]							= { "playstation.x" };
	fileName[SAMPLE_HAT]											= { "hat.x" };
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
StaticMeshLoader::~StaticMeshLoader()
{
	release();
}

//===================================================================================================================================
//【ロード】
//===================================================================================================================================
HRESULT StaticMeshLoader::load(LPDIRECT3DDEVICE9 device)
{
	setVisualDirectory();
	// Xファイルからメッシュをロードする
	for (int i = 0; i < MESH_NUM; i++)
	{
		staticMesh[i].bufferMaterial = NULL;
		D3DXLoadMeshFromX(
			(LPCSTR)fileName[i],
			D3DXMESH_SYSTEMMEM,
			device,
			NULL,
			&staticMesh[i].bufferMaterial,
			NULL,
			&staticMesh[i].numMaterial,
			&staticMesh[i].mesh);

		D3DXMATERIAL* materials = (D3DXMATERIAL*)staticMesh[i].bufferMaterial->GetBufferPointer();
		staticMesh[i].materials = new D3DMATERIAL9[staticMesh[i].numMaterial];
		staticMesh[i].textures = new LPDIRECT3DTEXTURE9[staticMesh[i].numMaterial];

		for (DWORD num = 0; num < staticMesh[i].numMaterial; num++)
		{
			staticMesh[i].materials[num] = materials[num].MatD3D;
			staticMesh[i].materials[num].Ambient = staticMesh[i].materials[num].Diffuse;
			staticMesh[i].textures[num] = NULL;
			if (materials[num].pTextureFilename != NULL && lstrlen(materials[num].pTextureFilename) > 0)
			{
				D3DXCreateTextureFromFile(device,materials[num].pTextureFilename,&staticMesh[i].textures[num]);
			}
		}

		//インデックスバッファの取得
		staticMesh[i].mesh->GetIndexBuffer(&staticMesh[i].indexBuffer);
		
		//属性テーブルサイズを取得
		staticMesh[i].mesh->GetAttributeTable(NULL, &staticMesh[i].attributeTableSize);

		//サイズ分のメモリ領域確保
		staticMesh[i].attributeTable = new D3DXATTRIBUTERANGE[staticMesh[i].attributeTableSize];

		//属性テーブルの取得
		staticMesh[i].mesh->GetAttributeTable(staticMesh[i].attributeTable, &staticMesh[i].attributeTableSize);

		//頂点属性の取得・設定
		D3DVERTEXELEMENT9 vertexElement[65];
		staticMesh[i].mesh->GetDeclaration(vertexElement);
		for (int num = 0; num < 65;num++)
		{
			staticMesh[i].vertexElement[num] = vertexElement[num];
		}
		device->CreateVertexDeclaration(vertexElement, &staticMesh[i].declaration);

		staticMesh[i].numBytesPerVertex = staticMesh[i].mesh->GetNumBytesPerVertex();
		
		//頂点バッファの取得
		staticMesh[i].mesh->GetVertexBuffer(&staticMesh[i].vertexBuffer);
	}

	return S_OK;
}

//===================================================================================================================================
//【解放処理】
//===================================================================================================================================
void StaticMeshLoader::release(void)
{
	for (int i = 0; i < MESH_NUM; i++)
	{
		for (DWORD num = 0; num < staticMesh[i].numMaterial; num++)
		{
			SAFE_RELEASE(staticMesh[i].textures[num]);
		}
		SAFE_RELEASE(staticMesh[i].vertexBuffer);
		SAFE_RELEASE(staticMesh[i].indexBuffer);
		SAFE_RELEASE(staticMesh[i].mesh);
		SAFE_RELEASE(staticMesh[i].declaration);
		SAFE_RELEASE(staticMesh[i].bufferMaterial);

		SAFE_DELETE_ARRAY(staticMesh[i].textures);
		SAFE_DELETE_ARRAY(staticMesh[i].materials);
		SAFE_DELETE_ARRAY(staticMesh[i].bufferMaterial);
	}
}

//===================================================================================================================================
//【外部参照用関数】
//===================================================================================================================================
StaticMesh* staticMeshNS::reference(int staticMeshNo) {
	return &pointerStaticMeshLoader->staticMesh[staticMeshNo];
}

