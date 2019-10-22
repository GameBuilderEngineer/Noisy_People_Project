//-----------------------------------------------------------------------------
// エネミー管理クラス [Enemy.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include "EnemyManager.h"
#include "ImguiManager.h"
#include "EnemyTools.h"
using namespace enemyNS;


//=============================================================================
// 初期化
//=============================================================================
void EnemyManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;
	Enemy::resetNumOfEnemy();

	// 接地フィールド情報をセット
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// 描画オブジェクトの作成
	wolfRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	tigerRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_BUNNY));
	bearRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_HAT));


#if 1	// エネミーツールのデータを読み込む
	ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		createEnemyData(enemyTools->GetEnemySet(i));
	}
	SAFE_DELETE(enemyTools);
#endif

#if 1	// エネミーオブジェクトをツールデータを元に作成する
	enemyList.reserve(ENEMY_OBJECT_MAX);	// update()で動的な確保をせず済むようメモリを増やしておく
	for (size_t i = 0; i < enemyDataList.nodeNum; i++)
	{
		if (1/* 本来はプレイヤーの初期位置と近ければ～など条件が付く */)
		{
			createEnemy(enemyDataList.getValue(i));
		}
	}
#endif
}


//=============================================================================
// 終了処理
//=============================================================================
void EnemyManager::uninitialize()
{

	//for (int i = 0; i < objectNum; i++)
	//{
	//	SAFE_DELETE(*objectList->getValue(i));
	//}
	//objectList->allClear();
	//objectNum = objectList->nodeNum;

	// 全エネミーオブジェクトを破棄
	destroyAllEnemy();

	// ベクターの確保メモリを初期化（メモリアロケータだけに戻す）
	std::vector<Enemy*> temp;
	enemyList.swap(temp);

	// 描画オブジェクトの破棄
	SAFE_DELETE(wolfRenderer);
	SAFE_DELETE(tigerRenderer);
	SAFE_DELETE(bearRenderer);
}


//=============================================================================
// 更新処理
//=============================================================================
void EnemyManager::update(float frameTime)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->update(frameTime);

		// 撃退したエネミーオブジェクトを破棄する
		if (enemyList[i]->getEnemyData()->isAlive == false)
		{
			destroyEnemy(enemyList[i]->getEnemyData()->id);
		}
	}
	wolfRenderer->update();
	tigerRenderer->update();
	bearRenderer->update();
}


//=============================================================================
// 描画処理
//=============================================================================
void EnemyManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	wolfRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	tigerRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bearRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// エネミーデータの作成
//=============================================================================
enemyNS::EnemyData* EnemyManager::createEnemyData(enemyNS::ENEMYSET enemySetting)
{
	// ENEMYSET構造体からEnemyDataを作る
	EnemyData enemyData;
	enemyData.id = enemySetting.id;
	enemyData.type = enemySetting.type;
	enemyData.defaultState = enemySetting.defaultState;
	enemyData.defaultPosition = enemySetting.defaultPosition;
	enemyData.defaultDirection = enemySetting.defaultDirection;
	enemyData.setUp();
	// リストに追加
	enemyDataList.insertFront(enemyData);
	enemyDataList.listUpdate();
	// エネミーデータIDの最大値を更新（最後に追加したデータ + 1が次回発行IDとなる）
	nextID = enemyData.id + 1;
	// リストに追加したデータのポインタを返す
	return enemyDataList.getValue(0);
}


//=============================================================================
// エネミーオブジェクトの作成
//=============================================================================
void EnemyManager::createEnemy(EnemyData* enemyData)
{
	Enemy* enemy = NULL;

	switch (enemyData->type)
	{
	case WOLF:
		enemy = new Wolf(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		wolfRenderer->generateObject(enemy);
		break;

	case TIGER:
		enemy = new Tiger(staticMeshNS::reference(staticMeshNS::SAMPLE_BUNNY), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		tigerRenderer->generateObject(enemy);
		break;

	case BEAR:
		enemy = new Bear(staticMeshNS::reference(staticMeshNS::SAMPLE_HAT), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		bearRenderer->generateObject(enemy);
		break;
	}
}


//=============================================================================
// エネミーオブジェクトの破棄
//=============================================================================
void EnemyManager::destroyEnemy(int _id)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyData()->id != _id) { continue; }

		switch (enemyList[i]->getEnemyData()->type)
		{
		case WOLF:
			wolfRenderer->deleteObjectByID(enemyList[i]->id);
			break;

		case TIGER:
			tigerRenderer->deleteObjectByID(enemyList[i]->id);
			break;

		case BEAR:
			bearRenderer->deleteObjectByID(enemyList[i]->id);
			break;
		}
		enemyList.erase(enemyList.begin() + i);	// ベクター要素を消去
		break;
	}	
}


//=============================================================================
// 全エネミーオブジェクトの破棄
//=============================================================================
void EnemyManager::destroyAllEnemy()
{
	wolfRenderer->allDelete();
	tigerRenderer->allDelete();
	bearRenderer->allDelete();
	enemyList.clear();	// ベクター要素数を0にする
}


//=============================================================================
// エネミーIDを発行する
//=============================================================================
int EnemyManager::issueNewID()
{
	int ans = nextID;
	nextID++;
	return ans;
}


//=============================================================================
// ImGuiに出力
//=============================================================================
void EnemyManager::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("EnemyInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		//ImGui::Text("numOfEnemyData:%d\n", EnemyData::getNumOfEnemyData());
		ImGui::Text("nodeNum:%d\n", enemyDataList.nodeNum);
		ImGui::Text("nextID:%d\n", nextID);
		ImGui::Text("numOfEnemy:%d\n", Enemy::getNumOfEnemy());

		//ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//位置
		//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
		//ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);					//スケール
		//ImGui::SliderFloat("radius", &radius, 0, limitTop);							//半径
		//ImGui::SliderFloat("alpha", &alpha, 0, 255);									//透過値
		//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);					//速度
		//ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//加速度
		//ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);				//重力

		//ImGui::Checkbox("onGravity", &onGravity);										//重力有効化フラグ
		//ImGui::Checkbox("onActive", &onActive);										//アクティブ化フラグ
		//ImGui::Checkbox("onRender", &onRender);										//描画有効化フラグ
		//ImGui::Checkbox("onLighting", &onLighting);									//光源処理フラグ
		//ImGui::Checkbox("onTransparent", &onTransparent);								//透過フラグ
		//ImGui::Checkbox("operationAlpha", &operationAlpha);							//透過値の操作有効フラグ

		//ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//透過値の操作有効フラグ
	}
#endif
}


//=============================================================================
// Getter
//=============================================================================
LinkedList<enemyNS::EnemyData>* EnemyManager::getEnemyDataList() { return &enemyDataList; }
std::vector<Enemy*>& EnemyManager::getEnemyList() { return enemyList; }
