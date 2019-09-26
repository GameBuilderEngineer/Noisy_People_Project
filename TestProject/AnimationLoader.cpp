//===================================================================================================================================
//【AnimationLoader.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/12
// [更新日]2019/09/12
//===================================================================================================================================
#include "AnimationLoader.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
AnimationLoader::AnimationLoader()
{
	
}


//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
AnimationLoader::~AnimationLoader()
{

	for (int i = 0; i < animationPlayerNS::PLAYER_TYPE_MAX; i++)
	{
		for (int j = 0; j < animationPlayerNS::MODEL_TYPE_MAX; j++)
		{
			animation[i][j].release();
		}
	}
}

void AnimationLoader::initialize(LPDIRECT3DDEVICE9 device)
{
	for (int i = 0; i < animationPlayerNS::PLAYER_TYPE_MAX; i++)
	{
		for (int j = 0; j < animationPlayerNS::MODEL_TYPE_MAX; j++)
		{
			animation[i][j].initialize(device, i, j);
		}
	}
}

AnimationPlayer* AnimationLoader::getAnimationModel(int playerIndex,int modelType)
{
	return &animation[playerIndex][modelType];
}