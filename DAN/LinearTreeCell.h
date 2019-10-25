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
#include <vector>
#include <list>

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace std;

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
		cellArray	= new Cell<T>*[cellNum];
		ZeroMemory(cellArray, sizeof(Cell<T>)*cellNum);

		//領域を登録
		this->left			= left;
		this->top			= top;
		this->width			= right - left;
		this->height		= bottom - top;
		this->unitWidth		= width / (1 << level);
		this->unitHeight	= height / (1 << level);
		this->level			= level;

		return true;

	}

	//オブジェクトを登録する
	bool registObject(float left, float top, float right, float bottom, ObjectTree<T>* objectTree)
	{
		//オブジェクトの境界範囲から登録モートン番号を算出
		DWORD element = getMortonNumber(left, top, right, bottom);
		if (element < cellNum)
		{
			//空間が無い場合は新規作成
			if (!cellArray[element])
			{
				createNewCell(element);
			}
			return cellArray[element]->push(objectTree);
		}
		return false;
	}

	//衝突判定リストを作成する
	DWORD getAllCollisionList(vector<T*> &collisionVector)
	{
		//リスト（配列）は必ず初期化する
		collisionVector.clear();

		//ルート空間の存在をチェック
		if (cellArray[0] == NULL)	return 0;	//空間が存在していない

		//ルート空間を処理
		list<T*> collisionStac;
		getCollisionList(0, collisionVector, collisionStac);

		return (DWORD)collisionVector.size();

	}

protected:
	//空間内で衝突リストを作成する
	bool getCollisionList(DWORD element, vector<T*> &collisionVector, list<T*> &collisionStac)
	{
		typename list<T*>::iterator it;
		//①空間内のオブジェクト同士の衝突リスト作成
		ObjectTree<T>* objectTree1 = cellArray[element]->getFirstObject();
		while (objectTree1 != NULL)
		{
			ObjectTree<T>* objectTree2 = objectTree1->next;
			while (objectTree2 != NULL)
			{
				//衝突リスト作成
				collisionVector.push_back(objectTree1->object);
				collisionVector.push_back(objectTree2->object);
				objectTree2 = objectTree2->next;
			}
			//②衝突スタックとの衝突リスト作成
			for (it = collisionStac.begin(); it != collisionStac.end(); it++)
			{
				collisionVector.push_back(objectTree1->object);
				collisionVector.push_back(*it);
			}
			objectTree1 = objectTree1->next;
		}

		bool childFlag = false;
		//③子空間に移動
		DWORD objectNum = 0;
		DWORD i, nextElement;
		for (i = 0; i < 4; i++)
		{
			nextElement = element * 4 + 1 * i;
			if (nextElement < cellNum 
				&& cellArray[element * 4 + 1 + i])
			{
				if (!childFlag)
				{
					//④登録オブジェクトをスタックに追加
					objectTree1 = cellArray[element]->getFirstObject();
					while (objectTree1)
					{
						collisionStac.push_back(objectTree1->object);
						objectNum++;
						objectTree1 = objectTree1->next;
					}
				}
				childFlag = true;
				getCollisionList(element * 4 + 1 + i, collisionVector, collisionStac);	//子空間へ
			}
		}

		//⑤スタックからオブジェクトを外す
		if (childFlag)
		{
			for (i = 0; i < objectNum; i++)
			{
				collisionStac.pop_back();
			}
		}

		return true;

	}

	//空間を生成
	bool createNewCell(DWORD element)
	{
		//引数の要素番号
		while (!cellArray[element])
		{
			//指定の要素番号に空間を新規作成
			cellArray[element] = new Cell<T>;

			//親空間にジャンプ
			element = (element - 1) >> 2;
			if (element >= cellNum)break;

		}
		return true;
	}

	//座標から空間番号を算出
	DWORD getMortonNumber(float left, float top, float right, float bottom)
	{
		//最小レベルにおける各軸位置算出
		DWORD leftTop = getPointElement(left, top);
		DWORD rightBottom = getPointElement(right, bottom);

		//空間番号を引き算して
		//最上位区切りから所属レベルを算出
		DWORD difference = rightBottom - leftTop;
		unsigned int hiLevel = 0;
		unsigned int i;
		for (i = 0; i < level; i++)
		{
			DWORD check = (difference >> (i * 2)) & 0x3;
			if (check != 0)
			{
				hiLevel = i + 1;
			}
			DWORD spaceNum = rightBottom >> (hiLevel * 2);
			DWORD addNum = (pow[level - hiLevel] - 1) / 3;
			spaceNum += addNum;

			if (spaceNum > cellNum)
				return 0xffffffff;

			return spaceNum;
		}
	}

	//ビット分割関数
	DWORD bitSeparate32(DWORD n)
	{
		n = (n | (n << 8)) & 0x00ff00ff;
		n = (n | (n << 4)) & 0x0f0f0f0f;
		n = (n | (n << 2)) & 0x33333333;
		return (n | (n << 1)) & 0x55555555;
	}

	//2Dモートン空間番号算出関数
	WORD get2DMortonNumber(WORD x, WORD y)
	{
		return (WORD)(bitSeparate32(x) | (bitSeparate32(y) << 1));
	}

	//座標→線形4分木要素番号変換関数
	DWORD getPointElement(float x, float y)
	{
		return get2DMortonNumber((WORD)((x - left) / unitWidth), (WORD)((y - top) / unitHeight));
	}
};

//===================================================================================================================================
//【空間クラス】
//template<class T>
//class Cell
//{
//protected:
//	ObjectTree<T>* latest;	//	最新ObjectTreeへのポインタ
//
//public:
//	Cell();				//コンストラクタ
//	virtual ~Cell();	//デストラクタ
//
//	void resetLink(ObjectTree<T>* &objectTree);	//リンクを全てリセットする
//
//	bool push(ObjectTree<T>* &objectTree);	//ObjectTreeをプッシュ
//
//	ObjectTree<T>* getFirstObject();	//最初のオブジェクトツリーを取得
//
//	bool onRemove(ObjectTree<T>* removeObject);	//削除されるオブジェクトチェック
//}
//===================================================================================================================================
template<class T>
class Cell
{
protected:
	ObjectTree<T>* latest;	//	最新ObjectTreeへのポインタ

public:
	//コンストラクタ
	Cell(){}

	//デストラクタ
	virtual ~Cell()
	{
		if (latest != NULL)
			resetLink(latest);
	}

	//リンクを全てリセットする
	void resetLink(ObjectTree<T>* &objectTree)
	{
		if (objectTree->next != NULL)
		{
			resetLink(objectTree->next);
		}
		objectTree = NULL;	//NULLでリセット
	}

	//ObjectTreeをプッシュ
	bool push(ObjectTree<T>* &objectTree)
	{
		if (objectTree == NULL)			return false;	//無効オブジェクトは登録しない
		if (objectTree->cell == this)	return false;	//２重登録チェック
		if (latest == NULL)
		{
			latest = objectTree;						//空間に新規登録
		}
		else
		{
			//最新ObjectTreeオブジェクトを更新
			objectTree->next	= latest;
			latest->prev		= objectTree;
			latest				= objectTree;
		}
		objectTree->registerCell(this);					//空間を登録
		return true;
	}

	ObjectTree<T>* getFirstObject()
	{
		return latest;
	}

	//削除されるオブジェクトチェック
	bool onRemove(ObjectTree<T>* removeObject)
	{
		if (latest == removeObject)
		{
			//次のオブジェクトに挿げ替え
			if (latest != NULL)
				latest = latest->getNextObj();
		}
		return true;
	}
};