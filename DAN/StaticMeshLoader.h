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
		SAMPLE_SCISSORS,
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

		GREEN_TREE_002,
		GRASS,
		STONE_003,
		A_TRUNK,
		A_LEAF,
		B_TRUNK,
		B_LEAF,
		C_TRUNK,
		C_LEAF,
		OCEAN,
		ITEM_BRANCH,


		YAMADA_ROBOT,
		YAMADA_ROBOT2,
		SKY_DOME,
		TUTORIAL_FILED,
		DATE_ISLAND_FINAL,
		DATE_ISLAND_FINAL_FACE,
		DATE_ISLAND_FINAL_FACE_BLACK,
		DIGITAL_SPHERE,

		POWER_UP_ITEM,
		DISTANT_VIEW,

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

	//NormalとTangentの情報を付記する関数
	HRESULT withNormalTangent(
		LPDIRECT3DDEVICE9 device,	//描画デバイス
		LPD3DXMESH* sourceMesh		//元のメッシュ
		);

};

