//===================================================================================================================================
//【StaticMeshLoader.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/11/07
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

	fileName[FIELD]									= { "testField.x" };						//01
	fileName[CUBE]									= { "cube.x" };								//02
	fileName[STAR_REGULAR_POLYHEDRON]				= { "starRegularPolyhedron.x" };			//03
	fileName[STAR_REGULAR_POLYHEDRON_X10]			= { "x10starRegularPolyhedron.x" };			//04
	fileName[STAR_REGULAR_POLYHEDRON_X100]			= { "x100starRegularPolyhedron.x" };		//05

	fileName[SAMPLE_REDBULL]						= { "Enemy01.x" };							//03
	fileName[SAMPLE_SCISSORS]						= { "scissors.x" };							//08
	fileName[ENEMY_01]								= { "Enemy01.x" };
	fileName[ENEMY_02]								= { "Enemy02.x" };
	fileName[WOLF]									= { "Wolf.x" };
	fileName[TIGER]									= { "Tiger.x" };
	fileName[BEAR]									= { "Bear.x" };

	fileName[DEAD_TREE]								= { "deadTree001.x" };						//01
	fileName[GREEN_TREE_001]						= { "greenTree001.x" };						//02
	fileName[GREEN_TREE_002]						= { "greenTree002.x" };						//03
	fileName[GRASS]									= { "grass001.x" };							//04
	fileName[STONE_001]								= { "stone001.x" };							//05
	fileName[STONE_002]								= { "stone002.x" };							//06
	fileName[STONE_003]								= { "Rocks.x" };							//07
	fileName[A_TRUNK]								= { "tree_01_blanch.x" };					//08
	fileName[A_LEAF]								= { "tree_01_leaf.x" };						//09
	fileName[B_TRUNK]								= { "tree_02_blanch.x" };					//10
	fileName[B_LEAF]								= { "tree_02_leaf.x" };						//11
	fileName[OCEAN]									= { "Water4.x" };							//12

	fileName[YAMADA_LAND]							= { "SandBox_01_TestIsland.x" };			//01
	fileName[NAKAGOMI_LAND]							= { "nakagomiLand.x" };						//02
	fileName[YAMADA_ROBOT]							= { "SandBox_03_Robot.x" };					//03
	fileName[SAMPLE_NAVMESH]						= { "SandBox_02_NavMesh.x" };				//04
	fileName[DICE]									= { "dice.x" };								//05
	fileName[YAMADA_PLANE]							= { "plane.x" };							//06
	fileName[YAMADA_ROBOT2]							= { "Robot.x" };							//07
	fileName[YAMADA_TEST_ZONE]						= { "SandBox.x" };							//08
	fileName[SKY_DOME]								= { "domebase.x" };							//09
	fileName[TUTORIAL_FILED]						= { "Tutorial_Stage.x" };					//10
	fileName[DATE_ISLAND]							= { "DateIsland_prev.x" };					//11
	fileName[DATE_ISLAND_V2]						= { "DateIsland_v2.x" };					//12
	fileName[DATE_ISLAND_V2_NAVI_MESH]				= { "mesh.x" };								//13
	fileName[DATE_ISLAND_V2_BLACK]					= { "DateIsland_v2_Black.x" };					//14

	fileName[DIGITAL_SPHERE]						= { "digitalSphere.x" };					//01


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
	// Xファイルからメッシュをロードする
	for (int i = 0; i < MESH_NUM; i++)
	{
		setModelDirectory();
		staticMesh[i].bufferMaterial = NULL;
		D3DXLoadMeshFromX(
			(LPCSTR)fileName[i],
			D3DXMESH_MANAGED,
			device,
			NULL,
			&staticMesh[i].bufferMaterial,
			NULL,
			&staticMesh[i].numMaterial,
			&staticMesh[i].mesh);

		D3DXMATERIAL* materials = (D3DXMATERIAL*)staticMesh[i].bufferMaterial->GetBufferPointer();
		staticMesh[i].materials = new D3DMATERIAL9[staticMesh[i].numMaterial];
		staticMesh[i].textures = new LPDIRECT3DTEXTURE9[staticMesh[i].numMaterial];
		setTextureDirectory();
		for (DWORD num = 0; num < staticMesh[i].numMaterial; num++)
		{
			staticMesh[i].materials[num] = materials[num].MatD3D;
			staticMesh[i].materials[num].Ambient = staticMesh[i].materials[num].Diffuse;
			staticMesh[i].textures[num] = NULL;
			if (materials[num].pTextureFilename != NULL && lstrlen(materials[num].pTextureFilename) > 0)
			{
				D3DXCreateTextureFromFile(device,materials[num].pTextureFilename,&staticMesh[i].textures[num]);
			}
			else
			{
				D3DXCreateTextureFromFile(device, "white.png", &staticMesh[i].textures[num]);
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

