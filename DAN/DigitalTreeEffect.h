//===================================================================================================================================
//【DigitalTreeEffect.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/25
// [更新日]2019/11/25
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "InstancingBillboard.h"
#include "EffekseerManager.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace DigitalTreeEffectNS
{
	//===================================================================================================================================
	//【エフェクトインスタンスクラス】
	//===================================================================================================================================
	class EffectInstance :public InstancingBillboardNS::Instance
	{
	public:
		//コンストラクタ
		EffectInstance(D3DXVECTOR3 position);
		//更新
		virtual void update(float frameTime) override;
	};

	//===================================================================================================================================
	//【エフェクトインスタンスクラス】
	//===================================================================================================================================

	//デジタルツリー襲撃エフェクト
	class DigitTreeRaid :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3 * syncPosition;
		DigitTreeRaid(D3DXVECTOR3* sync) {
			syncPosition = sync;
			effectNo = effekseerNS::DIGIT_TREE_RAID;}
		virtual void update() {
			position = *syncPosition;
			Instance::update();
		};
	};
	//エフェクト発生周期
	const float EFFECT_CYCLE = 0.1f;

}


//===================================================================================================================================
//【デジタルツリーエフェクトクラス】
//===================================================================================================================================
class DigitalTreeEffect :public Base
{
private:
	InstancingBillboard* billboard;
	float effectCycle;							//エフェクト周期



public:
	DigitalTreeEffect();
	~DigitalTreeEffect();

	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generateInstance(D3DXVECTOR3 position);
	void resetEffectCycle();
	void playStandardEffect(D3DXVECTOR3* position);
	void playRaidEffect(int no,D3DXVECTOR3* position);

};

