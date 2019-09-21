//===================================================================================================================================
//【Operation.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
//#include "KeyOpe.h"
//#include "PadOpe.h"
//#include "SceneEffect.h"

//===================================================================================================================================
//【マクロ定義】
//===================================================================================================================================
#define OPE_MAX (1)		// オペレーション移動限界

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace operationNS
{
	static const float EFFECT_MAX = 1500;	// エフェクト上限

	enum PLAYER_TYPE
	{
		PLAYER_1,
		PLAYER_2,
		PLAYER_TYPE_MAX
	};
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION[operationNS::PLAYER_TYPE::PLAYER_TYPE_MAX] =
	{
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f),
#ifdef _DEBUG
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f)
#else
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f)
#endif // _DEBUG
	};
}

//===================================================================================================================================
//【操作方法説明シーンクラス】
//===================================================================================================================================
class Operation : public AbstractScene
{
private:
	// キーボード説明
	//KeyOpe keyOpe;
	// パッド説明
	//PadOpe padOpe;
	//SceneEffect sceneEffect;//	シーンエフェクト

public:
	Operation();
	~Operation();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	//void render3D(Camera _currentCamera);
	void renderUI(LPDIRECT3DDEVICE9 device);
	int opeTransition;	// オペレーション画像入れ替え

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};