//===================================================================================================================================
//【StaticMeshLoader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/04
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
		FIELD,
		CUBE,
		SAMPLE_HIERARCHY_MESH,
		PLANET,
		SAMPLE_ROBOT001,
		SAMPLE_ROBOT002,
		SAMPLE_SKINNED_MESH,
		STAR_REGULAR_POLYHEDRON,
		STAR_REGULAR_POLYHEDRON_X10,
		STAR_REGULAR_POLYHEDRON_X100,
		SAMPLE_STATIC_MESH,
		SAMPLE_TOON_MESH,
		SAMPLE_BUNNY,
		SAMPLE_PAPER,
		SAMPLE_REDBULL,
		SAMPLE_SHOES,
		SAMPLE_TEAPOT,
		SAMPLE_LIGHT,
		SAMPLE_BALL,
		SAMPLE_SCISSORS,
		SAMPLE_PLAYSTATION,
		SAMPLE_HAT,
		DEAD_TREE,
		YAMADA_LAND,
		NAKAGOMI_LAND,
		YAMADA_ROBOT,
		SAMPLE_NAVMESH,
		YAMADA_ROBOT2,
		YAMADA_PLANE,
		DICE,
		YAMADA_TEST_ZONE,
		
		MESH_NUM,
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

