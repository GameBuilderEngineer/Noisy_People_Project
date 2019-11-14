//===================================================================================================================================
//【Advertisement.h】
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/11/12
//===================================================================================================================================
#pragma once
#include "InstancingBillboard.h"

//=============================================================================
// インスタンスクラス
//=============================================================================
namespace advetisementNS
{
	class alphaVer :public InstancingBillboardNS::Instance
	{
	public:
		//コンストラクタ
		alphaVer(D3DXVECTOR3 position);
		//更新
		virtual void update(float frameTime) override;
	};
}


//=============================================================================
// レンダラークラス
//=============================================================================
class Advertisement :public InstancingBillboard
{
private:

public:
	Advertisement();
	~Advertisement();
	void update(float frameTime);
};
