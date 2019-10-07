//============================================================================================================================================
// Document
//============================================================================================================================================
// Animation.h
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/09/03
//============================================================================================================================================
#pragma once
#include "Base.h"
#include "AllocateHierarchy.h"
//============================================================================================================================================
// NameSpace
// 名前空間
//============================================================================================================================================
namespace animationNS
{
	const float TIME_PER_FRAME = (0.0167f);

	enum TYPE
	{
		IDLE_GENERAL,
		IDLE,
		SHOOTING,
		RUN,
		RUN_FAST,
		JUMP,
		INSTALLATION,
		STANDING,
		SLASH,
		DOWN,
		DOWN_POSE,
		REVIVAL,
		RECURSION,
		FLOATING,
		FALLING,
		LANDING,
		TYPE_MAX
	};
}
//============================================================================================================================================
// Structure
// 構造体
//============================================================================================================================================
struct AnimationManager
{
	ID3DXAnimationSet* animationSet;	//	アニメーションセット
	LPCSTR animationSetName;			//	アニメーションセットの名前
	float shiftTime;					//	シフトするのに必要な時間
	float weightTime;					//	現在のウェイト時間
};
struct AnimationFlag
{
	bool animationOn;		//	アニメーションの再生可否フラグ
	bool animationPlayEnd;	//	アニメーションの再生終了フラグ
};
struct AnimationID
{
	UINT current;	//	今
	UINT next;		//	次
};
struct Animation
{
	AnimationManager* animationManager;				//	アニメーションセットの管理
	LPD3DXANIMATIONCONTROLLER animationController;	//	アニメーションコントローラ
	AllocateHierarchy* allocateHierarchy;			//	Xファイルの情報
	LPD3DXFRAME rootFrame;							//	ルートフレーム( フレーム階層構造の最上位フレーム )
	AnimationFlag flag;								//	アニメーションフラグ
	AnimationID animationID;						//	アニメーションID
	UINT animationIdCurrent;						//	現在、再生しているアニメーションID
	UINT animationIdPast;							//	過去、再生していたアニメーションID
	int animationSetMax;							//	アニメーションセットの最大数
	int keyFrameCount;								//	コールバック( Key_Frame )の処理数
	float animationSpeed;							//	アニメーション再生速度
	HRESULT(*initialize)(Animation* _animation, LPCSTR _setName, int _setNo);
	void(*release)(Animation* _animation);
	void(*update)(Animation* _animation, float _time);
	void(*render)(LPDIRECT3DDEVICE9 _device, Animation* _animation, LPD3DXMATRIX _worldMatrix);
	void(*switching)(Animation* _animation, UINT _animationIdentification);
	void(*setShiftTime)(Animation* _animation, UINT _animationIdentification, float _interval);
};
struct CallBackAnimation : public ID3DXAnimationCallbackHandler
{
	Animation* animation;														//	アニメーション
	LPCSTR setName;																//	セット名
	int status;																	//	ステータス
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);	//	コールバックハンドル
};
//============================================================================================================================================
// Prototype Declaration
// プロトタイプ宣言
//============================================================================================================================================
HRESULT initialize(Animation* _animation, LPCSTR _setName, int _setNo);
void release(Animation* _animation);
void update(Animation* _animation, float _time);
void updateFrameMatrix(LPDIRECT3DDEVICE9* _device, LPD3DXFRAME _baseFrame, LPD3DXMATRIX _parentMatrix);
void render(LPDIRECT3DDEVICE9 _device, Animation* _animation, LPD3DXMATRIX _worldMatrix);
void renderFrame(LPDIRECT3DDEVICE9 _device, Animation* _animation, LPD3DXFRAME _frame);
void renderMeshContainer(LPDIRECT3DDEVICE9 _device, LPD3DXMESHCONTAINER _baseMeshContainer, LPD3DXFRAME _baseFrame);
void switching(Animation* _animation, UINT _animationIdentification);
void switchingSpeed(Animation* _animation);
Animation* createObject(void);
HRESULT loadXFile(LPDIRECT3DDEVICE9 _device, Animation* _animation, LPCTSTR _fileName);
D3DXFrameDerived* searchBoneFrame(Animation* _animation, const char* _boneName, D3DXFRAME* _frame);
void setShiftTime(Animation* _animation, UINT _animationIdentification, float _interval);
HRESULT setBoneMatrix(LPD3DXFRAME _baseFrame, LPD3DXFRAME _rootFrame);
HRESULT setCallBackKeyFrame(Animation* _animation, LPCSTR _setName);
D3DXMATRIX getBoneMatrix(Animation* _animation, const char* _boneName);