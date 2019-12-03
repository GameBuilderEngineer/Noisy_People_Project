//===================================================================================================================================
//【TmpObject.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/24
// [更新日]2019/10/18
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "StaticMeshRenderer.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace tmpObjNS {

	enum TMPOBJ_LIST
	{
		TMPOBJ_PLAYER,
		TMPOBJ_WOLF,
		TMPOBJ_TIGER,
		TMPOBJ_BEAR,
		TMPOBJ_BATTERY,
		TMPOBJ_EXAMPLE,
		TMPOBJ_MAX
	};

	struct OperationKeyTable
	{
		BYTE front;
		BYTE back;
		BYTE right;
		BYTE left;
		BYTE reset;
		BYTE spin;
		BYTE down;
		BYTE fly;
	};

	const OperationKeyTable KEY_TABLE_1P = {
		'W',					//FRONT
		'S',					//BACK
		'D',					//RIGHT
		'A',					//LEFT
		'R',					//RESET
		'E',					//SPIN
		VK_LSHIFT,				//DASH
		VK_SPACE,				//JUMP
	};

	const OperationKeyTable NON_CONTOROL = {
		VK_ESCAPE,		//FRONT
		VK_ESCAPE,		//BACK
		VK_ESCAPE,		//RIGHT
		VK_ESCAPE,		//LEFT
		VK_ESCAPE,		//RESET
		VK_ESCAPE,		//DASH
		VK_ESCAPE,		//JUMP
	};

	const BYTE BUTTON_JUMP = virtualControllerNS::B;
	const BYTE BUTTON_BULLET = virtualControllerNS::R1;
	const BYTE BUTTON_CUT = virtualControllerNS::X;
	const BYTE BUTTON_REVIVAL = virtualControllerNS::A;
	const BYTE BUTTON_PILE = virtualControllerNS::L1;

	const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(-115, 40, 0);

	// Physics
	const float MOVE_ACC = 27.0f*1.5f;								// 移動加速度
	const float STOP_SPEED = 1.0f;									// 移動停止速度
	const float FALL_SPEED_MAX = 60.0f;								// 落下最高速度
	const float JUMP_SPEED = 6.0f;									// ジャンプ初速
	const float JUMP_CONTROL_SPEED = 1.0f;							// ジャンプ高さコントール速度
	const float DASH_MAGNIFICATION = 2.0f;							// ダッシュ倍率
	// Another
	const float DIFFERENCE_FIELD = 0.05f;							// フィールド補正差分
	const float CAMERA_SPEED = 1.0f;								// カメラの速さ
}


//===================================================================================================================================
//【クラス定義】
//===================================================================================================================================
class TmpObject : public Object
{
private:
	//ステータス
	int							type;							//プレイヤータイプ
	tmpObjNS::OperationKeyTable	keyTable;						//操作Keyテーブル
	int							state;							//状態変数

	//操作
	float						reverseValueXAxis;				//操作X軸
	float						reverseValueYAxis;				//操作Y軸
	bool							isExecutingMoveOperation;		// 移動操作中フラグ
	bool							onFly;							// 飛ぶフラグ
	bool							jumping;							// ジャンプ中フラグ

	// 衝突
	BoundingSphere				bodyCollide;						// 球コリジョン
	float						difference;						//フィールド補正差分
	bool						onGround;						//接地判定

	// 汎用
	LPDIRECT3DDEVICE9			device;						// Direct3Dデバイス
	Input*						input;						// 入力系
	Camera*						camera;						// 操作するカメラへのポインタ
	D3DXVECTOR3					centralPosition;				// 中心座標
	D3DXMATRIX					centralMatrixWorld;			// 中心座標ワールドマトリクス

	// その他
	int							staticMeshNo;				// メッシュのID
	//int							ItemListboxMesh;				// メッシュの種類(リストボックス)
	
	D3DXVECTOR3					PositionYeah;

public:
	TmpObject();
	~TmpObject();

	// 基本処理
	virtual void initialize(int playerType, int modelType);
	virtual void update(float frameTime);
	void otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// 物理
	void configurationGravityWithRay(D3DXVECTOR3* attractorPosition,
		LPD3DXMESH attractorMesh, D3DXMATRIX* attractorMatrix);
	void updatePhysics(float frameTime);							// 物理の更新

	//操作
	void moveOperation();											// 移動操作
	void controlCamera(float frameTime);							// カメラ操作
	bool grounding(LPD3DXMESH mesh, D3DXMATRIX matrix);				//接地処理

	// アクション
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);//移動

	// その他
	virtual void outputGUI() override;							// ImGUI
	void reset();												// リセット
	void resetMesh(int meshId);									// メッシュのリセット

	//setter
	void setCamera(Camera* _camera);							//操作対象カメラのセット

	//getter
	int getState();
	BoundingSphere* getBodyCollide();							//球コリジョンの取得
	D3DXVECTOR3*	getPositionYeah() { return&PositionYeah; };
};