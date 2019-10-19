//-----------------------------------------------------------------------------
// �A�C�e�����N���X [Item.cpp]
// Author : 
// �쐬�J�n�� : 2019/  /
//-----------------------------------------------------------------------------
#include "Item.h"
#include "ImguiManager.h"
using namespace itemNS;


int Item::numOfItem = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Item::Item(StaticMesh* _staticMesh, ItemData* _itemData)
{
	numOfItem++;

	onGravity = true;
	itemData = _itemData;
	position = itemData->initialPosition;
	Object::initialize(&position);
	sphereCollider.initialize(&position, _staticMesh->mesh);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Item::~Item()
{
	numOfItem--;
}


//=============================================================================
// �X�V����
//=============================================================================
void Item::update(float frameTime)
{
	Object::update();
}


//=============================================================================
// Getter
//=============================================================================
int Item::getNumOfItem(){ return numOfItem; }
ItemData* Item::getItemData() { return itemData; }


//=============================================================================
// Setter
//=============================================================================
void Item::setDataToItem(ItemData* _itemData) { itemData = _itemData; }