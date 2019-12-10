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
void EnemyManager::initialize(std::string _sceneName, LPD3DXMESH _attractorMesh,
	D3DXMATRIX* _attractorMatrix, GameMaster* _gameMaster, Player* _player)
{
	nextID = 0;								// 次回発行IDを0に初期化
	Enemy::resetNumOfEnemy();				// エネミーオブジェクトの数を初期化
	enemyList.reserve(ENEMY_OBJECT_MAX);	// update()で動的な確保をせず済むようメモリを増やしておく
	cntTimeDataList = 0.0f;

	// 接地フィールドとプレイヤーをセット
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
	gameMaster = _gameMaster;
	player = _player;

	// 描画オブジェクトの作成
	wolfRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::WOLF));
	tigerRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::TIGER));
	bearRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::BEAR));

// チュートリアルシーンでの初期化処理
if (_sceneName == "Scene -Tutorial-")
{

	return;
}

#if 1	// エネミーツールのデータを読み込む
ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
{
	createEnemyData(enemyTools->GetEnemySet(i));
}
SAFE_DELETE(enemyTools);
#endif

#if 1	// エネミーオブジェクトをツールデータを元に作成する
for (int i = 0; i < enemyDataList.nodeNum; i++)
{
	if (1/* 本来はプレイヤーの初期位置と近ければ〜など条件が付く */)
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
	//----------------------
	// エネミーデータリスト
	//----------------------
	cntTimeDataList += frameTime;
	if (cntTimeDataList > DATA_LIST_CHECK_INTERVAL)
	{
		cntTimeDataList = 0.0f;

		for (int i = 0; i < enemyDataList.nodeNum; i++)
		{
			// 存在している or 死亡済みのデータをパスする
			if (enemyDataList.getValue(i)->isObjectExists ||
				enemyDataList.getValue(i)->isAlive == false)
			{
				continue;
			}

			// 近距離エネミーの自動作成
			float dist1 = D3DXVec3LengthSq(&(enemyDataList.getValue(i)->position - player[gameMasterNS::PLAYER_1P].position));
			float dist2 = D3DXVec3LengthSq(&(enemyDataList.getValue(i)->position - player[gameMasterNS::PLAYER_2P].position));
			if (dist1 < NEAR_DISTANCE2 || dist2 < NEAR_DISTANCE2)
			{
				enemyDataList.getValue(i)->setUp();		// 初期配置エネミーのデータを初期化する
				createEnemy(enemyDataList.getValue(i));
			}
		}
	}

	//----------------
	// エネミーリスト
	//----------------
	vector<Enemy*>::iterator itr = enemyList.begin();
	while(itr != enemyList.end())
	{
		// エネミーの更新
		(*itr)->update(frameTime);

		// 破棄フラグ
		bool isDestroyTarget = false;

		// 遠距離エネミーを破棄する
		float dist1 = D3DXVec3LengthSq(&((*itr)->position - player[gameMasterNS::PLAYER_1P].position));
		float dist2 = D3DXVec3LengthSq(&((*itr)->position - player[gameMasterNS::PLAYER_2P].position));
		if ((dist1 > FAR_DISTANCE2 && dist2 > FAR_DISTANCE2) && (*itr)->getEnemyData()->type != enemyNS::BEAR)
		{
			isDestroyTarget = true;
		}

		// 死亡済みエネミーを破棄する
		if ((*itr)->getEnemyData()->isAlive == false)
		{
			isDestroyTarget = true;
		}

		// 破棄処理
		if (isDestroyTarget)
		{
			// 動的作成イベントで作ったエネミーのIDを記録する
			int destroyTargetEnemyData = -1;
			if ((*itr)->getEnemyData()->isGeneratedBySpawnEvent)
			{
				destroyTargetEnemyData = (*itr)->getEnemyID();
			}

			// エネミーオブジェクトの破棄
			(*itr)->getEnemyData()->isObjectExists = false;
			destroyEnemy(*itr);
			itr = enemyList.erase(itr);

			// 動的作成イベントで作ったエネミーのみエネミーデータも破棄する（エネミーの破棄後）
			if (destroyTargetEnemyData != -1)
			{
				destroyEnemyData(destroyTargetEnemyData);
			}
		}
		else
		{
			itr++;
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

#ifdef _DEBUG

	for (size_t i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->debugRender();

		// センサーを描画
#ifdef RENDER_SENSOR
		for (int k = 0; k < 4; k++)
		{
			enemyList[i]->eyeAngleRay[k].render(VISIBLE_DISTANCE[enemyList[i]->getEnemyData()->type]);
		}
		float len = D3DXVec3Length(&(player[0].center - enemyList[i]->center));
		enemyList[i]->hearingSphere[0].render(enemyList[i]->matrixCenter);
		enemyList[i]->hearingSphere[1].render(enemyList[i]->matrixCenter);
#endif

	}

#endif// _DEBUG
}


//=============================================================================
// エネミーデータの作成
//=============================================================================
enemyNS::EnemyData* EnemyManager::createEnemyData(enemyNS::ENEMYSET enemySetting)
{
	// ENEMYSET構造体からEnemyDataを作る
	EnemyData enemyData;
	enemyData.enemyID = enemySetting.enemyID;
	enemyData.type = enemySetting.type;
	enemyData.defaultState = enemySetting.defaultState;
	enemyData.defaultPosition = enemySetting.defaultPosition;
	enemyData.defaultDirection = enemySetting.defaultDirection;
	enemyData.numRoute = enemySetting.numRoute;
	//enemyData.patrolRoute = new D3DXVECTOR3[enemyData.numRoute];
	//memcpy(enemyData.patrolRoute, (const void*)enemySetting.numRoute, sizeof(D3DXVECTOR3) * enemyData.numRoute);
	enemyData.setUp();
	// リストに追加
	enemyDataList.insertFront(enemyData);
	enemyDataList.listUpdate();
	// エネミーデータIDの最大値を更新（最後に追加したデータ + 1が次回発行IDとなる）
	nextID = enemyData.enemyID + 1;
	// リストに追加したデータのポインタを返す
	return enemyDataList.getValue(0);
}


//=============================================================================
// エネミーオブジェクトを作成
//=============================================================================
void EnemyManager::createEnemy(EnemyData* enemyData)
{
	Enemy* enemy = NULL;

	enemyNS::ConstructionPackage constructionPackage;
	constructionPackage.enemyData = enemyData;
	constructionPackage.gameMaster = gameMaster;
	constructionPackage.player = player;
	constructionPackage.attractorMesh = attractorMesh;
	constructionPackage.attractorMatrix = attractorMatrix;
	constructionPackage.enemyData->isObjectExists = true;

	switch (enemyData->type)
	{
	case WOLF:
		constructionPackage.staticMesh = staticMeshNS::reference(staticMeshNS::WOLF);
		enemy = new Wolf(constructionPackage);
		wolfRenderer->registerObject(enemy);
		wolfRenderer->updateAccessList();
		break;

	case TIGER:
		constructionPackage.staticMesh = staticMeshNS::reference(staticMeshNS::TIGER);
		enemy = new Tiger(constructionPackage);
		tigerRenderer->registerObject(enemy);
		tigerRenderer->updateAccessList();
		break;

	case BEAR:
		constructionPackage.staticMesh = staticMeshNS::reference(staticMeshNS::BEAR);
		enemy = new Bear(constructionPackage);
		bearRenderer->registerObject(enemy);
		tigerRenderer->updateAccessList();
		break;
	}

	enemyList.emplace_back(enemy);
}


//=============================================================================
// エネミーデータの破棄
//=============================================================================
void EnemyManager::destroyEnemyData(int _enemyID)
{
	for (int i = 0; i < enemyDataList.nodeNum; i++)
	{
		if (enemyDataList.getValue(i)->enemyID != _enemyID) { continue; }
		// エネミーデータ構造体はノード内にメモリが取られているため
		// ノードの破棄と合わせてエネミーデータも破棄されている
		enemyDataList.remove(enemyDataList.getNode(i));
		enemyDataList.listUpdate();
		break;
	}

#ifdef _DEBUG
	assertDestructionOrder();
#endif//_DEBUG
}


//=============================================================================
// 全てのエネミーデータを破棄
//=============================================================================
void EnemyManager::destroyAllEnemyData()
{
	// エネミーデータ構造体はノード内にメモリが取られているため
	// ノードの破棄と合わせてエネミーデータも破棄されている
	enemyDataList.allClear();
	enemyDataList.listUpdate();

#ifdef _DEBUG
	assertDestructionOrder();
#endif//_DEBUG
}


//=============================================================================
// エネミーオブジェクトを破棄その１
//=============================================================================
void EnemyManager::destroyEnemy(int _enemyID)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyData()->enemyID == _enemyID)
		{
			destroyEnemy(enemyList[i]);	
			enemyList.erase(enemyList.begin() + i);	// ベクター要素を消去
			break;
		}
	}
}


//=============================================================================
// エネミーオブジェクトを破棄その２（実際の破棄はこちら）
//=============================================================================
void EnemyManager::destroyEnemy(Enemy* enemy)
{
	// 描画の解除
	switch (enemy->getEnemyData()->type)
	{
	case WOLF:
		//wolfRenderer->updateAccessList();
		wolfRenderer->unRegisterObjectByID(enemy->id);
		break;

	case TIGER:
		//tigerRenderer->updateAccessList();
		tigerRenderer->unRegisterObjectByID(enemy->id);
		break;

	case BEAR:
		//bearRenderer->updateAccessList();
		bearRenderer->unRegisterObjectByID(enemy->id);
		break;
	}
	enemy->treeCell.remove();					// 衝突判定リストから削除
	SAFE_DELETE(enemy);							// インスタンス破棄
}


//=============================================================================
// 全てのエネミーオブジェクトを破棄
//=============================================================================
void EnemyManager::destroyAllEnemy()
{
	// 描画全解除
	wolfRenderer->allUnRegister();
	tigerRenderer->allUnRegister();
	bearRenderer->allUnRegister();

	for (size_t i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->treeCell.remove();		// 衝突判定リストから削除
		SAFE_DELETE(enemyList[i]);				// インスタンス破棄
	}
	enemyList.clear();
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
			if (enemyDataList.getValue(i)->enemyID == enemyList[k]->getEnemyData()->enemyID)
			{
				cnt++;
			}
		}
	}
	if (cnt != enemyList.size())
	{
		int unko = 1;
	}
	assert(cnt == enemyList.size());
	assert(enemyDataList.nodeNum >= enemyList.size());
}


//=============================================================================
// エネミーデータを探す
//=============================================================================
enemyNS::EnemyData* EnemyManager::findEnemyData(int _enemyID)
{
	for (int i = 0; i < enemyDataList.nodeNum; i++)
	{
		if (enemyDataList.getValue(i)->enemyID == _enemyID)
		{
			return enemyDataList.getValue(i);
		}
	}

	return NULL;
}


//=============================================================================
// エネミーを探す
//=============================================================================
Enemy* EnemyManager::findEnemy(int _enemyID)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyID() == _enemyID)
		{
			return enemyList[i];
		}
	}

	return NULL;
}


//=============================================================================
// エネミーIDを発行する
//=============================================================================
int EnemyManager::issueNewEnemyID()
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

	// フラグ
	bool createFlag = false;
	bool createDebugEnemy = false;
	bool returnPlayer = false;
	bool destroyAllFlag = false;

	if (ImGui::CollapsingHeader("EnemyInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("enemyDataList.nodeNum:%d\n", enemyDataList.nodeNum);
		ImGui::Text("enemyList.size()     :%d\n", enemyList.size());
		ImGui::Text("numOfEnemy           :%d\n", Enemy::getNumOfEnemy());
		ImGui::Text("nextID	:%d\n", nextID);
		//ImGui::Text("wolfRenderer.objectNum:%d\n", wolfRenderer->objectNum);
		//ImGui::Text("tigerRenderer.objectNum:%d\n", tigerRenderer->objectNum);
		//ImGui::Text("bearRenderer.objectNum:%d\n", bearRenderer->objectNum);

		// ボタン
		ImGui::Checkbox("Create Enemy", &createFlag);
		ImGui::Checkbox("Create Debug Enemy", &createDebugEnemy);
		ImGui::Checkbox("Return Player", &returnPlayer);
		ImGui::Checkbox("Delete All Enemy", &destroyAllFlag);

		// デバッグエネミー
		for (size_t i = 0; i < enemyList.size(); i++)
		{
			if (enemyList[i]->getEnemyID() == Enemy::debugEnemyID)
			{
				ImGui::Text("state:%d\n", enemyList[i]->getEnemyData()->state);
				ImGui::Text("[U]faceNumber:%d\n", enemyList[i]->faceNumber);

				if (enemyList[i]->edgeList == NULL) { continue; }
				for (int cnt = 0; cnt < enemyList[i]->edgeList->nodeNum; cnt++)
				{
					ImGui::Text("index(%d, %d)\n", enemyList[i]->edgeList->getValue(cnt)->index[0],
						enemyList[i]->edgeList->getValue(cnt)->index[1]);
				}	
			}
		}
	}

	if (createFlag)
	{
		enemyNS::ENEMYSET tmp =
		{
			issueNewEnemyID(),
			enemyNS::WOLF,
			stateMachineNS::PATROL,
			*player->getPosition(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			0
		};
		EnemyData* p = createEnemyData(tmp);
		createEnemy(p);
	}

	static D3DXQUATERNION playerRelativeQuaternion;	// プレイヤー注視カメラの相対位置ベクトル
	if (createDebugEnemy)
	{
		if (getEnemyList().empty() == false)
		{
			Enemy* debugEnemy = getEnemyList().back();
			debugEnemy->debugEnemyID = debugEnemy->getEnemyID();
			camera->setTarget(&debugEnemy->center);
			camera->setTargetX(&debugEnemy->getAxisX()->direction);
			camera->setTargetY(&debugEnemy->getAxisY()->direction);
			camera->setTargetZ(&debugEnemy->getAxisZ()->direction);
			playerRelativeQuaternion = camera->relativeQuaternion;
			camera->setRelative(D3DXQUATERNION(0.0f, 30.0f, -10.5f, 0.0f));
			debugEnemy->setCamera(&camera[0]);
			debugEnemy->setDebugEnvironment();
		}
	}

	if (returnPlayer)
	{
		camera->setTarget(player->getPosition());
		camera->setTargetX(&player->getAxisX()->direction);
		camera->setTargetY(&player->getAxisY()->direction);
		camera->setTargetZ(&player->getAxisZ()->direction);
		camera->setRelative(playerRelativeQuaternion = camera->relativeQuaternion);
	}

	if (destroyAllFlag)
	{
		destroyAllEnemy();
		destroyAllEnemyData();
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
int EnemyManager::getNextID() { return nextID; }

