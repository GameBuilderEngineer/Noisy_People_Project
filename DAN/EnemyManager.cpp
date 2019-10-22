//-----------------------------------------------------------------------------
// エネミー管理クラス [Enemy.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include <cassert>
#include "EnemyManager.h"
#include "ImguiManager.h"
#include "EnemyTools.h"
using namespace enemyNS;


//=============================================================================
// 初期化
//=============================================================================
void EnemyManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;								// 次回発行IDを0に初期化
	Enemy::resetNumOfEnemy();				// エネミーオブジェクトの数を初期化
	enemyList.reserve(ENEMY_OBJECT_MAX);	// update()で動的な確保をせず済むようメモリを増やしておく

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
	// 全てのエネミーオブジェクトを破棄
	destroyAllEnemy();

	// 全てのエネミーデータを破棄
	destroyAllEnemyData();

	// ベクターの確保メモリを初期化（メモリアロケータだけに戻す）
	std::vector<Enemy*> temp;
	enemyList.swap(temp);

	// 描画オブジェクトを破棄
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
// エネミーオブジェクトを作成
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
// エネミーデータの破棄
//=============================================================================
void EnemyManager::destroyEnemyData(int _id)
{
	for (int i = 0; i < enemyDataList.nodeNum; i++)
	{
		if (enemyDataList.getValue(i)->id != _id) { continue; }

		// 破棄するエネミーデータのIDに無効値を書き込んでおく（※assertDestructionOrder()）
		enemyDataList.getValue(i)->id = -1;

		// エネミーデータ構造体はノード内にメモリが取られているため
		// ノードの破棄と合わせてエネミーデータも破棄されている
		enemyDataList.remove(enemyDataList.getNode(i));
		break;
	}

}


//=============================================================================
// 全てのエネミーデータを破棄
//=============================================================================
void EnemyManager::destroyAllEnemyData()
{
	// エネミーデータ構造体はノード内にメモリが取られているため
	// ノードの破棄と合わせてエネミーデータも破棄されている
	enemyDataList.allClear();
}


//=============================================================================
// エネミーオブジェクトを破棄
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

#ifdef _DEBUG
	assertDestructionOrder();
#endif//_DEBUG
}


//=============================================================================
// 全てのエネミーオブジェクトを破棄
//=============================================================================
void EnemyManager::destroyAllEnemy()
{
	wolfRenderer->allDelete();
	tigerRenderer->allDelete();
	bearRenderer->allDelete();
	enemyList.clear();	// ベクター要素数を0にする

#ifdef _DEBUG
	assertDestructionOrder();
#endif//_DEBUG
}


//=============================================================================
// 破棄順序違反
//=============================================================================
void EnemyManager::assertDestructionOrder()
{
	// エネミーの参照先のエネミーデータがエネミーデータリストにない場合のアサーション
	// 先にデータを破棄して後にエネミーを破棄しようとする場合に該当する
	int cnt = 0;
	for (int i = 0; i < enemyDataList.nodeNum; i++)
	{
		for (int k = 0; k < enemyList.size(); k++)
		{
			// エネミーデータ単体を破棄する際にIDに無効値が書き込まれているため
			// 解放済みのメモリにデータが残っていたとしても下記if文条件は偽となる
			if (enemyDataList.getValue(i)->id == enemyList[k]->getEnemyData()->id)
			{
				cnt++;
			}
		}
	}
	assert(cnt == enemyList.size());
	assert(enemyDataList.nodeNum >= enemyList.size());
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
		ImGui::Text("enemyDataList.nodeNum:%d\n", enemyDataList.nodeNum);
		ImGui::Text("enemyList.size()     :%d\n", enemyList.size());
		ImGui::Text("numOfEnemy           :%d\n", Enemy::getNumOfEnemy());
		ImGui::Text("nextID				　:%d\n", nextID);
		//for (int i = 0; i < enemyList.size(); i++)
		//{
		//	ImGui::Text("%d", enemyList[i]->getEnemyData()->id);
		//}
	}
#endif
}


//=============================================================================
// ファイルに準拠してエネミーオブジェクトを再配置する
//=============================================================================
void EnemyManager::relocateEnemyAccordingToFile()
{
	destroyAllEnemy();						// 全てのエネミーオブジェクトを破棄
	destroyAllEnemyData();					// 全てのエネミーデータを破棄
	nextID = 0;								// 次回発行IDを0に初期化
	Enemy::resetNumOfEnemy();				// エネミーオブジェクトの数を初期化
	enemyList.reserve(ENEMY_OBJECT_MAX);	// update()で動的な確保をせず済むようメモリを増やしておく

	// エネミーツールのデータを読み込む
	ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		createEnemyData(enemyTools->GetEnemySet(i));
	}
	SAFE_DELETE(enemyTools);

	// エネミーオブジェクトをツールデータを元に作成する
	for (size_t i = 0; i < enemyDataList.nodeNum; i++)
	{
		createEnemy(enemyDataList.getValue(i));
	}
}


//=============================================================================
// Getter
//=============================================================================
LinkedList<enemyNS::EnemyData>* EnemyManager::getEnemyDataList() { return &enemyDataList; }
std::vector<Enemy*>& EnemyManager::getEnemyList() { return enemyList; }
