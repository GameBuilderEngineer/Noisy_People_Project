//===================================================================================================================================
//【Finale.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/12/12
// [更新日]2019/12/12
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
#include "UtilityFunction.h"
#include "Object.h"
#include "StaticMeshRenderer.h"
#include "Sky.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace FinaleNS
{
	//プレイヤー初期位置
	const D3DXVECTOR3 PLAYER_POSITION = D3DXVECTOR3(0, 100, 0);

	//カメラ相対位置
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f, 0.0f, -1.5f, 0.0f);

	//カメラ相対注視位置
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===================================================================================================================================
//【フィナーレシーンクラス】
//===================================================================================================================================
class Finale : public AbstractScene
{
private:

	//フィールド
	Object*					finaleField;
	StaticMeshRenderer*		finaleFieldRenderer;
	//スカイドーム
	Sky* sky;
	//ターゲットオブジェクト
	Object* target;

	int stateCamera;						//カメラステータス
	float frameDegree = 1.0f / 10.0f;		//自動速度
	float inputDegree = 2.0f;				//入力速度

	//カメラの軸取得変数
	D3DXVECTOR3 cameraAxisZ;
	D3DXVECTOR3 fixedAxisZ;
	D3DXVECTOR3 cameraAxisX;
	D3DXVECTOR3 cameraAxisY;
	D3DXVECTOR3 fixedAxisX;
	D3DXQUATERNION tmpCameraQ;//カメラの相対位置ベクトルの一時保存
	D3DXVECTOR3 BezierCurveS1;
	D3DXVECTOR3 BezierCurveS2;

	D3DXQUATERNION cameraRelativeQuaternion;

public:
	Finale();
	~Finale();
	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	virtual void render() override;
	void render3D(Camera _currentCamera);
	void render2D();
	virtual void collisions(void) override;
	virtual void AI(void) override;
#ifdef _DEBUG
	virtual void createGUI() override;
#endif

};

