//===================================================================================================================================
//【CountUI.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/12/12
// [更新日]2019/12/12
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace CountUINS
{
	//表示時間
	const float COUNT_DISPLAY_TIME	= 1.0f;
	const float START_DISPLAY_TIME	= 3.0f;
	const float FINISH_DISPLAY_TIME	= 5.0f;

	//スプラッシュスプライト情報の定義
	//開始時
	namespace StartNS {
		const int WIDTH					= 512;
		const int DIVIDE_WIDTH			= 4;//横の分割数
		const int HEIGHT				= 128;
	}

	//終了時
	namespace FinishNS {
		const int WIDTH					= 1024;
		const int DIVIDE_WIDTH			= 11;//横の分割数
		const int HEIGHT				= 128;
	}
	const D3DXVECTOR3 POSITION		= D3DXVECTOR3((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 ROTATION		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR			= D3DCOLOR_RGBA(255, 255, 255, 0);


	enum {
		START,
		FINISH,
	};
}

//===================================================================================================================================
//【カウントＵＩクラス】
//===================================================================================================================================
class CountUI :public Sprite
{
private:
	float	displayTimer;
	float	displayTime;
	int		count;
	bool	finished;
public:
	CountUI();
	~CountUI();
	void update(float frameTime);
	void startCount(int count);
	void finishCount(int count);

};

