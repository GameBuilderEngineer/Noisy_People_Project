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

	// 接地フィールド情報をセット
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// 描画オブジェクトの作成
	wolfRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	tigerRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	bearRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));

#if 0	// エネミーツールのデータを読み込む
	ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
	enemyDataList.reserve(enemyTools->GetEnemyMax());
	EnemyData enemyData;
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		enemyData.id = enemyTools->GetEnemySet(i).id;
		enemyData.type = enemyTools->GetEnemySet(i).type;
		enemyData.defaultState = enemyTools->GetEnemySet(i).defaultState;
		enemyData.defaultPosition = enemyTools->GetEnemySet(i).defaultPosition;
		enemyData.defaultDirection = enemyTools->GetEnemySet(i).defaultDirection;
		enemyData.setUp();
		enemyDataList.push_back(enemyData);
	}
	SAFE_DELETE(enemyTools);
	nextID = enemyData.id + 1;	// エネミーデータIDの最大値を更新
#endif

#if 0	// エネミーオブジェクトをデータを元に作成する
	enemyList.reserve(ENEMY_OBJECT_MAX);	// update()で動的な確保をせず済むようメモリを増やしておく
	for (size_t i = 0; i < enemyDataList.size(); i++)
	{
		if (1/* 本来はプレイヤーの初期位置と近ければ～など条件が付く */)
		{
			createEnemy(&enemyDataList[i]);
		}
	}
#endif
}


//=============================================================================
// 終了処理
//=============================================================================
void EnemyManager::uninitialize()
{
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
		wolfRenderer->registerObject(enemy);
		break;

	case TIGER:
		break;

	case BEAR:
		break;

	default:
		break;
	}
}


//=============================================================================
// エネミーオブジェクトの破棄
// ※使用不可
//=============================================================================
void EnemyManager::destroyEnemy(int _id)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyData()->id == _id)
		{
			switch (enemyList[i]->getEnemyData()->type)
			{
			case WOLF:
				wolfRenderer->unRegisterObjectByID(enemyList[i]->id);
				break;

			case TIGER:
				//tigerRenderer->
				break;

			case BEAR:
				break;

			default:
				break;

			}
			SAFE_DELETE(enemyList[i]);

			enemyList.erase(enemyList.begin() + i);
			break;
		}
	}
	// 今はStaticMeshRendererに登録されたオブジェクトを個別に破棄できない
	
}


//=============================================================================
// 全エネミーオブジェクトの破棄
// ※仮実装
//=============================================================================
void EnemyManager::destroyAllEnemy()
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		SAFE_DELETE(enemyList[i]);
	}

	wolfRenderer->allUnRegister();
	tigerRenderer->allUnRegister();
	bearRenderer->allUnRegister();

	enemyList.clear();
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
		//float limitTop = 1000;
		//float limitBottom = -1000;

		ImGui::Text("numOfEnemy:%d", Enemy::getNumOfEnemy());

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