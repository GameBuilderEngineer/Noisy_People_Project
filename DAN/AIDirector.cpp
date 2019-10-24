//-----------------------------------------------------------------------------
// メタAIクラス [AIDirector.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "EnemyManager.h"
#include "ImguiManager.h"
using namespace aiNS;


//=============================================================================
// 初期化
//=============================================================================
void AIDirector::initialize()
{
	sensor.initialize(&playerAD, &enemyAD, &treeAD, &itemAD);
	eventMaker.initialize(&playerAD, &enemyAD, &treeAD, &itemAD);
	opeGenerator.initialize(&playerAD, &enemyAD, &treeAD, &itemAD);
	frameCnt = 0;
}


//=============================================================================
// 実行
//=============================================================================
void AIDirector::run()
{
	switch (frameCnt %= 3)
	{
	case 0:	sensor.update();		break;
	case 1:	eventMaker.update();	break;
	case 2:	opeGenerator.update();	break;
	}
	frameCnt++;
}


//=============================================================================
// ImGuiに出力
//=============================================================================
void AIDirector::outputGUI()
{
#ifdef _DEBUG

	//if (ImGui::CollapsingHeader("EnemyInformation"))
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	//float limitTop = 1000;
	//	//float limitBottom = -1000;

	//	ImGui::Text("numOfEnemy:%d", Enemy::getNumOfEnemy());

	//	//ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//位置
	//	//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
	//	//ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);					//スケール
	//	//ImGui::SliderFloat("radius", &radius, 0, limitTop);							//半径
	//	//ImGui::SliderFloat("alpha", &alpha, 0, 255);									//透過値
	//	//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);					//速度
	//	//ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//加速度
	//	//ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);				//重力

	//	//ImGui::Checkbox("onGravity", &onGravity);										//重力有効化フラグ
	//	//ImGui::Checkbox("onActive", &onActive);										//アクティブ化フラグ
	//	//ImGui::Checkbox("onRender", &onRender);										//描画有効化フラグ
	//	//ImGui::Checkbox("onLighting", &onLighting);									//光源処理フラグ
	//	//ImGui::Checkbox("onTransparent", &onTransparent);								//透過フラグ
	//	//ImGui::Checkbox("operationAlpha", &operationAlpha);							//透過値の操作有効フラグ

	//	//ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//透過値の操作有効フラグ
	//}

#endif
}