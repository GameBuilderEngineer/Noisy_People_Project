//===================================================================================================================================
//【LinearTreeCell.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/10/23
// [更新日]2019/11/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Direct3D9.h"
#include "ShaderLoader.h"
#include "Ray.h"
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
//【分木登録オブジェクトクラス】
//class ObjectTree
//{
//public:
//	Cell<T>*			cell;			//登録空間
//	T*					object;			//判定対象オブジェクト
//	ObjectTree<T>*		prev;			//前のObjectTreeクラスポインタ
//	ObjectTree<T>*		next;			//次のObjectTreeクラスポインタ
//public:
//	ObjectTree()						//コンストラクタ
//	virtual ~ObjectTree()				//デストラクタ
//public:
//	bool remove()						//自らリストから外れる
//	void registerCell(Cell<T>* cell)	//空間を登録
//	ObjectTree<T>* getNextObj()			//次のオブジェクトへのポインタを取得
//};
//===================================================================================================================================
#pragma region ObjectTree
template<class T>
class ObjectTree
{
public:
	Cell<T>*			cell;	//登録空間
	T*					object;	//判定対象オブジェクト
	ObjectTree<T>*		prev;	//前のObjectTreeクラスポインタ
	ObjectTree<T>*		next;	//次のObjectTreeクラスポインタ
	int					type;	//自身のオブジェクトタイプ
	int					target;	//衝突すべきオブジェクトタイプ
public:
	//コンストラクタ
	ObjectTree()
	{
		cell	= NULL;
		object	= NULL;
		prev	= NULL;
		next	= NULL;
		type	= 0;
		target	= 0;
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
		if (!cell->onRemove(this))return false;

		//逸脱処理
		//前後のオブジェクト結びつける
		if (prev != 0)
		{
			prev->next = next;
		}
		if (next != 0)
		{
			next->prev = prev;
		}
		prev = 0;
		next = 0;
		cell = NULL;
		return true;
	}

	//空間を登録
	void registerCell(Cell<T>* cell)
	{
		this->cell = cell;
	}

	//次のオブジェクトへのポインタを取得
	ObjectTree<T>* getNextObj()
	{
		return next;
	}

};
#pragma endregion

//===================================================================================================================================
//【衝突リスト】
//メモリ確保後、リストそのものを解放するまで、メモリ解放をせず保持する。
//class CollisionList
//{
//private:
//	T**		root;			//リストルートポインタ
//	size_t	pos;			//書き込みポインタ
//	size_t	mallocSize;		//メモリ確保サイズ
//
//public:
//	CollisionList() :root(0), pos(0), mallocSize(0)		//コンストラクタ
//	~CollisionList()									//デストラクタ
//
//	size_t getSize()									//サイズ取得
//	T** getRoot()										//ルートポインタ取得
//	void resetPos()										//メモリポインタのリセット
//	void write(T* obj1, T* obj2)						//書き込み
//	void reflesh()										//解放処理
//};
//===================================================================================================================================
#pragma region CollisionList
#define COLLISIONLIST_REALLOCSIZE	100
template <class T>
class CollisionList
{
public:
	//コンストラクタ
	CollisionList() :root(0), pos(0), mallocSize(0)
	{
		root = (T**)malloc(0);
	}
	//デストラクタ
	~CollisionList()
	{
		reflesh();
	}
	//サイズ取得
	size_t getSize()
	{
		return pos;
	}
	//ルートポインタ取得
	T** getRoot()
	{
		return root;
	}
	//メモリポインタのリセット
	void resetPos()
	{
		pos = 0;
	}
	//書き込み
	void write(T* obj1, T* obj2) {
		if (pos >= mallocSize)
		{
			root = (T**)realloc(root, sizeof(T*)*(mallocSize + COLLISIONLIST_REALLOCSIZE));
			mallocSize += COLLISIONLIST_REALLOCSIZE;
		}
		root[pos++] = obj1;
		root[pos++] = obj2;
	}
	//解放処理
	void reflesh()
	{
		if (root)
		{
			free(root);
		}
	}
private:
	T**		root;			//リストルートポインタ
	size_t	pos;			//書き込みポインタ
	size_t	mallocSize;		//メモリ確保サイズ
};
#pragma endregion

//===================================================================================================================================
//【線形４分木名前空間】
//===================================================================================================================================
namespace linear4TreeNS
{
	//算出式：(2<<Level+1)^2/3
	//レベル0空間数：1
	//レベル1空間数：5
	//レベル2空間数：21
	//レベル3空間数：85
	//レベル4空間数：341
	//レベル5空間数：1,365
	//レベル6空間数：5,461
	//レベル7空間数：21,845
	//レベル8空間数：87,381
	//レベル9空間数：349,525
	const int MAX_LEVEL = 9;	//最大分割レベル
}

//===================================================================================================================================
//【線形４分木管理クラス】
//class Linear4TreeManager :public Base
//{
//protected:
//	Cell<T>**						cellArray;															//線形空間ポインタ配列
//	unsigned int					pow[linear4TreeNS::MAX_LEVEL + 1];									//べき乗数値配列
//	float							width;																//領域のX軸幅
//	float							height;																//領域のY軸幅
//	float							left;																//領域の左側(X軸最小値)
//	float							top;																//領域の上側(Y軸最小値)
//	float							unitWidth;															//最小レベル空間の幅単位
//	float							unitHeight;															//最小レベル空間の高さ単位
//	DWORD							cellNum;															//空間の数
//	unsigned int					level;																//最下位レベル
//	CollisionList<T>				collisionList;														//衝突リスト
//
//public:
//	Linear4TreeManager()																				//コンストラクタ
//	virtual ~Linear4TreeManager()																		//デストラクタ
//
//	bool initialize(unsigned int level, float left, float top, float right, float bottom)				//線形４分木配列を構築する
//	bool registerObject(float left, float top, float right, float bottom, ObjectTree<T>* objectTree)	//オブジェクトを登録する
//	DWORD getAllCollisionList(CollisionList<T>** collisionList)											//衝突判定リストを作成する
//
//#if _DEBUG
//	void render()																						//分割ライン描画
//#endif
//
//protected:
//	bool getCollisionList(DWORD element, list<T*> &collisionStac)										//空間内で衝突リストを作成する
//	DWORD getMortonNumber(float left, float top, float right, float bottom)								//座標から空間番号を算出
//	DWORD bitSeparate32(DWORD n)																		//ビット分割関数
//	WORD get2DMortonNumber(WORD x, WORD y)																//2Dモートン空間番号算出関数
//	DWORD getPointElement(float x, float y)																//座標→線形4分木要素番号変換関数
//};
//===================================================================================================================================
#pragma region Linear4TreeManager
template <class T>
class Linear4TreeManager :public Base
{
protected:
	Cell<T>**						cellArray;							//線形空間ポインタ配列
	unsigned int					pow[linear4TreeNS::MAX_LEVEL + 1];	//べき乗数値配列
	float							width;								//領域のX軸幅
	float							height;								//領域のY軸幅
	float							left;								//領域の左側(X軸最小値)
	float							top;								//領域の上側(Y軸最小値)
	float							unitWidth;							//最小レベル空間の幅単位
	float							unitHeight;							//最小レベル空間の高さ単位
	DWORD							cellNum;							//空間の数
	unsigned int					level;								//最下位レベル
	CollisionList<T>				collisionList;						//衝突リスト

#ifdef _DEBUG
	Ray*							line;								//描画用ライン
	float*							length;
	int								splitSum;							//ライン分割合計
#endif // _DEBUG

public:
	//コンストラクタ
	Linear4TreeManager()
	{
		cellArray		= NULL;
		level			= 0;
		width			= 0.0f;
		height			= 0.0f;
		left			= 0.0f;
		top				= 0.0f;
		unitWidth		= 0.0f;
		unitHeight		= 0.0f;
		cellNum			= 0;

#ifdef _DEBUG
		line			= NULL;
		splitSum		= 0;
		length			= 0;
#endif
		//各レベルでの空間数を算出
		pow[0] = 1;
		for (int i = 1; i < linear4TreeNS::MAX_LEVEL + 1; i++)
		{
			pow[i] = pow[i - 1] * 4;
		}
	}

	//デストラクタ
	virtual ~Linear4TreeManager()
	{
		for (DWORD i = 0; i < cellNum; i++)
		{
			if(cellArray[i] != NULL)
				delete cellArray[i];
		}
		SAFE_DELETE_ARRAY(cellArray);
#ifdef _DEBUG
		SAFE_DELETE_ARRAY(line);
		SAFE_DELETE_ARRAY(length);
#endif // _DEBUG

	}

	//線形４分木配列を構築する
	bool initialize(unsigned int level, float left, float top, float right, float bottom)
	{
		//設定最高レベル以上の空間は作れない
		if (level >= linear4TreeNS::MAX_LEVEL)return false;

		//levelレベル(0基点)の配列作成
		cellNum		= (pow[level + 1] - 1) / 3;
		cellArray	= new Cell<T>*[cellNum];
		ZeroMemory(cellArray, sizeof(Cell<T>*)*cellNum);

		//領域を登録
		this->left			= left;
		this->top			= top;
		this->width			= right - left;
		this->height		= bottom - top;
		this->unitWidth		= width / (1 << level);
		this->unitHeight	= height / (1 << level);
		this->level			= level;

#ifdef _DEBUG
		//DEBUG用分割空間描画情報の設定
		int splitNum	= (int)((1 << level) + 1);
		splitSum		= 0;
		splitSum += splitNum;//x
		splitSum += splitNum;//z
		line = new Ray[splitSum];
		D3DXVECTOR3 direction;
		length = new float[splitSum];

		//x軸始点ライン
		for (int i = 0; i < splitNum; i++)
		{
			length[i] = Base::between2VectorDirection(
				&direction, 
				D3DXVECTOR3(left + (unitWidth*i), 0, -height / 2), 
				D3DXVECTOR3(left + (unitWidth*i), 0, height / 2));
			line[i].initialize(D3DXVECTOR3(left + (unitWidth*i), 0, -height / 2), direction);
			line[i].color = D3DXCOLOR(255, 0, 0, 255);
		}

		//z軸始点ライン
		for (int i = 0; i < splitNum; i++)
		{
			length[splitNum + i] = Base::between2VectorDirection(
				&direction,
				D3DXVECTOR3(-width / 2, 0, top + (unitHeight*i)) ,
				D3DXVECTOR3(width / 2, 0, top + (unitHeight*i)) );
			line[splitNum + i].initialize(D3DXVECTOR3(-width / 2, 0, top + (unitHeight*i)), direction);
			line[splitNum + i].color = D3DXCOLOR(0, 0, 255, 255);
		}
#endif // _DEBUG

		return true;

	}

	//オブジェクトを登録する
	bool registerObject(float left, float top, float right, float bottom, ObjectTree<T>* objectTree)
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
	DWORD getAllCollisionList(CollisionList<T>** collisionList)
	{
		//リスト（配列）は必ず初期化する
		this->collisionList.resetPos();

		//ルート空間の存在をチェック
		if (cellArray[0] == NULL)	return 0;	//空間が存在していない

		//ルート空間を処理
		list<T*> collisionStac;
		getCollisionList(0, collisionStac);

		*collisionList = &this->collisionList;

		return (DWORD)this->collisionList.getSize();

	}
#if _DEBUG
	//空間分割ライン描画
	void render()
	{
		for(int i = 0; i < splitSum; i++)
		{
			line[i].render(length[i]);
		}
	}
#endif


protected:
	//空間内で衝突リストを作成する
	bool getCollisionList(DWORD element, list<T*> &collisionStac)
	{
		typename list<T*>::iterator it;
		//①空間内のオブジェクト同士の衝突リスト作成
		ObjectTree<T>* objectTree1 = cellArray[element]->getFirstObject();
		while (objectTree1 != 0)
		{
			ObjectTree<T>* objectTree2 = objectTree1->next;
			while (objectTree2 != 0)
			{
				//衝突リスト作成
				collisionList.write(objectTree1->object, objectTree2->object);
				objectTree2 = objectTree2->next;
			}
			//②衝突スタックとの衝突リスト作成
			for (it = collisionStac.begin(); it != collisionStac.end(); it++)
			{
				collisionList.write(objectTree1->object, *it);
			}
			objectTree1 = objectTree1->next;
		}

		bool childFlag = false;
		//③子空間に移動
		DWORD objectNum = 0;
		DWORD i, nextElement;
		for (i = 0; i < 4; i++)
		{
			nextElement = element * 4 + 1 + i;
			if (nextElement < cellNum && cellArray[element * 4 + 1 + i])
			{
				if (!childFlag)
				{
					//④登録オブジェクトをスタックに追加
					objectTree1 = cellArray[element]->getFirstObject();
					while (objectTree1 != 0)
					{
						collisionStac.push_back(objectTree1->object);
						objectNum++;
						objectTree1 = objectTree1->next;
					}
				}
				childFlag = true;
				getCollisionList(element * 4 + 1 + i, collisionStac);	//子空間へ
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

		//空間番号の排他的論理和から
		//所属レベルを算出
		DWORD difference = rightBottom ^ leftTop;
		unsigned int hiLevel = 0;
		unsigned int i;
		for (i = 0; i < level; i++)
		{
			DWORD check = (difference >> (i * 2)) & 0x3;
			if (check != 0)
			{
				hiLevel = i + 1;
			}
		}
		DWORD spaceNum = rightBottom >> (hiLevel * 2);
		DWORD addNum = (pow[level - hiLevel] - 1) / 3;
		spaceNum += addNum;

		if (spaceNum > cellNum)
			return 0xffffffff;

		return spaceNum;
		
	}

	//ビット分割関数
	DWORD bitSeparate32(DWORD n)
	{
		n = (n | (n << 8)) & 0x00ff00ff;		//0000 0000 1111 1111 0000 0000 1111 1111
		n = (n | (n << 4)) & 0x0f0f0f0f;		//0000 1111 0000 1111 0000 1111 0000 1111
		n = (n | (n << 2)) & 0x33333333;		//0011 0011 0011 0011 0011 0011 0011 0011
		return (n | (n << 1)) & 0x55555555;		//0101 0101 0101 0101 0101 0101 0101 0101
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
#pragma endregion

//===================================================================================================================================
//【線形８分木名前空間】
//===================================================================================================================================
namespace linear8TreeNS
{
	//算出式：(2<<Level+1)^3/7
	//レベル0空間数：1
	//レベル1空間数：9
	//レベル2空間数：73
	//レベル3空間数：585
	//レベル4空間数：4,681
	//レベル5空間数：37,449
	//レベル6空間数：299,593
	//レベル7空間数：2,396,745
	//レベル8空間数：19,173,961
	//レベル9空間数：153,391,689
	const int MAX_LEVEL = 9;	//最大分割レベル
}

//===================================================================================================================================
//【線形８分木管理クラス】
//class Linear8TreeManager :public Base
//{
//protected:
//	Cell<T>**						cellArray;											//線形空間ポインタ配列
//	unsigned int					pow[linear8TreeNS::MAX_LEVEL + 1];					//べき乗数値配列
//	D3DXVECTOR3						width;												//領域の幅
//	D3DXVECTOR3						regionMin;											//領域の最小値
//	D3DXVECTOR3						regionMax;											//領域の最大値
//	D3DXVECTOR3						unit;												//最小領域の辺の長さ
//	DWORD							cellNum;											//空間の数
//	unsigned int					level;												//最下位レベル
//	CollisionList<T>				collisionList;										//衝突リスト
//
//public:
//	Linear8TreeManager()																//コンストラクタ
//	virtual ~Linear8TreeManager()														//デストラクタ
//
//	bool initialize(unsigned int level, D3DXVECTOR3 min, D3DXVECTOR3 max)				//【初期化】線形８分木配列を構築する
//	bool registerObject(D3DXVECTOR3* min, D3DXVECTOR3* max, ObjectTree<T>* objectTree)	//オブジェクトを登録する
//	DWORD getAllCollisionList(CollisionList<T>** collisionList)							//衝突判定リストを作成する
//
//#if _DEBUG
//	void render()																		//分割ライン描画
//#endif
//
//protected:
//	bool getCollisionList(DWORD element, list<T*> &collisionStac)						//空間内で衝突リストを作成する
//	bool createNewCell(DWORD element)													//空間を生成
//	DWORD getMortonNumber(D3DXVECTOR3* min, D3DXVECTOR3* max)							//座標から空間番号を算出
//	DWORD bitSeparateFor3D(DWORD n)														//ビット分割関数
//	DWORD get3DMortonNumber(BYTE x, BYTE y, BYTE z)										//3Dモートン空間番号算出関数
//	DWORD getPointElement(D3DXVECTOR3 &point)											//座標→線形8分木要素番号変換関数
//}
//===================================================================================================================================
#pragma region Linear8TreeManager
template <class T>
class Linear8TreeManager :public Base
{
protected:
	Cell<T>**						cellArray;							//線形空間ポインタ配列
	unsigned int					pow[linear8TreeNS::MAX_LEVEL + 1];	//べき乗数値配列
	D3DXVECTOR3						width;								//領域の幅
	D3DXVECTOR3						regionMin;							//領域の最小値
	D3DXVECTOR3						regionMax;							//領域の最大値
	D3DXVECTOR3						unit;								//最小領域の辺の長さ
	DWORD							cellNum;							//空間の数
	unsigned int					level;								//最下位レベル
	CollisionList<T>				collisionList;						//衝突リスト

#ifdef _DEBUG
	Ray*							xLine;								//描画用ライン
	Ray*							yLine;								//描画用ライン
	Ray*							zLine;								//描画用ライン
	float							xLength;								//描画長さ
	float							yLength;								//描画長さ
	float							zLength;								//描画長さ
	int								splitSum;							//分割ライン合計
#endif // _DEBUG

public:
	//コンストラクタ
	Linear8TreeManager()
	{
		cellArray	= NULL;
		width		= D3DXVECTOR3(1, 1, 1);
		regionMin	= D3DXVECTOR3(0, 0, 0);
		regionMax	= D3DXVECTOR3(1, 1, 1);
		unit		= D3DXVECTOR3(1, 1, 1);
		cellNum		= 0;
		level		= 0;

#ifdef _DEBUG
		xLine = NULL;
		yLine = NULL;
		zLine = NULL;
		splitSum = 0;
		xLength = 0;
		yLength = 0;
		zLength = 0;
#endif
		//各レベルでの空間数を算出
		pow[0] = 1;
		for (int i = 1; i < linear8TreeNS::MAX_LEVEL + 1; i++)
		{
			pow[i] = pow[i - 1] * 8;
		}
	}

	//デストラクタ
	virtual ~Linear8TreeManager()
	{
		for (DWORD i = 0; i < cellNum; i++)
		{
			if (cellArray[i] != NULL)
				delete cellArray[i];
		}
		SAFE_DELETE_ARRAY(cellArray);
#ifdef _DEBUG
		SAFE_DELETE_ARRAY(xLine);
		SAFE_DELETE_ARRAY(yLine);
		SAFE_DELETE_ARRAY(zLine);
#endif // _DEBUG
	}

	//【初期化】線形８分木配列を構築する
	bool initialize(unsigned int level, D3DXVECTOR3 min, D3DXVECTOR3 max)
	{
		//設定最高レベル以上の空間は作れない
		if (level >= linear8TreeNS::MAX_LEVEL)return false;

		//levelレベル(0基点)の配列作成
		cellNum = (pow[level + 1] - 1) / 7;
		cellArray = new Cell<T>*[cellNum];
		ZeroMemory(cellArray, sizeof(Cell<T>*)*cellNum);

		//領域を登録
		this->regionMin = min;
		this->regionMax = max;
		this->width		= regionMax - regionMin;
		this->unit		= width/((float)(1<<level));
		this->level		= level;


#ifdef _DEBUG
		//DEBUG用分割空間描画情報の設定
		int splitNum = (int)((1 << level) + 1);
		splitSum = splitNum * splitNum;
		xLine = new Ray[splitSum];
		yLine = new Ray[splitSum];
		zLine = new Ray[splitSum];
		D3DXVECTOR3 direction;
		xLength = max.x - min.x;
		yLength = max.y - min.y;
		zLength = max.z - min.z;

		//X軸始点ライン
		direction = D3DXVECTOR3(1, 0, 0);
		for (int j = 0; j < splitNum; j++)
		{
			for (int i = 0; i < splitNum; i++)
			{
				xLine[i+ (j*splitNum)].initialize(D3DXVECTOR3(min.x, min.y + (unit.y*j), min.z+(unit.z*i)), direction);
				xLine[i+ (j*splitNum)].color = D3DXCOLOR(255, 0, 0, 255);
			}
		}
		//Y軸始点ライン
		direction = D3DXVECTOR3(0, 1, 0);
		for (int j = 0; j < splitNum; j++)
		{
			for (int i = 0; i < splitNum; i++)
			{
				yLine[i + (j*splitNum)].initialize(D3DXVECTOR3(min.x + (unit.x*j), min.y, min.z+(unit.z*i)), direction);
				yLine[i + (j*splitNum)].color = D3DXCOLOR(0, 255, 0, 255);
			}
		}
		//Z軸始点ライン
		direction = D3DXVECTOR3(0, 0, 1);
		for (int j = 0; j < splitNum; j++)
		{
			for (int i = 0; i < splitNum; i++)
			{
				zLine[i+ (j*splitNum)].initialize(D3DXVECTOR3(min.x + (unit.x*j), min.y + (unit.y*i), min.z), direction);
				zLine[i+ (j*splitNum)].color = D3DXCOLOR(0, 0, 255, 255);
			}
		}
#endif // _DEBUG

		return true;
	}

	//オブジェクトを登録する
	bool registerObject(D3DXVECTOR3* min,D3DXVECTOR3* max,ObjectTree<T>* objectTree)
	{
		//オブジェクトの境界範囲から登録モートン番号を算出
		DWORD element = getMortonNumber(min,max);
		if (element < cellNum)
		{
			//空間が無い場合は新規作成
			if (!cellArray[element])
			{
				createNewCell(element);
			}
			return cellArray[element]->push(objectTree);
		}
		return false;//登録失敗
	}

	//衝突判定リストを作成する
	DWORD getAllCollisionList(CollisionList<T>** collisionList)
	{
		//リスト（配列）は必ず初期化する
		this->collisionList.resetPos();

		//ルート空間の存在をチェック
		if (cellArray[0] == NULL)	return 0;	//空間が存在していない

		//ルート空間を処理
		list<T*> collisionStac;
		getCollisionList(0, collisionStac);

		*collisionList = &this->collisionList;

		return (DWORD)this->collisionList.getSize();
	}
#if _DEBUG
	//分割ライン描画
	void render()
	{
		for (int i = 0; i < splitSum; i++)
		{
			xLine[i].render(xLength);
		}
		for (int i = 0; i < splitSum; i++)
		{
			yLine[i].render(yLength);
		}
		for (int i = 0; i < splitSum; i++)
		{
			zLine[i].render(zLength);
		}
	}
#endif

protected:
	//空間内で衝突リストを作成する
	bool getCollisionList(DWORD element, list<T*> &collisionStac)
	{
		typename list<T*>::iterator it;
		//①空間内のオブジェクト同士の衝突リスト作成
		ObjectTree<T>* objectTree1 = cellArray[element]->getFirstObject();
		while (objectTree1 != 0)
		{
			ObjectTree<T>* objectTree2 = objectTree1->next;
			while (objectTree2 != 0)
			{
				//衝突リスト作成
				if (objectTree1->type & objectTree2->target ||
					objectTree2->type & objectTree1->target)
				{
					collisionList.write(objectTree1->object, objectTree2->object);
				}
				objectTree2 = objectTree2->next;
			}
			//②衝突スタックとの衝突リスト作成
			for (it = collisionStac.begin(); it != collisionStac.end(); it++)
			{
				if (objectTree1->type & (*it)->treeCell.target 
					||	(*it)->treeCell.type & objectTree1->target)
				{
					collisionList.write(objectTree1->object, *it);
				}
			}
			objectTree1 = objectTree1->next;
		}

		bool childFlag = false;
		//③子空間に移動
		DWORD objectNum = 0;
		DWORD i, nextElement;
		for (i = 0; i < 8; i++)
		{
			nextElement = element * 8 + 1 + i;
			if (nextElement < cellNum && cellArray[element * 8 + 1 + i])
			{
				if (!childFlag)
				{
					//④登録オブジェクトをスタックに追加
					objectTree1 = cellArray[element]->getFirstObject();
					while (objectTree1 != 0)
					{
						collisionStac.push_back(objectTree1->object);
						objectNum++;
						objectTree1 = objectTree1->next;
					}
				}
				childFlag = true;
				getCollisionList(element * 8 + 1 + i, collisionStac);	//子空間へ
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
			element = (element - 1) >> 3;
			if (element >= cellNum)break;

		}
		return true;
	}

	//座標から空間番号を算出
	DWORD getMortonNumber(D3DXVECTOR3* min,D3DXVECTOR3* max)
	{
		//最小レベルにおける各軸位置算出
		DWORD leftTop		= getPointElement(*min);
		DWORD rightBottom	= getPointElement(*max);

		//空間番号の排他的論理和から
		//所属レベルを算出
		DWORD difference = rightBottom ^ leftTop;
		unsigned int hiLevel = 0;
		unsigned int i;
		for (i = 0; i < level; i++)
		{
			DWORD check = (difference >> (i * 3)) & 0x7;
			if (check != 0)
			{
				hiLevel = i + 1;
			}
		}
		DWORD spaceNum = rightBottom >> (hiLevel * 3);
		DWORD addNum = (pow[level - hiLevel] - 1) / 7;
		spaceNum += addNum;

		if (spaceNum > cellNum)
			return 0xffffffff;

		return spaceNum;

	}

	//ビット分割関数
	DWORD bitSeparateFor3D(DWORD n)
	{
		DWORD s = n;
		s = (s | (s << 8)) & 0x0000f00f;		//0000 0000 0000 0000 1111 0000 0000 1111
		s = (s | (s << 4)) & 0x000c30c3;		//0000 0000 0000 1100 0011 0000 1100 0011
		s = (s | (s << 2)) & 0x00249249;		//0000 0000 0010 0100 1001 0010 0100 1001
		return s;
	}

	//3Dモートン空間番号算出関数
	DWORD get3DMortonNumber(BYTE x, BYTE y,BYTE z)
	{
		return bitSeparateFor3D(x) | bitSeparateFor3D(y) << 1 | bitSeparateFor3D(z) << 2;
	}

	//座標→線形8分木要素番号変換関数
	DWORD getPointElement(D3DXVECTOR3 &point)
	{
		return get3DMortonNumber(
			(BYTE)((point.x-regionMin.x)/unit.x),
			(BYTE)((point.y-regionMin.y)/unit.y),
			(BYTE)((point.z-regionMin.z)/unit.z));
	}
};
#pragma endregion

//===================================================================================================================================
//【空間クラス】
//template<class T>
//class Cell
//{
//protected:
//	ObjectTree<T>* latest;	//	最新ObjectTreeへのポインタ
//
//public:
//	Cell();										//コンストラクタ
//	virtual ~Cell();							//デストラクタ
//
//	void resetLink(ObjectTree<T>* &objectTree);	//リンクを全てリセットする
//
//	bool push(ObjectTree<T>* &objectTree);		//ObjectTreeをプッシュ
//
//	ObjectTree<T>* getFirstObject();			//空間の先頭に登録されたオブジェクトツリーを取得
//
//	bool onRemove(ObjectTree<T>* removeObject);	//削除されるオブジェクトチェック
//}
//===================================================================================================================================
#pragma region Cell
template<class T>
class Cell
{
protected:
	ObjectTree<T>* latest;	//	最新ObjectTreeへのポインタ

public:
	//コンストラクタ
	Cell():latest(0){}

	//デストラクタ
	virtual ~Cell()
	{
		if (latest != NULL)
			resetLink(latest);
	}

	//リンクを全てリセットする
	void resetLink(ObjectTree<T>* objectTree)
	{
		if (objectTree->next != 0)
		{
			resetLink(objectTree->next);
		}
		objectTree = 0;
	}

	//ObjectTreeをプッシュ
	bool push(ObjectTree<T>* &objectTree)
	{
		if (objectTree == 0)			return false;	//無効オブジェクトは登録しない
		if (objectTree->cell == this)	return false;	//２重登録チェック
		if (latest == 0)
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

	//空間の先頭に登録されたオブジェクトを取得する
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
#pragma endregion
