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
	// マップオブジェクトの種類
	// モデル違いも全てタイプとして分けしている
	enum MAPOBJECT_TYPE
	{
		STONE_01,
		STONE_02, 
		STONE_03,

		MAPOBJECT_MAX
	};

	// サイズ
	const D3DXVECTOR3 SETTING_SIZE[MAPOBJECT_MAX] =
	{
		D3DXVECTOR3(1.7f, 1.0f, 1.7f),	// STONE_01
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
	StaticMesh*	staticMesh;						// メッシュ情報
	LPD3DXMESH	attractorMesh;					// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;				// 重力（引力）発生オブジェクトマトリックス

	// Static
	static int numOfMapObject;					// アイテムオブジェクトの総数

public:
	MapObject(StaticMesh* _staticMesh, mapObjectNS::MapObjectData _mapObjData);
	~MapObject();
	// 更新
	virtual void update(float frameTime);
	// 重力発生メッシュの設定
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	// Getter
	static int getNumOfMapObject();					// マップオブジェクトの数を取得
	mapObjectNS::MapObjectData* getMapObjectData();	// マップオブジェクトデータを取得
	StaticMesh* getStaticMesh();					// メッシュ情報を取得
	// Setter
	void setDataToMapObject(mapObjectNS::MapObjectData _itemData);
};
