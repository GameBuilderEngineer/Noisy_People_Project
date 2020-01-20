//===================================================================================================================================
//【PlayEffect.h】
// [作成者] HAL東京GP12A332 11 菅野 樹
// [作成日] 2019/12/23
// [更新日] 2019/12/23
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "EffekseerManager.h"
#include "Object.h"
#include "Camera.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace PlayerEffectNS
{

	class DigitMode :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		DigitMode(D3DXMATRIX* sync, int no = 0) {
			syncMatrix = sync;
			managerNo = no;
			effectNo = effekseerNS::DIGIT_MODE;
			scale = D3DXVECTOR3(0.25f, 0.25f, 0.25f);
		}
		virtual void update() {
			D3DXMATRIX M = *syncMatrix;

			::Effekseer::Manager*	manager = getEffekseerManager(managerNo)->manager;
			::Effekseer::Matrix43 matrix;
			matrix.Value[0][0] = M._11*scale.x;
			matrix.Value[0][1] = M._12;
			matrix.Value[0][2] = M._13;
			matrix.Value[1][0] = M._21;
			matrix.Value[1][1] = M._22*scale.y;
			matrix.Value[1][2] = M._23;
			matrix.Value[2][0] = M._31;
			matrix.Value[2][1] = M._32;
			matrix.Value[2][2] = M._33*scale.z;
			matrix.Value[3][0] = M._41;
			matrix.Value[3][1] = M._42;
			matrix.Value[3][2] = M._43;
			manager->SetScale(handle, scale.x, scale.y, scale.z);
			manager->SetMatrix(handle, matrix);
		};
	};

	class DigitalShiftEffect :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		DigitalShiftEffect(D3DXMATRIX* sync, int no = 0) {
			syncMatrix = sync;
			managerNo = no;
			effectNo = effekseerNS::DIGITAL_SHIFT;
			scale = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
		}
		virtual void update() {
			D3DXMATRIX M = *syncMatrix;

			::Effekseer::Manager*	manager = getEffekseerManager(managerNo)->manager;
			::Effekseer::Matrix43 matrix;
			matrix.Value[0][0] = M._11;
			matrix.Value[0][1] = M._12;
			matrix.Value[0][2] = M._13;
			matrix.Value[1][0] = M._21;
			matrix.Value[1][1] = M._22;
			matrix.Value[1][2] = M._23;
			matrix.Value[2][0] = M._31;
			matrix.Value[2][1] = M._32;
			matrix.Value[2][2] = M._33;
			matrix.Value[3][0] = M._41;
			matrix.Value[3][1] = M._42;
			matrix.Value[3][2] = M._43;
			manager->SetScale(handle, scale.x, scale.y, scale.z);
			manager->SetMatrix(handle, matrix);
		};
	};

	class SkyJumpEffect :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		D3DXVECTOR3* syncPosition;
		SkyJumpEffect(D3DXMATRIX* sync, D3DXVECTOR3* syncPosition, int no = 0) {
			syncMatrix = sync;
			this->syncPosition = syncPosition;
			managerNo = no;
			effectNo = effekseerNS::SKY_JUMP;
			scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
		virtual void update() {
			::Effekseer::Manager*	manager = getEffekseerManager(managerNo)->manager;
			position = *syncPosition;
			position.y -= 2.0f;
			Instance::update();
		};
	};

	class SkyVisionEffect :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		D3DXVECTOR3* syncPosition;
		SkyVisionEffect(D3DXMATRIX* sync, D3DXVECTOR3* syncPosition, int no = 0) {
			syncMatrix = sync;
			this->syncPosition = syncPosition;
			managerNo = no;
			effectNo = effekseerNS::SKY_JUMP;
			scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
		}
		virtual void update() {
			D3DXMATRIX M = *syncMatrix;
			::Effekseer::Manager*	manager = getEffekseerManager(managerNo)->manager;
			position = *syncPosition;
			position.y -= 2.0f;
			Instance::update();
		};
	};

	class ShiftTerminateEffect :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		ShiftTerminateEffect(D3DXMATRIX* sync, int no = 0) {
			syncMatrix = sync;
			managerNo = no;
			effectNo = effekseerNS::SHIFT_TERMINATE;
			scale = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
		}
		virtual void update() {
			D3DXMATRIX M = *syncMatrix;

			::Effekseer::Manager*	manager = getEffekseerManager(managerNo)->manager;
			::Effekseer::Matrix43 matrix;
			matrix.Value[0][0] = M._11;
			matrix.Value[0][1] = M._12;
			matrix.Value[0][2] = M._13;
			matrix.Value[1][0] = M._21;
			matrix.Value[1][1] = M._22;
			matrix.Value[1][2] = M._23;
			matrix.Value[2][0] = M._31;
			matrix.Value[2][1] = M._32;
			matrix.Value[2][2] = M._33;
			matrix.Value[3][0] = M._41;
			matrix.Value[3][1] = M._42;
			matrix.Value[3][2] = M._43;
			manager->SetScale(handle, scale.x, scale.y, scale.z);
			manager->SetMatrix(handle, matrix);
		};
	};

	//class DigitaTreeNearEffect :public effekseerNS::Instance
	//{
	//public:
	//	D3DXMATRIX* syncMatrix;
	//	DigitaTreeNearEffect(D3DXMATRIX* sync, int no = 0) {
	//		syncMatrix = sync;
	//		managerNo = no;
	//		effectNo = effekseerNS::DIGIT_TREE_NEAR;
	//		scale = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
	//	}
	//	virtual void update() {
	//		D3DXMATRIX M = *syncMatrix;

	//		::Effekseer::Manager*	manager = getEffekseerManager(managerNo)->manager;
	//		::Effekseer::Matrix43 matrix;
	//		matrix.Value[0][0] = M._11;
	//		matrix.Value[0][1] = M._12;
	//		matrix.Value[0][2] = M._13;
	//		matrix.Value[1][0] = M._21;
	//		matrix.Value[1][1] = M._22;
	//		matrix.Value[1][2] = M._23;
	//		matrix.Value[2][0] = M._31;
	//		matrix.Value[2][1] = M._32;
	//		matrix.Value[2][2] = M._33;
	//		matrix.Value[3][0] = M._41;
	//		matrix.Value[3][1] = M._42;
	//		matrix.Value[3][2] = M._43;
	//		manager->SetScale(handle, scale.x, scale.y, scale.z);
	//		manager->SetMatrix(handle, matrix);
	//	};
	//};

	enum EFFECT_LIST
	{
		DIGIT_MODE,
		DIGHITAL_SHIFT,
		SKY_JUMP,
		SKY_VISION,
		SHIFT_TERMINATE,
	};

}

//===================================================================================================================================
//【プレイヤーエフェクトクラス】
//===================================================================================================================================
class PlayerEffect : public Base
{
private:
	effekseerNS::Instance* digitMode;
	effekseerNS::Instance* digitalShift;
	effekseerNS::Instance* skyJump;
	effekseerNS::Instance* skyVision;
	effekseerNS::Instance* shiftTerminate;

	Object* player;
	D3DXVECTOR3* skyPosition;
	int managerNo;

public:
	PlayerEffect(int managerNo);
	~PlayerEffect();

public:
	float distanceFlower;
	float distanceFeather;
	void setPointer(Object* player, D3DXVECTOR3* skyPosition);
	void update(float frameTime);

	void play(int effectNo);
	void stop(int effectNo);

};
