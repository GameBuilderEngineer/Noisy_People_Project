//===================================================================================================================================
//【OPState.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2020/01/20
// [更新日]2020/01/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Player.h"

namespace OPStateNS {
	//===================================================================================================================================
	//【OPステートクラス】
	//===================================================================================================================================
	class OPState :	public AbstractState
	{
	private:
		Player* player;
	public:
		OPState(Player* player);
		~OPState();
		virtual void update(float frameTime);
		virtual void start();
		virtual void operation();
		virtual void physics();
		virtual void controlCamera();
		virtual AbstractState* transition();
		virtual void end();
	};
}