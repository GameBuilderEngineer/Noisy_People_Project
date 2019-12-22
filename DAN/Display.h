//===================================================================================================================================
//【Display .h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/12/10
// [更新日]2019/12/10
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
#include "StaticMeshRenderer.h"
#include "TreeManager.h"
#include "networkServer.h"
#include "Sky.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace DisplayNS
{
	const D3DXQUATERNION	CAMERA_RELATIVE_QUATERNION	= D3DXQUATERNION(0.0f,300.0f, 50.0f, 0.0f);
	const float				CAMERA_SPEED				= 8.0f;


	const int				SERVER_MODE					= 0x00000001;
	const int				CLIENT_MODE					= 0x00000002;
}

//===================================================================================================================================
//【ディスプレイシーンクラス】
//===================================================================================================================================
class Display : public AbstractScene
{
private:
	//フィールド
	Object*							testField;			//フィールド
	StaticMeshRenderer*				testFieldRenderer;	//フィールドレンダラー
	NETWORK_INTERFACE*				networkServer;
	TreeManager*					treeManager;		//ツリー
	Sky*							sky;				//スカイドーム


	float							syncTimer;			//同期タイマー

	int								testMode;			//テスト用モード切替

public:
	Display();
	~Display();

	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	virtual void render() override;
	virtual void collisions(void) override;
	virtual void AI(void) override;

	void render3D(Camera* currentCamera);
	//void renderUI();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};

