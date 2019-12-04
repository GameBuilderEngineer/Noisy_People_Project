//===================================================================================================================================
//【TreeEffect.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/25
// [更新日]2019/12/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "InstancingBillboard.h"
#include "EffekseerManager.h"
#include "Tree.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace TreeEffectNS
{
	//===================================================================================================================================
	//【ツリー状態マーカーインスタンスクラス】
	//===================================================================================================================================
	class MarkerInstance :public InstancingBillboardNS::Instance
	{
	public:
		Tree* tree;
	public:
		//コンストラクタ
		MarkerInstance(Tree* owner);
		//更新
		virtual void update(float frameTime) override;
		void changeColor();
	};

	//===================================================================================================================================
	//【ツリー標識インスタンスクラス】
	//===================================================================================================================================
	class SignInstance :public InstancingBillboardNS::Instance
	{
	public:
		Tree* tree;
	public:
		//コンストラクタ
		SignInstance(Tree* owner);
		//更新
		virtual void update(float frameTime) override;
		void changeColor();
	};

	enum {
		INSTANCE_MARKER,
		INSTANCE_SIGN,
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
class TreeEffect :public Base
{
private:
	InstancingBillboard* marker;	//状態マーカー
	InstancingBillboard* sign;		//
	float effectCycle;							//エフェクト周期



public:
	TreeEffect();
	~TreeEffect();

	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generateInstance(int type,Tree* position);
	void resetEffectCycle();
	void playStandardEffect(D3DXVECTOR3* position);
	void playRaidEffect(int no,D3DXVECTOR3* position);

	void enableRender();
	void disableRender();

};

