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
	wolfRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::ENEMY_01));
	tigerRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON));
	bearRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON_X10));

#if 0	// エネミーツールのデータを読み込む
	ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
	playParameters = new PLAY_PARAMETERS[enemyTools->GetEnemyMax()*gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		for (int j = 0; j < gameMasterNS::PLAYER_NUM; j++)
		{
			//3Dサウンド
			switch (enemyTools->GetEnemySet(i).type)
			{
			case ENEMY_TYPE::WOLF:
				playParameters[(i*gameMasterNS::PLAYER_NUM) + j] = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, GAME_S3D_LIST::GAME_S3D_FOOTSTEP_01, true ,NULL,true,j };
				break;
			case ENEMY_TYPE::TIGER:
				playParameters[(i*gameMasterNS::PLAYER_NUM) + j] = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, GAME_S3D_LIST::GAME_S3D_FOOTSTEP_02, true ,NULL,true,j };
				break;
			case ENEMY_TYPE::BEAR:
				playParameters[(i*gameMasterNS::PLAYER_NUM) + j] = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, GAME_S3D_LIST::GAME_S3D_FOOTSTEP_03, true ,NULL,true,j };
				break;
			default:
				playParameters[(i*gameMasterNS::PLAYER_NUM) + j] = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, GAME_S3D_LIST::GAME_S3D_FOOTSTEP_03, true ,NULL,true,j };
				break;
			}
			SoundInterface::S3D->playSound(&playParameters[(i*gameMasterNS::PLAYER_NUM) + j]);
		}

		createEnemyData(enemyTools->GetEnemySet(i));
	}
	SAFE_DELETE(enemyTools);
#endif

#if 0	// エネミーオブジェクトをツールデータを元に作成する
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
			destroyEnemy(enemyList[i]->getEnemyData()->enemyID);
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
	enemyData.enemyID = enemySetting.enemyID;
	enemyData.type = enemySetting.type;
	enemyData.defaultState = enemySetting.defaultState;
	enemyData.defaultPosition = enemySetting.defaultPosition;
	enemyData.defaultDirection = enemySetting.defaultDirection;
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

	switch (enemyData->type)
	{
	case WOLF:
		enemy = new Wolf(staticMeshNS::reference(staticMeshNS::ENEMY_01), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		wolfRenderer->registerObject(enemy);
		break;

	case TIGER:
		enemy = new Tiger(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		tigerRenderer->registerObject(enemy);
		break;

	case BEAR:
		enemy = new Bear(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON_X10), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		bearRenderer->registerObject(enemy);
		break;
	}
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
// エネミーオブジェクトを破棄
//=============================================================================
void EnemyManager::destroyEnemy(int _enemyID)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyData()->enemyID == _enemyID)
		{
			// 描画の解除
			switch (enemyList[i]->getEnemyData()->type)
			{
			case WOLF:
				wolfRenderer->unRegisterObjectByID(enemyList[i]->id);
				break;

			case TIGER:
				tigerRenderer->unRegisterObjectByID(enemyList[i]->id);
				break;

			case BEAR:
				bearRenderer->unRegisterObjectByID(enemyList[i]->id);
				break;
			}
			SAFE_DELETE(enemyList[i]);				// インスタンス破棄
			enemyList.erase(enemyList.begin() + i);	// ベクター要素を消去
			break;
		}
	}
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
		SAFE_DELETE(enemyList[i]);
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

		// ボタン
		ImGui::Checkbox("Create Enemy", &createFlag);
		ImGui::Checkbox("Create Debug Enemy", &createDebugEnemy);
		ImGui::Checkbox("Return Player", &returnPlayer);
		ImGui::Checkbox("Delete All Enemy", &destroyAllFlag);
	}

	if (createFlag)
	{
		enemyNS::ENEMYSET tmp =
		{
			issueNewEnemyID(),
			enemyNS::WOLF,
			enemyNS::CHASE,
			*player->getPosition(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		};
		EnemyData* p = createEnemyData(tmp);
		createEnemy(p);
	}

	if (createDebugEnemy)
	{
		Enemy* debugEnemy = getEnemyList().back();
		debugEnemy->debugEnemyID = debugEnemy->getEnemyID();
		camera->setTarget(debugEnemy->getPosition());
		camera->setTargetX(&debugEnemy->getAxisX()->direction);
		camera->setTargetY(&debugEnemy->getAxisY()->direction);
		camera->setTargetZ(&debugEnemy->getAxisZ()->direction);
		debugEnemy->setDebugEnvironment();
		debugEnemy->setCamera(&camera[0]);
		debugEnemy->setDebugEnvironment();
	}

	if (returnPlayer)
	{
		camera->setTarget(player->getPosition());
		camera->setTargetX(&player->getAxisX()->direction);
		camera->setTargetY(&player->getAxisY()->direction);
		camera->setTargetZ(&player->getAxisZ()->direction);
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
	enemyList.reserve(ENEMY_OBJECT_MAX);		// update()で動的な確保をせず済むようメモリを増やしておく

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
// 終了処理(サウンド)
//=============================================================================
void EnemyManager::uninitializeSound()
{
	SoundInterface::S3D->uninitSoundStop();

	SAFE_DELETE_ARRAY(playParameters);
}

//=============================================================================
// 足音の処理
//=============================================================================
void EnemyManager::footsteps(D3DXVECTOR3 playerPos, int playerID)
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		float distance = D3DXVec3Length(&(enemyDataList.getValue(i)->position - playerPos));
		float volume = 0.0f;
		if (distance < DISTANCE_MAX)
		{
			volume = (DISTANCE_MAX - distance) / DISTANCE_MAX;
		}

		SoundInterface::S3D->SetVolume(playParameters[(i*gameMasterNS::PLAYER_NUM) + playerID], volume);
	}
}

//=============================================================================
// Getter
//=============================================================================
LinkedList<enemyNS::EnemyData>* EnemyManager::getEnemyDataList() { return &enemyDataList; }
std::vector<Enemy*>& EnemyManager::getEnemyList() { return enemyList; }
int EnemyManager::getNextID() { return nextID; }

