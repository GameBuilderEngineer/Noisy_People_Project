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
		*textureNS::reference(textureNS::UI_COUNT_NUM),	//テクスチャポインタ
		SpriteNS::CENTER,									//原点
		WIDTH/DIVIDE_WIDTH,									//幅
		HEIGHT,												//高さ
		POSITION,											//位置
		ROTATION,											//回転
		COLOR												//色
	);

	setSize(WIDTH / DIVIDE_WIDTH*3, HEIGHT*3);

	//UV値の設定
	setUVCoord(
		D3DXVECTOR2(0.0f, 0.0f)					+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f),
		D3DXVECTOR2(1.0f/DIVIDE_WIDTH, 0.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f),
		D3DXVECTOR2(0.0f, 1.0f)					+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f),
		D3DXVECTOR2(1.0f/ DIVIDE_WIDTH, 1.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f)
	);

	displayTimer = 0.0f;
	finished = false;
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

	float rate = 0.0f;

	//表示時間の更新
	displayTimer -= frameTime;

	if (finished)
	{
		setAlphaAllVertex(255);
	}
	else {
		//タイマーに応じて、状態を変化させる
		rate = max(0.0f,displayTimer/displayTime);		//rate：1.0f→0.0fに遷移する
		setAlphaAllVertex((int)((float)255 * rate));			//徐々に透明になる
	}

	setVertex();
}

//===================================================================================================================================
//【スタート時カウント】
//===================================================================================================================================
void CountUI::startCount(int count)
{
	this->count = count;
	switch (count)
	{
	case 3:	case 2:	case 1:
		//表示時間のセット
		displayTimer = displayTime = COUNT_DISPLAY_TIME;
		//テクスチャポインタのセット
		setTexturePointer(*textureNS::reference(textureNS::UI_COUNT_NUM));
		//サイズのセット
		setSize(WIDTH / DIVIDE_WIDTH * 3, HEIGHT * 3);
		//UV値の設定
		setUVCoord(
			D3DXVECTOR2(0.0f, 0.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(1.0f / DIVIDE_WIDTH, 0.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(0.0f, 1.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(1.0f / DIVIDE_WIDTH, 1.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f));
		break;
	case 0:
		//表示時間のセット
		displayTimer = displayTime = START_DISPLAY_TIME;
		//テクスチャポインタのセット
		setTexturePointer(*textureNS::reference(textureNS::UI_GAME_START));
		//サイズのセット
		setSize(1024, 512);
		//UV値の設定
		setUVCoord(D3DXVECTOR2(0.0, 0.0), D3DXVECTOR2(1.0, 0.0), D3DXVECTOR2(0.0, 1.0), D3DXVECTOR2(1.0, 1.0));
		break;
	}
}

//===================================================================================================================================
//【フィニッシュ時カウント】
//===================================================================================================================================
void CountUI::finishCount(int count)
{
	this->count = count;
	switch (count)
	{
	case 10:case 9:case 8:case 7:case 6:case 5:case 4:case 3:case 2:case 1:
		//表示時間のセット
		displayTimer = displayTime = COUNT_DISPLAY_TIME;
		setTexturePointer(*textureNS::reference(textureNS::UI_COUNT_NUM));
		setSize(WIDTH / DIVIDE_WIDTH * 3, HEIGHT * 3);
		//UV値の設定
		setUVCoord(
			D3DXVECTOR2(0.0f, 0.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(1.0f / DIVIDE_WIDTH, 0.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(0.0f, 1.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(1.0f / DIVIDE_WIDTH, 1.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f));
		break;
	case 0:
		//表示時間のセット
		displayTimer = displayTime = FINISH_DISPLAY_TIME;
		setTexturePointer(*textureNS::reference(textureNS::UI_TIME_UP));
		setSize(1700, 400);
		setUVCoord(D3DXVECTOR2(0.0, 0.0), D3DXVECTOR2(1.0, 0.0), D3DXVECTOR2(0.0, 1.0), D3DXVECTOR2(1.0, 1.0));
		finished = true;
		break;
	}

}

