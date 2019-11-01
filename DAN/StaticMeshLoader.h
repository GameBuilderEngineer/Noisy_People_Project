//===================================================================================================================================
//【StaticMeshLoader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/24
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//【スタティックメッシュ構造体】
//===================================================================================================================================
struct StaticMesh
{
	LPDIRECT3DVERTEXBUFFER9				vertexBuffer;				//頂点情報バッファ
	LPDIRECT3DINDEXBUFFER9				indexBuffer;				//インデックスバッファ
	LPD3DXMESH							mesh;						//メッシュ情報
	D3DMATERIAL9*						materials;					//マテリアル情報格納先
	LPD3DXBUFFER						bufferMaterial;				//メッシュのマテリアルバッファ
	LPDIRECT3DTEXTURE9*					textures;					//テクスチャ情報格納先
	DWORD								numMaterial;				//マテリアル情報の数
	D3DVERTEXELEMENT9					vertexElement[65];			//頂点属性
	LPDIRECT3DVERTEXDECLARATION9		declaration;				//頂点宣言
	DWORD								numBytesPerVertex;			//頂点サイズ
	D3DXATTRIBUTERANGE*					attributeTable;				//属性テーブル
	DWORD								attributeTableSize;			//属性テーブルサイズ
};

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace staticMeshNS {
	enum MESH_NUMBER {
		FIELD,									//01
		CUBE,									//02
		STAR_REGULAR_POLYHEDRON,				//03
		STAR_REGULAR_POLYHEDRON_X10,			//04
		STAR_REGULAR_POLYHEDRON_X100,			//05

		SAMPLE_REDBULL,							//03
		SAMPLE_SCISSORS,						//08
		ENEMY_01,

		DEAD_TREE,								//01
		GREEN_TREE_001,							//02
		GREEN_TREE_002,							//03
		GRASS,									//04
		STONE_001,								//05
		STONE_002,								//06
		STONE_003,								//07

		YAMADA_LAND,							//01
		NAKAGOMI_LAND,							//02
		YAMADA_ROBOT,							//03
		SAMPLE_NAVMESH,							//04
		YAMADA_ROBOT2,							//05
		YAMADA_PLANE,							//06
		DICE,									//07
		YAMADA_TEST_ZONE,						//08
		SKY_DOME,								//09
		TUTORIAL_FILED,							//10

		MESH_NUM,								//NUM
	};

	//外部参照用関数
	StaticMesh* reference(int staticMeshNo);
}


//===================================================================================================================================
//【スタティックメッシュ読込クラス】
//===================================================================================================================================
class StaticMeshLoader :public Base
{
private:
	const char* fileName[staticMeshNS::MESH_NUM];
public:
	StaticMesh	staticMesh[staticMeshNS::MESH_NUM];
	StaticMeshLoader();
	~StaticMeshLoader();

	HRESULT load(LPDIRECT3DDEVICE9 device);
	void release(void);
};

