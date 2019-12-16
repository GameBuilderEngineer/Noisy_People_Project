//-----------------------------------------------------------------------------
// エネミー追跡マーク[EnemyChaseMark.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/12/10
//-----------------------------------------------------------------------------
#include "EnemyChaseMark.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
using namespace enemyChaseMarkNS;

// Staticメンバ変数
int StandardChaseMark::cntInstance= 0;

StandardChaseMark::StandardChaseMark(D3DXVECTOR3 position) : InstancingBillboardNS::Instance()
{
	//初期値の設定
	this->position	= position;
	this->rotation	= D3DXVECTOR3(-D3DX_PI / 2.0f, 0, 0);
	this->scale		= D3DXVECTOR2(1.0f, 1.0f);
	this->speed		= D3DXVECTOR3(0, 0, 0);
	this->limitTime	= 1.0f;
	//this->uv.x		= 0.125f*7;
	//this->uv.y		= 0.125f*7;
}

void StandardChaseMark::update(float frameTime)
{
	//lifeTimer += frameTime;
	if (lifeTimer >= limitTime)return;
}


//=============================================================================
// レンダラークラス
//=============================================================================
EnemyChaseMark::EnemyChaseMark() :InstancingBillboard::InstancingBillboard()
{
	using namespace InstancingBillboardNS;
	InstancingBillboard::initialize(*textureNS::reference(textureNS::UI_CAHSE_MARK),0,0);
	setRenderType(FOREGROUND_PASS | OFF_BILLBOARD_PASS);
}

EnemyChaseMark::~EnemyChaseMark()
{
}

void EnemyChaseMark::update(float frameTime)
{
	InstancingBillboard::update(frameTime);
}

void EnemyChaseMark::generateMark(enemyChaseMarkNS::StandardChaseMark* mark)
{
	StandardChaseMark::cntInstance++;

	generateInstance(mark);
	updateAccessList();
}

void EnemyChaseMark::deleteMark(enemyChaseMarkNS::StandardChaseMark* mark)
{
	StandardChaseMark::cntInstance--;

	// アドレス値で検索
	int target;
	for (int i = 0; i < instanceList->nodeNum; i++)
	{
		if (mark == *instanceList->getValue(i))
		{
			target = i;
			break;
		}
	}

	mark->lifeTimer = mark->limitTime + 1.0f;	// deleteInstance()を有効にするため
	deleteInstance(target);
	updateAccessList();
}