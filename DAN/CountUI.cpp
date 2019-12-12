//===================================================================================================================================
//【SplashSprite.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/09/23
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "CountUI.h"
#include "TextureLoader.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace CountUINS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
CountUI::CountUI()
{
	initialize(
		*textureNS::reference(textureNS::UI_TIMER_NUMBER),	//テクスチャポインタ
		SpriteNS::CENTER,									//原点
		WIDTH/DIVIDE_WIDTH,									//幅
		HEIGHT,												//高さ
		POSITION,											//位置
		ROTATION,											//回転
		COLOR												//色
	);
	//UV値の設定
	setUVCoord(
		D3DXVECTOR2(0.0f, 0.0f)					+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f),
		D3DXVECTOR2(1.0f/DIVIDE_WIDTH, 0.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f),
		D3DXVECTOR2(0.0f, 1.0f)					+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f),
		D3DXVECTOR2(1.0f/ DIVIDE_WIDTH, 1.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f)
	);

	displayTimer = 0.0f;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
CountUI::~CountUI()
{

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void CountUI::update(float frameTime)
{
	//表示時間がない場合は更新しない
	if (displayTimer <= 0)return;

	//表示時間の更新
	displayTimer -= frameTime;
	
	//タイマーに応じて、状態を変化させる
	float rate = max(0.0f,displayTimer/DISPLAY_TIME);		//rate：1.0f→0.0fに遷移する
	setAlphaAllVertex((int)((float)255 * rate));			//徐々に透明になる

	setVertex();

}

//===================================================================================================================================
//【再生】
//===================================================================================================================================
void CountUI::play(int count)
{
	//表示時間のセット
	displayTimer = DISPLAY_TIME;

	switch (count)
	{
	case 3:	case 2:	case 1:
	//UV値の設定
	setUVCoord(
		D3DXVECTOR2(0.0f,				0.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*count,0.0f),
		D3DXVECTOR2(1.0f/DIVIDE_WIDTH,	0.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*count,0.0f),
		D3DXVECTOR2(0.0f,				1.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*count,0.0f),
		D3DXVECTOR2(1.0f/DIVIDE_WIDTH,	1.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*count,0.0f));
		break;
	case 0:
		setTexturePointer(*textureNS::reference(textureNS::UI_TIME_UP));
		setSize(1700, 400);
		setUVCoord(D3DXVECTOR2(0.0, 0.0), D3DXVECTOR2(1.0, 0.0), D3DXVECTOR2(0.0, 1.0), D3DXVECTOR2(1.0, 1.0));
		break;
	}

}

