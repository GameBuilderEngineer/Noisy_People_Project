//-----------------------------------------------------------------------------
// 衝突内容記述クラス [CollisionContent.h]
// 作成開始日 : 2019/11/9
//-----------------------------------------------------------------------------
#pragma once
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"

//=============================================================================
// CollisionContentクラスは衝突判定の衝突処理自体を担当する
//=============================================================================
class CollisionContent
{
private:
	Object* obj[2];

	// 引数として渡した2つのオブジェクト型ポインタのうち第3引数で指定された型に
	// キャストできるポインタを実際にその方にキャストして返却値とするメソッド
	template <typename T>
	T* castPointerFromTwoObject(Object* obj1, Object* obj2, DWORD objectType)
	{
		obj[0] = obj1, obj[1] = obj2;
		for (int i = 0; i < 2; i++)
		{
			if (obj[i]->objectType == objectType)
			{
				return (T*)(obj[i]);
			}
		}
		return NULL;
	}

public:
	// プレイヤーとエネミーと当たり判定
	void playerCollideEnemy(Object* obj1, Object* obj2);
	// プレイヤーとエネミーの攻撃の当たり判定
	void enemyAttacksPlayer(Object* obj1, Object* obj2);
};

