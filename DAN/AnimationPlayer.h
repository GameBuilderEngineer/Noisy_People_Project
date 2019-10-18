//============================================================================================================================================
// Document
//============================================================================================================================================
// AnimationPlayer.h
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/09/07
//============================================================================================================================================
#pragma once
#include "Base.h"
#include "Input.h"
#include "Sound.h"
#include "Animation.h"
#include "StaticMeshLoader.h"
//============================================================================================================================================
// NameSpace
// 名前空間
//============================================================================================================================================
namespace animationPlayerNS
{
	enum PLAYER_TYPE
	{
		PLAYER_1,
		PLAYER_2,
		PLAYER_TYPE_MAX
	};
	enum MODEL_TYPE
	{
		ADAM,
		EVE,
		MODEL_TYPE_MAX
	};
	enum SCENE_TYPE
	{
		TITLE,
		CHARACTER_SELECT,
		GAME,
		SCENE_TYPE_MAX
	};
	enum STATE_TYPE
	{
		GROUND,
		DOWN,
		FALL,
		SKY,
		REVIVAL,
		STATE_TYPE_MAX
	};
	struct OperationKeyTable
	{
		BYTE front;
		BYTE back;
		BYTE right;
		BYTE left;
		BYTE reset;
		BYTE dash;
		BYTE jump;
		BYTE cameraX;
		BYTE cameraY;
		BYTE provisional;
		BYTE revival;
	};
	struct Flag
	{
		bool moveBan;		//	移動禁止フラグ
		bool gunRenderFlag;	//	銃の描画フラグ
		bool jump;			//	ジャンプフラグ
		bool installation;	//	メモリー・パイル設置フラグ
		bool slash;			//	メモリー・ライン切断フラグ
		bool recursion;		//	リカージョンフラグ
		bool falling;		//	落下フラグ
		bool landing;		//	着地フラグ
		bool revival;		//	復活フラグ
	};
	const OperationKeyTable KEY_TABLE_PLAYER_1 =
	{
		'W',		//	FRONT
		'S',		//	BACK
		'D',		//	RIGHT
		'A',		//	LEFT
		'R',		//	RESET
		VK_LSHIFT,	//	DASH
		VK_SPACE,	//	JUMP
		VK_F3,		//	CameraAxisX
		VK_F4,		//	CameraAxisY
		'G',		//	Provisional
		VK_SPACE,	//	Revival
	};
	const OperationKeyTable KEY_TABLE_PLAYER_2 =
	{
		VK_UP,		//	FRONT
		VK_DOWN,	//	BACK
		VK_RIGHT,	//	RIGHT
		VK_LEFT,	//	LEFT
		'R',		//	RESET
		VK_LSHIFT,	//	DASH
		VK_RETURN,	//	JUMP
		VK_F5,		//	CameraAxisX
		VK_F6,		//	CameraAxisY
		'T',		//	Provisional
		VK_SPACE,	//	Revival
	};
	const OperationKeyTable KEY_TABLE_NONE =
	{
		VK_ESCAPE,	//	FRONT
		VK_ESCAPE,	//	BACK
		VK_ESCAPE,	//	RIGHT
		VK_ESCAPE,	//	LEFT
		VK_ESCAPE,	//	RESET
		VK_ESCAPE,	//	DASH
		VK_ESCAPE,	//	JUMP
		VK_ESCAPE,	//	CameraAxisX
		VK_ESCAPE,	//	CameraAxisY
		VK_ESCAPE,	//	Provisional
		VK_ESCAPE,	//	Revival
	};
	enum AI_ANIMATION_FALG
	{
		INSTALLATION,
		SLASH,
		MOVE,
		SHOOTING,
		AI_FLAG_MAX
	};
}
//============================================================================================================================================
// Class
// クラス
//============================================================================================================================================
class AnimationPlayer
{
private:
	animationPlayerNS::OperationKeyTable keyTable;	//	操作Keyテーブル
	animationPlayerNS::Flag flag;					//	フラグ
	bool aiAnimFlag[animationPlayerNS::AI_FLAG_MAX];//  AI用のアニメーション使用追加フラグ
	Animation* animation;							//	アニメーション
	AnimationID animationID;						//	アニメーションID
	int playerIndex;								//	プレイヤーインデックス
	int modelType;									//  モデルタイプ
public:
	AnimationPlayer(void);
	~AnimationPlayer(void);
	HRESULT initialize(LPDIRECT3DDEVICE9 _device, int _playerIndexintl, int _modelType);
	void release(void);
	void update(Input* _input, int _state);
	void updateAnimationID(Input* _input, int _state);
	void updateAnimationIDCurrentIdle(Input* _input, int _state);
	void updateAnimationIDCurrentShooting(Input* _input, int _state);
	void updateAnimationIDCurrentRun(Input* _input, int _state);
	void updateAnimationIDCurrentRunFast(Input* _input, int _state);
	void updateAnimationIDCurrentJump(Input* _input, int _state);
	void updateAnimationIDCurrentInstallation(Input* _input, int _state);
	void updateAnimationIDCurrentStanding(Input* _input, int _state);
	void updateAnimationIDCurrentSlash(Input* _input, int _state);
	void updateAnimationIDCurrentDown(Input* _input);
	void updateAnimationIDCurrentDownPose(Input* _input);
	void updateAnimationIDCurrentRevival(Input* _input, int _state);
	void updateAnimationIDCurrentRecursion(Input* _input);
	void updateAnimationIDCurrentFloating(Input* _input);
	void updateAnimationIDCurrentFalling(Input* _input);
	void updateAnimationIDCurrentLanding(Input* _input, int _state);
	void updateAnimationIDNextIdle(void);
	void updateAnimationIDNextShooting(void);
	void updateAnimationIDNextRun(void);
	void updateAnimationIDNextRunFast(void);
	void updateAnimationIDNextJump(void);
	void updateAnimationIDNextInstallation(void);
	void updateAnimationIDNextStanding(void);
	void updateAnimationIDNextSlash(void);
	void updateAnimationIDNextDown(void);
	void updateAnimationIDNextDownPose(void);
	void updateAnimationIDNextRevival(void);
	void updateAnimationIDNextRecursion(void);
	void updateAnimationIDNextFloating(void);
	void updateAnimationIDNextFalling(void);
	void updateAnimationIDNextLanding(void);
	void updateTitle(void);
	void updateAnimationIDCurrentIdleGeneral(void);
	void updateAnimationIDNextIdleGeneral(void);
	void render(LPDIRECT3DDEVICE9 _device, D3DXMATRIX _matrixRotaition, D3DXMATRIX _matrixPosition, StaticMeshLoader* _staticMeshLoader);
	void resetAnimation(void);
	// Set
	void setAnimationConfiguration(int _scene);
	void setFlagMoveBan(bool _flag) { flag.moveBan = _flag; }
	void setFlagGunRender(bool _flag) { flag.gunRenderFlag = _flag; }
	void setFlagJump(bool _flag) { flag.jump = _flag; }
	void setFlagInstallation(bool _flag) { flag.installation = _flag; }
	void setFlagSlash(bool _flag) { flag.slash = _flag; }
	void setFlagRecursion(bool _flag) { flag.recursion = _flag; }
	void setFlagFalling(bool _flag) { flag.falling = _flag; }
	void setFlagLanding(bool _flag) { flag.landing = _flag; }
	void setFlagRevival(bool _flag) { flag.revival = _flag; }
	void setAIAnimFlag(animationPlayerNS::AI_ANIMATION_FALG type, bool _flag) { aiAnimFlag[type] = _flag; }
	// Get
	Animation* getAnimation(void) { return animation; }
	bool getFlagMoveBan(void) { return flag.moveBan; }
	bool getFlagGunRender(void) { return flag.gunRenderFlag; }
	bool getFlagJump(void) { return flag.jump; }
	bool getFlagInstallation(void) { return flag.installation; }
	bool getFlagSlash(void) { return flag.slash; }
	bool getFlagRecursion(void) { return flag.recursion; }
	bool getFlagFalling(void) { return flag.falling; }
	bool getFlagLanding(void) { return flag.landing; }
	bool getFlagRevival(void) { return flag.revival; }
};