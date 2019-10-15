//-----------------------------------------------------------------------------
// アナログツリークラス [Tree.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/13
//-----------------------------------------------------------------------------
#pragma once
#include"Tree.h"

//=============================================================================
//クラス定義
//=============================================================================
class AnalogTree: public Tree
{
private:

public:
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// Getter

	// Setter
};
