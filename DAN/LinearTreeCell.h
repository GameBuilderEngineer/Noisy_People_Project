//===================================================================================================================================
//【LinearTreeCell.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/10/23
// [更新日]2019/10/23
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//【空間クラス】
//===================================================================================================================================
template <class T>
class Cell;

//===================================================================================================================================
//【分木登録オブジェクト】
//===================================================================================================================================
template<class T>
class ObjectTree
{
public:
	Cell<T>*		cell;	//登録空間
	T*				object;	//判定対象オブジェクト
	ObjectTree*		prev;	//前のObjectTreeクラスポインタ
	ObjectTree*		next;	//次のObjectTreeクラスポインタ
public:
	//コンストラクタ
	ObjectTree()
	{
		cell	= NULL;
		object	= NULL;
		prev	= NULL;
		next	= NULL;
	}
	
	//デストラクタ
	virtual ~ObjectTree()
	{

	}
public:
	//自らリストから外れる
	bool remove()
	{
		//すでに逸脱している時は処理終了
		if (!cell)return false;

		//自分を登録している空間に自身を通知
		if (cell->onRemove(this))return false;

		//逸脱処理
		//前後のオブジェクト結びつける
		if (prev != NULL)
		{
			prev->next = next;
			prev = NULL;
		}
		if (next != NULL)
		{
			next->prev = prev;
			next = NULL;
		}
		cell = NULL;
		return true;
	}

	//空間を登録
	void registerCell(Cell<T>* cell)
	{

	}

	//次のオブジェクトへのポインタを取得
	ObjectTree<T>* getNextObj()
	{
		return next;
	}

};

//===================================================================================================================================
//【線形４分木名前空間】
//===================================================================================================================================
namespace linear4TreeNS
{
	const int MAX_LEVEL = 9;	//最大分割レベル
}

//===================================================================================================================================
//【線形４分木管理クラス】
//===================================================================================================================================
template <class T>
class Linear4TreeManager
{
protected:
	unsigned int		dim;
	Cell<T>**			cellArray;							//線形空間ポインタ配列
	unsigned int		pow[linear4TreeNS::MAX_LEVEL + 1];	//べき乗数値配列
	float				width;								//領域のX軸幅
	float				height;								//領域のY軸幅
	float				left;								//領域の左側(X軸最小値)
	float				top;								//領域の上側(Y軸最小値)
	float				unitWidth;							//最小レベル空間の幅単位
	float				unitHeight;							//最小レベル空間の高さ単位
	DWORD				cellNum;							//空間の数
	unsigned int		level;								//最下位レベル
public:
	//コンストラクタ
	Linear4TreeManager()
	{
		level		= 0;
		width		= 0.0f;
		height		= 0.0f;
		left		= 0.0f;
		top			= 0.0f;
		unitWidth	= 0.0f;
		unitHeight	= 0.0f;
		cellNum		= 0.0f;
		cellArray	= NULL;
		dim			= 0;
	}

	//デストラクタ
	virtual ~Linear4TreeManager()
	{
		for (DWORD i = 0; i < cellNum; i++)
		{
			SAFE_DELETE(cellArray[i]);
		}
		SAFE_DELETE_ARRAY(cellArray);
	}

	//線形４分木配列を構築する
	bool initialize(unsigned int level, float left, float top, float right, float bottom)
	{
		//設定最高レベル以上の空間は作れない
		if (level >= linear4TreeNS::MAX_LEVEL)return false;

		//各レベルでの空間数を算出
		pow[0] = 1;
		for (int i = 1; i < linear4TreeNS::MAX_LEVEL+1; i++)
		{
			pow[i] = pow[i - 1] * 4;
		}

		//levelレベル(0基点)の配列作成
		cellNum		= (pow[level + 1] - 1) / 3;
		cellArray	= new Cell<T>*()[cellNum];
		ZeroMemory(cellArray, sizeof(Cell<T>)*cellNum);

		//領域を登録


	}

};

