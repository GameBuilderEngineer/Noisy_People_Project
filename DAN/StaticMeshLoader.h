//===================================================================================================================================
//【StaticMeshLoader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/11/07
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
		//FIELD,									//01
		//CUBE,									//02
		//STAR_REGULAR_POLYHEDRON,				//03
		//STAR_REGULAR_POLYHEDRON_X10,			//04
		//STAR_REGULAR_POLYHEDRON_X100,			//05

		//SAMPLE_REDBULL,							//03
		SAMPLE_SCISSORS,						//08
		//ENEMY_01,
		//ENEMY_02,
		WOLF,
		TIGER,
		BEAR,
		WOLF_BODY,
		WOLF_ARM,
		TIGER_BODY,
		TIGER_GUN,
		TIGER_LEG_L,
		TIGER_LEG_R,
		BEAR_BODY, 
		BEAR_ARM_L,
		BEAR_ARM_R,
		BEAR_WAIST,
		BEAR_LEG_L,
		BEAR_LEG_R,

		//DEAD_TREE,								//01
		//GREEN_TREE_001,							//02
		GREEN_TREE_002,							//03
		GRASS,									//04
		//STONE_001,								//05
		//STONE_002,								//06
		STONE_003,								//07
		A_TRUNK,								//08
		A_LEAF,									//09
		B_TRUNK,								//10
		B_LEAF,									//11
		C_TRUNK,								//12
		C_LEAF,									//13
		OCEAN,									//14

		ITEM_BRANCH,


		//YAMADA_LAND,							//01
		//NAKAGOMI_LAND,							//02
		YAMADA_ROBOT,							//03
		//SAMPLE_NAVMESH,							//04
		YAMADA_ROBOT2,							//05
		//YAMADA_PLANE,							//06
		//DICE,									//07
		//YAMADA_TEST_ZONE,						//08
		SKY_DOME,								//09
		TUTORIAL_FILED,							//10
		//DATE_ISLAND,							//11
		//DATE_ISLAND_V2,							//12
		//DATE_ISLAND_V2_NAVI_MESH,				//13
		//DATE_ISLAND_V2_BLACK,					//14
		DATE_ISLAND_FINAL,						//15
		//DATE_ISLAND_FINAL_BLACK,				//16
		//DATE_ISLAND_FINAL_NAVIMESH,				//17
		DATE_ISLAND_FINAL_FACE,					//18
		DATE_ISLAND_FINAL_FACE_BLACK,			//19
		DIGITAL_SPHERE,							//01

		//NAV_TEST1,
		//NAV_TEST2,
		//NAV_TEST3,
		//NAV_TEST4,
		//NAV_TEST5,
		//NAV_TEST6,

		POWER_UP_ITEM,
		DISTANT_VIEW,

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

	//NormalとTangentの情報を付記する関数
	HRESULT withNormalTangent(
		LPDIRECT3DDEVICE9 device,	//描画デバイス
		LPD3DXMESH* sourceMesh		//元のメッシュ
		);

};

