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
	//デジタルツリー標準エフェクト
	class DigitTree :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3 * syncPosition;
		DigitTree(D3DXVECTOR3* sync) {
			syncPosition = sync;
			effectNo = effekseerNS::DIGIT_TREE;}
		virtual void update() {
			position = *syncPosition;
			Instance::update();
		};
	};
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

}


//===================================================================================================================================
//【デジタルツリーエフェクトクラス】
//===================================================================================================================================
class DigitalTreeEffect :public Base
{
private:
	InstancingBillboard* billboard;

public:
	DigitalTreeEffect();
	~DigitalTreeEffect();

	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generateInstance(D3DXVECTOR3 position);
	void playStandardEffect(D3DXVECTOR3* position);
	void playRaidEffect(D3DXVECTOR3* position);

};

