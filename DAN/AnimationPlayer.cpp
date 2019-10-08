//============================================================================================================================================
// Document
//============================================================================================================================================
// AnimationPlayer.cpp
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/09/07
//============================================================================================================================================
#include "AnimationPlayer.h"
//============================================================================================================================================
// Using Declaration
// using宣言
//============================================================================================================================================
using namespace animationPlayerNS;
//============================================================================================================================================
// Global Variable
// グローバル変数
//============================================================================================================================================
bool initializeFlag = false;
//============================================================================================================================================
// Constructor
// コンストラクタ
//============================================================================================================================================
AnimationPlayer::AnimationPlayer(void)
{
	animation = NULL;				//	アニメーション
	animationID = { NULL,NULL };	//	アニメーションID
	flag.moveBan = false;			//	移動禁止フラグ

	return;
}
//============================================================================================================================================
// Destructor
// デストラクタ
//============================================================================================================================================
AnimationPlayer::~AnimationPlayer(void)
{
	return;
}
//============================================================================================================================================
// initialize
// 初期化
//============================================================================================================================================
HRESULT AnimationPlayer::initialize(LPDIRECT3DDEVICE9 _device, int _playerIndex, int _modelType)
{
	// 変数に代入
	playerIndex = _playerIndex;
	modelType = _modelType;

	// キーテーブルの設定
	switch (_playerIndex)
	{
	case PLAYER_TYPE::PLAYER_1:
		keyTable = KEY_TABLE_PLAYER_1;
		break;
	case PLAYER_TYPE::PLAYER_2:
		keyTable = KEY_TABLE_PLAYER_2;
		break;
	default:
		keyTable = KEY_TABLE_NONE;
		break;
	}

	// AIアニメーションフラグの初期化
	for (int i = 0; i < AI_FLAG_MAX; i++)
	{
		aiAnimFlag[i] = false;
	}

	// アニメーションセット名
	const char* animationSetName[animationNS::TYPE::TYPE_MAX] =
	{
		"IdleGeneral",
		"Idle",
		"Shooting",
		"Run",
		"RunFast",
		"Jump",
		"Installation",
		"Standing",
		"Slash",
		"Down",
		"DownPose",
		"Revival",
		"Recursion",
		"Floating",
		"Falling",
		"Landing"
	};

	// アニメーションを作成
	animation = createObject();

	// ディレクトリ設定
	setVisualDirectory();

	// Xファイルを読み込む
	switch (_modelType)
	{
	case MODEL_TYPE::ADAM:
		loadXFile(_device, animation, ("Character_Adam.x"));
		break;
	case MODEL_TYPE::EVE:
		loadXFile(_device, animation, ("Character_Eve.x"));
		break;
	default:
		break;
	}

	// アニメーションコールバックの設定
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::JUMP]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::INSTALLATION]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::STANDING]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::SLASH]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::DOWN]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::REVIVAL]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::LANDING]);

	// アニメーションセットの初期化
	for (int i = 0; i < animation->animationSetMax; i++)
	{
		animation->initialize(animation, animationSetName[i], i);
	}

	// パラメータ初期化
	animation->animationSpeed = 1.0f;

	// アニメーションシフトタイムの初期化
	animation->setShiftTime(animation, animationNS::TYPE::IDLE_GENERAL, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::IDLE, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::SHOOTING, 0.1f);
	animation->setShiftTime(animation, animationNS::TYPE::RUN, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::RUN_FAST, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::JUMP, 0.5f);
	animation->setShiftTime(animation, animationNS::TYPE::INSTALLATION, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::STANDING, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::SLASH, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::DOWN, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::DOWN_POSE, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::REVIVAL, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::RECURSION, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::FLOATING, 1.0f);
	animation->setShiftTime(animation, animationNS::TYPE::FALLING, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::LANDING, 0.25f);

	return S_OK;
}
//============================================================================================================================================
// release
// 解放
//============================================================================================================================================
void AnimationPlayer::release(void)
{
	if (playerIndex == animationPlayerNS::PLAYER_2)
	{
		int unko = 9;
	}

	// アニメーションの解放
	animation->release(animation);
	SAFE_DELETE(animation);

	return;
}
//============================================================================================================================================
// update
// 更新
//============================================================================================================================================
void AnimationPlayer::update(Input* _input, int _state)
{
	// アニメーションIDの更新
	updateAnimationID(_input, _state);

	// アニメーションの更新
	animation->update(animation, animationNS::TIME_PER_FRAME);

	return;
}
//============================================================================================================================================
// updateAnimationID
// 更新 - アニメーションID
//============================================================================================================================================
void AnimationPlayer::updateAnimationID(Input* _input, int _state)
{
	// アニメーションセット( 今 )の更新
	switch (animationID.current)
	{
	case animationNS::TYPE::IDLE:
		updateAnimationIDCurrentIdle(_input, _state);
		break;
	case animationNS::TYPE::SHOOTING:
		updateAnimationIDCurrentShooting(_input, _state);
		break;
	case animationNS::TYPE::RUN:
		updateAnimationIDCurrentRun(_input, _state);
		break;
	case animationNS::TYPE::RUN_FAST:
		updateAnimationIDCurrentRunFast(_input, _state);
		break;
	case animationNS::TYPE::JUMP:
		updateAnimationIDCurrentJump(_input, _state);
		break;
	case animationNS::TYPE::INSTALLATION:
		updateAnimationIDCurrentInstallation(_input, _state);
		break;
	case animationNS::TYPE::STANDING:
		updateAnimationIDCurrentStanding(_input, _state);
		break;
	case animationNS::TYPE::SLASH:
		updateAnimationIDCurrentSlash(_input, _state);
		break;
	case animationNS::TYPE::DOWN:
		updateAnimationIDCurrentDown(_input);
		break;
	case animationNS::TYPE::DOWN_POSE:
		updateAnimationIDCurrentDownPose(_input);
		break;
	case animationNS::TYPE::REVIVAL:
		updateAnimationIDCurrentRevival(_input, _state);
		break;
	case animationNS::TYPE::RECURSION:
		updateAnimationIDCurrentRecursion(_input);
		break;
	case animationNS::TYPE::FLOATING:
		updateAnimationIDCurrentFloating(_input);
		break;
	case animationNS::TYPE::FALLING:
		updateAnimationIDCurrentFalling(_input);
		break;
	case animationNS::TYPE::LANDING:
		updateAnimationIDCurrentLanding(_input, _state);
		break;
	default:
		break;
	}

	// アニメーションセット( 次 )の更新
	switch (animationID.next)
	{
	case animationNS::TYPE::IDLE:
		updateAnimationIDNextIdle();
		break;
	case animationNS::TYPE::SHOOTING:
		updateAnimationIDNextShooting();
		break;
	case animationNS::TYPE::RUN:
		updateAnimationIDNextRun();
		break;
	case animationNS::TYPE::RUN_FAST:
		updateAnimationIDNextRunFast();
		break;
	case animationNS::TYPE::JUMP:
		updateAnimationIDNextJump();
		break;
	case animationNS::TYPE::INSTALLATION:
		updateAnimationIDNextInstallation();
		break;
	case animationNS::TYPE::STANDING:
		updateAnimationIDNextStanding();
		break;
	case animationNS::TYPE::SLASH:
		updateAnimationIDNextSlash();
		break;
	case animationNS::TYPE::DOWN:
		updateAnimationIDNextDown();
		break;
	case animationNS::TYPE::DOWN_POSE:
		updateAnimationIDNextDownPose();
		break;
	case animationNS::TYPE::REVIVAL:
		updateAnimationIDNextRevival();
		break;
	case animationNS::TYPE::RECURSION:
		updateAnimationIDNextRecursion();
		break;
	case animationNS::TYPE::FLOATING:
		updateAnimationIDNextFloating();
		break;
	case animationNS::TYPE::FALLING:
		updateAnimationIDNextFalling();
		break;
	case animationNS::TYPE::LANDING:
		updateAnimationIDNextLanding();
		break;
	default:
		break;
	}

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentIdle
// 更新 - アニメーションID ( 今：アイドル )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentIdle(Input* _input, int _state)
{
	if (playerIndex == animationPlayerNS::PLAYER_2)
	{
		int unko = 9;
	}

	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}
	// ジャンプ
	if (_input->isKeyDown(keyTable.jump) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::B))
	{
		animationID.next = animationNS::TYPE::JUMP;
		return;
	}
	// メモリー・パイル設置
	if (_input->getMouseRButtonTrigger() || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::L1) ||
		aiAnimFlag[INSTALLATION])
	{
		animationID.next = animationNS::TYPE::INSTALLATION;
		return;
	}
	// メモリー・ライン切断
	if (_input->getMouseWheelState() == inputNS::DOWN || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::X) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000) && (_state == STATE_TYPE::GROUND) ||
		aiAnimFlag[SLASH])
	{
		animationID.next = animationNS::TYPE::SLASH;
		return;
	}
	// 走る・早く走る
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}
	// 射撃
	if (_input->getMouseLButton() || _input->getController()[playerIndex]->isButton(virtualControllerNS::BUTTONS::R1) ||
		aiAnimFlag[SHOOTING])
	{
		animationID.next = animationNS::TYPE::SHOOTING;
		return;
	}
	// アイドル
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentShooting
// 更新 - アニメーションID ( 今：射撃 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentShooting(Input* _input, int _state)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}
	// ジャンプ
	if (_input->isKeyDown(keyTable.jump) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::B))
	{
		animationID.next = animationNS::TYPE::JUMP;
		return;
	}
	// メモリー・パイル設置
	if (_input->getMouseRButtonTrigger() || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::L1) ||
		aiAnimFlag[INSTALLATION])
	{
		aiAnimFlag[INSTALLATION] = false;
		animationID.next = animationNS::TYPE::INSTALLATION;
		return;
	}
	// メモリー・ライン切断
	if (_input->getMouseWheelState() == inputNS::DOWN || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::X) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000) && (_state == STATE_TYPE::GROUND) ||
		aiAnimFlag[SLASH])
	{
		aiAnimFlag[SLASH] = false;
		animationID.next = animationNS::TYPE::SLASH;
		return;
	}
	// 走る・早く走る
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		aiAnimFlag[MOVE] = false;
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}
	// 射撃
	if (_input->getMouseLButton() || _input->getController()[playerIndex]->isButton(virtualControllerNS::BUTTONS::R1) ||
		aiAnimFlag[SHOOTING])
	{
		aiAnimFlag[SHOOTING] = false;
		animationID.next = animationNS::TYPE::SHOOTING;
		return;
	}
	// アイドル
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentRun
// 更新 - アニメーションID ( 今：走る )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentRun(Input* _input, int _state)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}
	// ジャンプ
	if (_input->isKeyDown(keyTable.jump) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::B))
	{
		animationID.next = animationNS::TYPE::JUMP;
		return;
	}
	// メモリー・パイル設置
	if (_input->getMouseRButtonTrigger() || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::L1) ||
		aiAnimFlag[INSTALLATION])
	{
		animationID.next = animationNS::TYPE::INSTALLATION;
		return;
	}
	// メモリー・ライン切断
	if (_input->getMouseWheelState() == inputNS::DOWN || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::X) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000) && (_state == STATE_TYPE::GROUND) ||
		aiAnimFlag[SLASH])
	{
		animationID.next = animationNS::TYPE::SLASH;
		return;
	}
	// 走る・早く走る
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}
	// アイドル
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentRunFast
// 更新 - アニメーションID ( 今：早く走る )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentRunFast(Input* _input, int _state)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}
	// ジャンプ
	if (_input->isKeyDown(keyTable.jump) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::B))
	{
		animationID.next = animationNS::TYPE::JUMP;
		return;
	}
	// メモリー・パイル設置
	if (_input->getMouseRButtonTrigger() || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::L1) ||
		aiAnimFlag[INSTALLATION])
	{
		animationID.next = animationNS::TYPE::INSTALLATION;
		return;
	}
	// メモリー・ライン切断
	if (_input->getMouseWheelState() == inputNS::DOWN || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::X) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000) && (_state == STATE_TYPE::GROUND) ||
		aiAnimFlag[SLASH])
	{
		animationID.next = animationNS::TYPE::SLASH;
		return;
	}
	// 走る・早く走る
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}
	// アイドル
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentJump
// 更新 - アニメーションID ( 今：ジャンプ )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentJump(Input* _input, int _state)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}

	animationID.next = animationNS::TYPE::JUMP;

	if (!animation->flag.animationPlayEnd) { return; }

	setFlagJump(false);

	// 走る・早く走る
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}

	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentInstallation
// 更新 - アニメーションID ( 今：立ち → メモリー・パイル設置 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentInstallation(Input* _input, int _state)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		if (getFlagInstallation()) { setFlagInstallation(false); }
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		if (getFlagInstallation()) { setFlagInstallation(false); }
		return;
	}

	animationID.next = animationNS::TYPE::INSTALLATION;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::STANDING;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentStanding
// 更新 - アニメーションID ( 今：メモリー・パイル設置 → 立ち )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentStanding(Input* _input, int _state)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		if (getFlagInstallation()) { setFlagInstallation(false); }
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		if (getFlagInstallation()) { setFlagInstallation(false); }
		return;
	}

	animationID.next = animationNS::TYPE::STANDING;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::IDLE;
	if (getFlagInstallation()) { setFlagInstallation(false); }

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentSlash
// 更新 - アニメーションID ( 今：メモリー・ライン切断 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentSlash(Input* _input, int _state)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}

	animationID.next = animationNS::TYPE::SLASH;

	if (!animation->flag.animationPlayEnd) { return; }

	if (getFlagMoveBan()) { setFlagMoveBan(false); }
	if (getFlagSlash()) { setFlagSlash(false); }
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentDown
// 更新 - アニメーションID ( 今：ダウン )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentDown(Input* _input)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		animation->flag.animationOn = true;
		return;
	}
	// 復活
	if (flag.revival)
	{
		animationID.next = animationNS::TYPE::REVIVAL;
		animation->flag.animationOn = true;
		return;
	}

	animationID.next = animationNS::TYPE::DOWN;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::DOWN_POSE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentDownPose
// 更新 - アニメーションID ( 今：ダウンポーズ )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentDownPose(Input* _input)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		animation->flag.animationOn = true;
		return;
	}
	// 復活
	if (flag.revival)
	{
		animationID.next = animationNS::TYPE::REVIVAL;
		animation->flag.animationOn = true;
		return;
	}

	animationID.next = animationNS::TYPE::DOWN_POSE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentRevival
// 更新 - アニメーションID ( 今：復活 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentRevival(Input* _input, int _state)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}

	animationID.next = animationNS::TYPE::REVIVAL;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentRecursion
// 更新 - アニメーションID ( 今：リカージョン )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentRecursion(Input* _input)
{
	animationID.next = animationNS::TYPE::RECURSION;

	if (flag.recursion) { return; }

	animationID.next = animationNS::TYPE::FLOATING;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentFloating
// 更新 - アニメーションID ( 今：浮遊 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentFloating(Input* _input)
{
	animationID.next = animationNS::TYPE::FLOATING;

	if (!flag.falling) { return; }

	animationID.next = animationNS::TYPE::FALLING;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentFalling
// 更新 - アニメーションID ( 今：落下 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentFalling(Input* _input)
{
	animationID.next = animationNS::TYPE::FALLING;

	if (!flag.landing) { return; }

	animationID.next = animationNS::TYPE::LANDING;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentLanding
// 更新 - アニメーションID ( 今：着地 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentLanding(Input* _input, int _state)
{
	// 被リカージョン
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// ダウン
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}

	animationID.next = animationNS::TYPE::LANDING;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDNextIdle
// 更新 - アニメーションID ( 次：アイドル )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextIdle(void)
{
	if (animationID.current == animationNS::TYPE::IDLE) { return; }

	animationID.current = animationNS::TYPE::IDLE;
	animation->animationSpeed = 1.0f;
	if (getFlagRevival()) { setFlagRevival(false); }
	if (getFlagLanding()) { setFlagLanding(false); }
	if (getFlagMoveBan()) { setFlagMoveBan(false); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::IDLE);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextShooting
// 更新 - アニメーションID ( 次：射撃 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextShooting(void)
{
	if (animationID.current == animationNS::TYPE::SHOOTING) { return; }

	animationID.current = animationNS::TYPE::SHOOTING;
	animation->animationSpeed = 1.0f;
	if (!getFlagGunRender()) { setFlagGunRender(true); }
	switching(animation, animationNS::TYPE::SHOOTING);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextRun
// 更新 - アニメーションID ( 次：走る )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextRun(void)
{
	if (animationID.current == animationNS::TYPE::RUN) { return; }

	animationID.current = animationNS::TYPE::RUN;
	animation->animationSpeed = 1.0f;
	if (!getFlagGunRender()) { setFlagGunRender(true); }
	switching(animation, animationNS::TYPE::RUN);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextRunFast
// 更新 - アニメーションID ( 次：早く走る )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextRunFast(void)
{
	if (animationID.current == animationNS::TYPE::RUN_FAST) { return; }

	animationID.current = animationNS::TYPE::RUN_FAST;
	animation->animationSpeed = 1.0f;
	if (!getFlagGunRender()) { setFlagGunRender(true); }
	switching(animation, animationNS::TYPE::RUN_FAST);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextJump
// 更新 - アニメーションID ( 次：ジャンプ )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextJump(void)
{
	if (animationID.current == animationNS::TYPE::JUMP) { return; }

	animationID.current = animationNS::TYPE::JUMP;
	animation->animationSpeed = 0.75f;
	if (!getFlagJump()) { setFlagJump(true); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::JUMP);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextInstallation
// 更新 - アニメーションID ( 次：立ち → メモリー・パイル設置 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextInstallation(void)
{
	if (animationID.current == animationNS::TYPE::INSTALLATION) { return; }

	animationID.current = animationNS::TYPE::INSTALLATION;
	animation->animationSpeed = 1.5f;
	if (!getFlagMoveBan()) { setFlagMoveBan(true); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::INSTALLATION);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextStanding
// 更新 - アニメーションID ( 次：メモリー・パイル設置 → 立ち )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextStanding(void)
{
	if (animationID.current == animationNS::TYPE::STANDING) { return; }

	animationID.current = animationNS::TYPE::STANDING;
	animation->animationSpeed = 1.5f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::STANDING);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextSlash
// 更新 - アニメーションID ( 今：メモリー・ライン切断 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextSlash(void)
{
	if (animationID.current == animationNS::TYPE::SLASH) { return; }

	animationID.current = animationNS::TYPE::SLASH;
	animation->animationSpeed = 2.0f;
	if (!getFlagMoveBan()) { setFlagMoveBan(true); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	if (!getFlagSlash()) { setFlagSlash(true); }
	switching(animation, animationNS::TYPE::SLASH);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextDown
// 更新 - アニメーションID ( 次：ダウン )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextDown(void)
{
	if (animationID.current == animationNS::TYPE::DOWN) { return; }

	animationID.current = animationNS::TYPE::DOWN;
	animation->animationSpeed = 1.0f;
	if (!getFlagMoveBan()) { setFlagMoveBan(true); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::DOWN);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextDownPose
// 更新 - アニメーションID ( 次：ダウンポーズ )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextDownPose(void)
{
	if (animationID.current == animationNS::TYPE::DOWN_POSE) { return; }

	animationID.current = animationNS::TYPE::DOWN_POSE;
	animation->animationSpeed = 1.0f;
	animation->flag.animationOn = false;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::DOWN_POSE);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextRevival
// 更新 - アニメーションID ( 次：復活 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextRevival(void)
{
	if (animationID.current == animationNS::TYPE::REVIVAL) { return; }

	animationID.current = animationNS::TYPE::REVIVAL;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::REVIVAL);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextRecursion
// 更新 - アニメーションID ( 次：リカージョン )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextRecursion(void)
{
	if (animationID.current == animationNS::TYPE::RECURSION) { return; }

	animationID.current = animationNS::TYPE::RECURSION;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::RECURSION);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextFloating
// 更新 - アニメーションID ( 次：浮遊 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextFloating(void)
{
	if (animationID.current == animationNS::TYPE::FLOATING) { return; }

	animationID.current = animationNS::TYPE::FLOATING;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::FLOATING);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextFalling
// 更新 - アニメーションID ( 次：落下 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextFalling(void)
{
	if (animationID.current == animationNS::TYPE::FALLING) { return; }

	animationID.current = animationNS::TYPE::FALLING;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::FALLING);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextLanding
// 更新 - アニメーションID ( 次：着地 )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextLanding(void)
{
	if (animationID.current == animationNS::TYPE::LANDING) { return; }

	animationID.current = animationNS::TYPE::LANDING;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	if (getFlagFalling()) { setFlagFalling(false); }
	switching(animation, animationNS::TYPE::LANDING);

	return;
}
//============================================================================================================================================
// updateTitle
// 更新 - タイトル
//============================================================================================================================================
void AnimationPlayer::updateTitle(void)
{
	// アニメーションセット( 今 )の更新
	updateAnimationIDCurrentIdleGeneral();

	// アニメーションセット( 次 )の更新
	updateAnimationIDNextIdleGeneral();

	// アニメーションの更新
	animation->update(animation, animationNS::TIME_PER_FRAME);

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentIdleGeneral
// 更新 - アニメーションID ( 今：汎用アイドル )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentIdleGeneral(void)
{
	// アイドル
	animationID.next = animationNS::TYPE::IDLE_GENERAL;

	return;
}
//============================================================================================================================================
// updateAnimationIDNextIdleGeneral
// 更新 - アニメーションID ( 次：汎用アイドル )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextIdleGeneral(void)
{
	if (animationID.current == animationNS::TYPE::IDLE_GENERAL) { return; }

	animationID.current = animationNS::TYPE::IDLE_GENERAL;
	animation->animationSpeed = 1.0f;
	switching(animation, animationNS::TYPE::IDLE_GENERAL);

	return;
}
//============================================================================================================================================
// render
// 描画
//============================================================================================================================================
void AnimationPlayer::render(LPDIRECT3DDEVICE9 _device, D3DXMATRIX _matrixRotation,D3DXMATRIX _matrixPosition, StaticMeshLoader* _staticMeshLoader)
{
	D3DMATERIAL9 materialDefault;	//	マテリアル

	D3DXMATRIX matrixWorld;

	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation, D3DXToRadian(180.0f));
	D3DXMatrixMultiply(&rotation, &rotation,&_matrixRotation);

	D3DXMatrixMultiply(&matrixWorld, &rotation, &_matrixPosition);

	// ライティングモードを設定
	_device->SetRenderState(D3DRS_LIGHTING, true);

	// モデルタイプでカリング設定の切り替え
	switch (modelType)
	{
	case MODEL_TYPE::ADAM: _device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); break;
	case MODEL_TYPE::EVE: _device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); break;
	default: break;
	}

	// ワールドマトリクスの設定
	_device->SetTransform(D3DTS_WORLD, &matrixWorld);

	// 現在のマテリアルを取得
	_device->GetMaterial(&materialDefault);

	// アニメーションの描画
	animation->render(_device, animation, &matrixWorld);

	// マテリアルを戻す
	_device->SetMaterial(&materialDefault);

	// ライティングモードを設定
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return;
}
//============================================================================================================================================
// setAnimationConfiguration
// 設定 - アニメーション設定
//============================================================================================================================================
void AnimationPlayer::setAnimationConfiguration(int _scene)
{
	switch (_scene)
	{
	case SCENE_TYPE::TITLE:
	case SCENE_TYPE::CHARACTER_SELECT:
		animationID.current = animationNS::TYPE::IDLE_GENERAL;
		//animationID.next = animationNS::TYPE::IDLE_GENERAL;
		animation->switching(animation, animationNS::TYPE::IDLE_GENERAL);
		animation->flag.animationOn = true;
		animation->flag.animationPlayEnd = false;
		//animation->update(animation, animationNS::TIME_PER_FRAME);
		break;
	case SCENE_TYPE::GAME:
		animationID.current = animationNS::TYPE::IDLE;
		//animationID.next = animationNS::TYPE::IDLE;
		animation->switching(animation, animationNS::TYPE::IDLE);
		animation->flag.animationOn = true;
		animation->flag.animationPlayEnd = false;
		//animation->update(animation, animationNS::TIME_PER_FRAME);
		break;
	default:
		break;
	}

	return;
}
//============================================================================================================================================
// resetAnimation
// リセット
//============================================================================================================================================
void AnimationPlayer::resetAnimation(void)
{
	flag.moveBan = false;
	flag.gunRenderFlag = false;
	flag.jump = false;
	flag.installation = false;
	flag.slash = false;
	flag.recursion = false;
	flag.falling = false;
	flag.landing = false;
	flag.revival = false;

	return;
}