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

	fileName[SAMPLE_SCISSORS]						= { "scissors.x" };
	fileName[WOLF]									= { "Wolf.x" };
	fileName[TIGER]									= { "Tiger.x" };
	fileName[BEAR]									= { "Bear.x" };

	fileName[WOLF_BODY]								= { "Wolf_body.x" };
	fileName[WOLF_ARM]								= { "Wolf_arm.x" };
	fileName[TIGER_BODY]							= { "Tiger_Body.x" };
	fileName[TIGER_GUN]								= { "Tiger_Gun.x" };
	fileName[TIGER_LEG_L]							= { "Tiger_LegL.x" };
	fileName[TIGER_LEG_R]							= { "Tiger_LegR.x" };
	fileName[BEAR_BODY]								= { "Bear_Body.x" };
	fileName[BEAR_ARM_L]							= { "Bear_ArmL.x" };
	fileName[BEAR_ARM_R]							= { "Bear_ArmR.x" };
	fileName[BEAR_WAIST]							= { "Bear_Waist.x" };
	fileName[BEAR_LEG_L]							= { "Bear_LegL.x" };
	fileName[BEAR_LEG_R]							= { "Bear_LegR.x" };

	fileName[GREEN_TREE_002]						= { "greenTree002.x" };
	fileName[GRASS]									= { "grass001.x" };
	fileName[STONE_003]								= { "Rocks.x" };
	fileName[A_TRUNK]								= { "Tree_A_Branch.x" };
	fileName[A_LEAF]								= { "Tree_A_Leaf.x" };
	fileName[B_TRUNK]								= { "Tree_B_Branch.x" };
	fileName[B_LEAF]								= { "Tree_B_Leaf.x" };
	fileName[C_TRUNK]								= { "Tree_C_Branch.x" };
	fileName[C_LEAF]								= { "Tree_C_Leaf.x" };
	fileName[OCEAN]									= { "ocean.x" };

	fileName[ITEM_BRANCH]							= { "Bllue_branch.x" };

	fileName[YAMADA_ROBOT]							= { "SandBox_03_Robot.x" };
	fileName[YAMADA_ROBOT2]							= { "Robot.x" };
	fileName[SKY_DOME]								= { "domebase.x" };
	fileName[TUTORIAL_FILED]						= { "Tutorial_Stage.x" };
	
	fileName[DATE_ISLAND_FINAL]						= { "DateIsland_Final.x" };
	fileName[DATE_ISLAND_FINAL_FACE]				= { "DateIsland_Final_Face.x" };
	fileName[DATE_ISLAND_FINAL_FACE_BLACK]			= { "DateIsland_Final_Face_Black.x" };
	fileName[DIGITAL_SPHERE]						= { "digitalSphere.x" };

	fileName[POWER_UP_ITEM]							= { "powup.x" };
	fileName[DISTANT_VIEW] = { "Far_Island.x" };

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
//【NormalとTangentの情報を付記する関数】
//===================================================================================================================================
HRESULT StaticMeshLoader::withNormalTangent(
	LPDIRECT3DDEVICE9 device,	//描画デバイス
	LPD3DXMESH* sourceMesh		//元のメッシュ
) 
{
	//頂点宣言
	const D3DVERTEXELEMENT9 vertexDeclaration[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,	0},
		{0, 24, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,	0},
		{0, 36, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0,44,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BINORMAL,0},//頂点シェーダー上で算出
		D3DDECL_END()
	};

	LPD3DXMESH cloneMesh = NULL;		//クローンメッシュ
	//クローンメッシュ生成
	if (FAILED((*sourceMesh)->CloneMesh((*sourceMesh)->GetOptions(), vertexDeclaration, device, &cloneMesh)))
	{
		return E_FAIL;
	}
	//接線情報を追加
	//Normal算出
	if (FAILED(D3DXComputeNormals(cloneMesh, NULL)))MSG("D3DXComuteNormals");
	//Tangent算出
	if (FAILED(D3DXComputeTangent(cloneMesh,0,0,D3DX_DEFAULT,true,NULL)))MSG("D3DXComputeTangent");
	SAFE_RELEASE(*sourceMesh);
	*sourceMesh = cloneMesh;

	return D3D_OK;
}

//===================================================================================================================================
//【外部参照用関数】
//===================================================================================================================================
StaticMesh* staticMeshNS::reference(int staticMeshNo) {
	return &pointerStaticMeshLoader->staticMesh[staticMeshNo];
}

