//===================================================================================================================================
//【Title.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
//#include "UITitle.h"
//#include "SceneEffect.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace titleNS
{
	enum PLAYER_TYPE
	{
		PLAYER_1,
		PLAYER_2,
		PLAYER_TYPE_MAX
	};
	
	static const float EFFECT_MAX = 1500;	// エフェクト上限

	// プレイヤー座標
	const D3DXVECTOR3 PLAYER_POSITION[titleNS::PLAYER_TYPE::PLAYER_TYPE_MAX] =
	{
		D3DXVECTOR3(-8.0f, 101.0f, 33.0f),
		D3DXVECTOR3(-15.0f, 95.0f, 30.0f)
	};
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION[titleNS::PLAYER_TYPE::PLAYER_TYPE_MAX] =
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
//【タイトルシーンクラス】
//===================================================================================================================================
class Title : public AbstractScene
{
private:
	//Player player[titleNS::PLAYER_TYPE::PLAYER_TYPE_MAX];	//	プレイヤー
	//SceneEffect sceneEffect;								//	シーンエフェクト
	//UITitle uiTitle;										//	タイトルUI
	//InstancingBillboard plane;								//	インスタンシングビルボード
public:
	Title(void);
	~Title(void);
	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	void updateInput(void);
	virtual void render() override;
	//void render3D(Camera _currentCamera);
	void render2D();
	virtual void collisions(void) override;
	virtual void AI(void) override;

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};