//===================================================================================================================================
//【Photograph.h】
// [作成者]HAL東京GP12A332 21 新里 将士
// [作成日]2019/12/26
// [更新日]2019/12/26
//===================================================================================================================================
#pragma once

#ifdef _DEBUG
//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include <vector>
#include "AbstractScene.h"
#include "TmpObject.h"
#include "StaticMeshRenderer.h"
#include "Stone.h"
#include "DeadTree.h"
#include "TreeTypeA.h"
#include "TreeTypeB.h"
#include "TestEffect.h"
#include "EnemyManager.h"
#include "AIDirector.h"
#include "NavigationMesh.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Sound.h"

#include "EnemyTools.h"
#include "ItemTools.h"
#include "TreeTools.h"
#include "MapObjectTool.h"

#include "EffekseerManager.h"

//===================================================================================================================================
//【マクロ】
//===================================================================================================================================
#define START_GUI_ID	(1)
#define END_GUI_ID		(2)
#define CURVE_GUI_ID	(3)
#define MAX_CURVE_POINT (1)

//===================================================================================================================================
//【構造体】
//===================================================================================================================================
typedef struct
{
	short pointMax;
}CURVE_POINT;


//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum POINT_TYPE
{
	POINT_NONE,
	POINT_START,
	POINT_END,
	POINT_CURVE,
	POINT_MAX
};

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace photographNS
{
	//=============================================================================
	// スタートポイントエフェクト
	//=============================================================================
	class StartEffect :public effekseerNS::Instance
	{
	public:
		const D3DXVECTOR3* syncPosition;
		StartEffect(int no, const D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::DROP_ITEM;
			position = *syncPosition;
		}
		virtual void update()
		{
			Instance::update();
		};
	};

	//=============================================================================
	// エンドポイントエフェクト
	//=============================================================================
	class EndEffect :public effekseerNS::Instance
	{
	public:
		const D3DXVECTOR3* syncPosition;
		EndEffect(int no, const D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::DROP_ITEM;
			position = *syncPosition;
		}
		virtual void update()
		{
			Instance::update();
		}
	};

	//=============================================================================
	// カーブポイントエフェクト
	//=============================================================================
	class CurveEffect :public effekseerNS::Instance
	{
	public:
		const D3DXVECTOR3* syncPosition;
		CurveEffect(int no, const D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::WIND;
			position = *syncPosition;
		}
		virtual void update()
		{
			Instance::update();
		}

	};

}


//===================================================================================================================================
//【クリエイトムービークラス】
//===================================================================================================================================
class Photograph : public AbstractScene
{
private:

	photographNS::StartEffect* startEffect;
	photographNS::EndEffect* endEffect;
	photographNS::CurveEffect* curveEffect;


	//俯瞰カメラ
	Camera* topView;
	bool onTopView;			//カメラスイッチ
	//仮オブジェクト
	TmpObject* tmpObject;
	StaticMeshRenderer* tmpObjRenderer;
	//確認用オブジェクト
	Object* cameraObject;
	StaticMeshRenderer* cameraObjectRenderer;
	//フィールド
	Object* testField;
	StaticMeshRenderer* testFieldRenderer;
	//ターゲット
	Object* target;
	//撮影カメラ確認用
	//Object* cameraObject;
	//StaticMeshRenderer* cameraObjectRenderer;

	//エネミーツール
	ENEMY_TOOLS *enemyTools;




	int stateCamera;						//カメラステータス
	float frameDegree = 1.0f / 10.0f;		//自動速度
	float inputDegree = 2.0f;				//入力速度

	float moveTime;				//終点までの時間
	float moveTimer;			//移動タイマー
	D3DXVECTOR3 startPos;		//ラープ始点


	float rate;
	float rateY;
	float rateX;
	float degreeY;
	float degreeX;
	float degreeTimer;
	float degreeTime;
	float distance;
	float moveDistance;

	//カメラの軸取得変数
	D3DXVECTOR3 cameraAxisX;			//カメラ回転軸X
	D3DXVECTOR3 cameraAxisY;			//カメラ回転軸Y
	D3DXVECTOR3 cameraAxisZ;			//カメラ回転軸Z
	D3DXVECTOR3 fixedAxisX;				//固定X軸
	D3DXVECTOR3 fixedAxisZ;				//固定Z軸
	D3DXQUATERNION tmpCameraQ;			//カメラの相対位置ベクトルの一時保存
	D3DXVECTOR3 BezierCurveS1;			//S字ベジェ曲線点１
	D3DXVECTOR3 BezierCurveS2;			//S字ベジェ曲線点２
	D3DXVECTOR3 targetDistance;
	D3DXVECTOR3 newRelative;
	bool measurement = true;

	D3DXVECTOR3 startPoint;
	D3DXVECTOR3 endPoint;
	D3DXVECTOR3 curvePoint;

	float photoTime = 5.0f;
	float photoTimer = 5.0f;
	//float rate;
	bool bezier = false;

public:
	Photograph();
	~Photograph();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	void render3D(Camera currentCamera);
	void renderUI();

	int PointToolsListboxType;
	int meshId;

	virtual void createGUI() override;
	void toolsGUI();
	
	
	void outputStartPointGUI(int GUIid,  D3DXVECTOR3* pos);
	void outputEndPointGUI(int GUIid, D3DXVECTOR3* pos);
	void outputCurvePointGUI(int GUIid, D3DXVECTOR3* pos);
	
	void addCurvePoint(D3DXVECTOR3* pos);

	static D3DXVECTOR3 setBezier(D3DXVECTOR3 startpoint, D3DXVECTOR3 curvepoint, D3DXVECTOR3 endpoint, float rate);

};

#endif
