//-----------------------------------------------------------------------------
// マップオブジェクトクラス [MapObject.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/11/12
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"


//=============================================================================
// 名前空間
//=============================================================================
namespace mapObjectNS
{
	enum MAP_OBJECT_TYPE
	{
		STONE_01,
		MAP_OBJECT_MAX
	};

	typedef struct MapObjectData
	{
		int mapObjectID;				// 識別番号(0..*)
		int type;						// アイテムタイプ
		D3DXVECTOR3 defaultPosition;	// 初期座標
		D3DXVECTOR3 defaultDirection;	// 初期正面方向
		void zeroClear() { ZeroMemory(this, sizeof(MapObjectData)); }
	}MAPOBJECTSET;
}


//=============================================================================
//クラス定義
//=============================================================================
class MapObject: public Object
{
private:
	mapObjectNS::MapObjectData mapObjectData;	// マップオブジェクトデータ
	LPD3DXMESH	attractorMesh;					// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;				// 重力（引力）発生オブジェクトマトリックス
	BoundingSphere sphereCollider;				// コライダ

	// Static
	static int numOfMapObject;				// アイテムオブジェクトの総数

public:
	MapObject(StaticMesh* _staticMesh, mapObjectNS::MapObjectData _itemData);
	~MapObject();
	virtual void update(float frameTime);
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);

	// Getter
	static int getNumOfMapObject();					// マップオブジェクトの数を取得
	mapObjectNS::MapObjectData* getMapObjectData();	// マップオブジェクトデータを取得
	BoundingSphere* getSphereCollider();			// コライダの取得

	// Setter
	void setDataToMapObject(mapObjectNS::MapObjectData _itemData);
};
