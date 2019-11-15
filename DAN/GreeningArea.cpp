#include "GreeningArea.h"



GreeningArea::GreeningArea()
{
	{//オブジェクトタイプと衝突対象の指定
		using namespace ObjectType;
		treeCell.type = GREENING_AREA;
		treeCell.target = TREE;
	}


}


GreeningArea::~GreeningArea()
{
}

void GreeningArea::update(float frameTime)
{

}

void GreeningArea::render()
{
	sphere->render();
}
