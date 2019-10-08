//===================================================================================================================================
//【AnimationLoader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/12
// [更新日]2019/09/12
//===================================================================================================================================
#pragma once
#include "Base.h"
#include "AnimationPlayer.h"

class AnimationLoader :
	public Base
{
private:
	AnimationPlayer animation[animationPlayerNS::PLAYER_TYPE_MAX][animationPlayerNS::MODEL_TYPE_MAX];					//	アニメーション

public:
	AnimationLoader();
	~AnimationLoader();
	void initialize(LPDIRECT3DDEVICE9 device);
	AnimationPlayer* getAnimationModel(int playerIndex,int modelType);

};

