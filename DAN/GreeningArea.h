//===================================================================================================================================
//【GreeningArea.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/15
// [更新日]2019/11/15
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Object.h"
#include "EffekseerManager.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace GreeningAreaNS
{
	//緑化エフェクト
	class GreeningEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		D3DXVECTOR3* syncScale;
		GreeningEffect(D3DXVECTOR3* syncPosition,D3DXVECTOR3* syncScale) {
			this->syncPosition = syncPosition;
			this->syncScale = syncScale;
			effectNo = effekseerNS::GREENING;
		}
		virtual void update() {
			position = *syncPosition;
			scale = *syncScale;
			Instance::update();
		};
	};

	//枯木化エフェクト
	class DeadingEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		D3DXVECTOR3* syncScale;
		DeadingEffect(D3DXVECTOR3* syncPosition,D3DXVECTOR3* syncScale) {
			this->syncPosition = syncPosition;
			this->syncScale = syncScale;
			effectNo = effekseerNS::DEADING;
		}
		virtual void update() {
			position = *syncPosition;
			scale = *syncScale;
			Instance::update();
		};
	};

	enum MODE
	{
		GREENING_MODE,
		DEAD_MODE,
	};
}



//===================================================================================================================================
//【緑化範囲クラス：衝突判定を行うために作成】
//===================================================================================================================================
class GreeningArea :public Object
{
private:

public:
	int playerNo;
	int mode;
	GreeningArea();
	~GreeningArea();
	void update(float frameTime);
	void render();	
};

