//=====================================
//基本ゲージクラス[BasicGuage.h]
//染矢晃介
//=====================================
#pragma once

//=====================================
//インクルード
//=====================================
#include "HPguage.h"
#include "HPguageEmp.h"

//=====================================
//名前空間
//=====================================
namespace guageNS
{

}

//=====================================
//クラス定義
//=====================================
class Guage
{
public://メンバー変数
	HPguage  *hpGuage;
	HPguageEmp *hpGuageEmp;
private:
public://メンバー関数
	Guage();
	~Guage();
	void initialize();
	void render();
	void update();

};