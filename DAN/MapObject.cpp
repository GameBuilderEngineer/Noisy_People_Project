//-----------------------------------------------------------------------------
// マップオブジェクトクラス [MapObject.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/11/12
//-----------------------------------------------------------------------------
#include "MapObject.h"
#include "ImguiManager.h"
#include "ObjectTypeList.h"
using namespace mapObjectNS;


// Staticメンバ変数
int MapObject::numOfMapObject = 0;


//=============================================================================
// コンストラクタ
//=============================================================================
MapObject::MapObject(StaticMesh* _staticMesh, MapObjectData _mapObjData)
{
	numOfMapObject++;
	staticMesh = _staticMesh;

	// マップオブジェクトデータをセット
	mapObjectData = _mapObjData;
	position = mapObjectData.defaultPosition;
	axisZ.direction = mapObjectData.defaultDirection;

	{// 衝突判定の設定
		using namespace ObjectType;
		treeCell.type = MAPOBJECT;
		treeCell.target = PLAYER | ENEMY;
	}

	// オブジェクトの初期化
	onGravity = true;
	setSize(SETTING_SIZE[mapObjectData.type]);
	Object::initialize(&position);

	// メッシュ半径を取得
	BoundingSphere radiusCheck;
	radiusCheck.initialize(&position, _staticMesh->mesh);
	setRadius(radiusCheck.getRadius());

	// 向き変更
	postureControl(axisZ.direction, mapObjectData.defaultDirection, 1);
}


//=============================================================================
// デストラクタ
//=============================================================================
MapObject::~MapObject()
{
	numOfMapObject--;
}


//=============================================================================
// 更新処理
//=============================================================================
void MapObject::update(float frameTime)
{
	Object::update();
}


//=============================================================================
// 重力発生メッシュ（接地メッシュ）の設定
//=============================================================================
void MapObject::setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
}


//=============================================================================
// Getter
//=============================================================================
int MapObject::getNumOfMapObject(){ return numOfMapObject; }
MapObjectData* MapObject::getMapObjectData() { return &mapObjectData; }
StaticMesh* MapObject::getStaticMesh() { return staticMesh; }

//=============================================================================
// Setter
//=============================================================================
void MapObject::setDataToMapObject(MapObjectData _itemData) { mapObjectData = mapObjectData; }
