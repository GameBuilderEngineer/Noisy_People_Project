//===================================================================================================================================
//【DigitalShiftEffect.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/25
// [更新日]2019/11/25
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshRenderer.h"
#include "EffekseerManager.h"
#include "InstancingBillboard.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace DigitalShiftEffectNS
{
	//エフェクト再生リスト
	const int	START_SHIFT			= 0x00000001;
	const int	RUNNING_SHIFT		= 0x00000002;
	const int	END_SHIFT			= 0x00000004;
	const int	SELECT_LIGHT		= 0x00000008;


	//エフェクト時間
	const float	START_TIME			= 1.0f;
	const float	RUNNING_TIME		= 1.0f;
	const float	END_TIME			= 1.0f;

	//デジタルツリー選択エフェクト
	//選択用ライトクラス
	class SelectLight:public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		SelectLight(int no,D3DXVECTOR3* sync) {
			syncPosition = sync;
			scale = D3DXVECTOR3(1.0f, 30.0f, 1.0f);
			managerNo = no;
			effectNo = effekseerNS::DIGIT_TREE_SELECT;
		}
		virtual void update() {
			position = *syncPosition;
			Instance::update();
		};
	};

}

//===================================================================================================================================
//【デジタルシフトエフェクトクラス】
//===================================================================================================================================
class DigitalShiftEffect :public Base
{
private:
	//デジタルスフィア
	LinkedList<Object*>*	sphereList;		
	StaticMeshRenderer*		sphereRenderer;

	
	InstancingBillboard*	billboard;


	//選択表示ライトリスト
	bool onPlayedSelectLight;
	DigitalShiftEffectNS::SelectLight* selectLight;

	//同期位置リスト
	std::vector<D3DXVECTOR3*> syncPositionList;

public:
	//コンストラクタ/デストラクタ
	DigitalShiftEffect();
	~DigitalShiftEffect();
	//基本
	void update(float frameTime);
	void updateSelectLight(D3DXVECTOR3 position);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//再生/一時停止
	void play(int effectType,D3DXVECTOR3 position);
	//void syncPlay(int effectType,D3DXVECTOR3* syncPosition);
	void pause();
	
	//再生リスト
	void playStartShift(D3DXVECTOR3 position);
	void playRunningShift(D3DXVECTOR3 position);
	void playEndShift(D3DXVECTOR3 position);
	void playSelectLight(int no,D3DXVECTOR3* position);
	void stopSelectLight(int no);
	void shownSelectLigth(bool shown);

	//オブジェクト生成
	void createDigitalSphere(D3DXVECTOR3 position, float runTime);

	//エフェクトインスタンス生成/削除
	void createSelectLight(D3DXVECTOR3* position);
	void deleteSelectLight();


	//削除
	void autoDestroy();



};

