//-----------------------------------------------------------------------------
// マップオブジェクトクラス [MapObject.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/11/12
//-----------------------------------------------------------------------------
#include "MapObject.h"
#include "ImguiManager.h"
using namespace mapObjectNS;


int MapObject::numOfMapObject = 0;


//=============================================================================
// コンストラクタ
//=============================================================================
MapObject::MapObject(StaticMesh* _staticMesh, MapObjectData _itemData)
{
	numOfMapObject++;

	onGravity = true;
	mapObjectData = _itemData;
	position = mapObjectData.defaultPosition;

	if (CAN_COLLISION[mapObjectData.type])
	{
		treeCell.type = OBJECT_TYPE[mapObjectData.type];
		treeCell.target = COLLISION_TARGET[mapObjectData.type];
		setSize(SETTING_SIZE[mapObjectData.type]);
	}
	else
	{
		treeCell.type = ObjectType::NONE;
	}



	Object::initialize(&position);



	//setRadius(1.0f);


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


//=============================================================================
// Setter
//=============================================================================
void MapObject::setDataToMapObject(MapObjectData _itemData) { mapObjectData = mapObjectData; }
