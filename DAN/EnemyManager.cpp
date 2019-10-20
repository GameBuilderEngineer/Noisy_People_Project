//-----------------------------------------------------------------------------
// エネミー管理クラス [Enemy.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
#include "EnemyManager.h"
#include "ImguiManager.h"
using namespace enemyNS;


//=============================================================================
// 初期化
//=============================================================================
void EnemyManager::initialize()
{
#if 0
	// エネミーファイルからエネミー初期ステータスを読み込む
	FILE	*fp = NULL;
	fopen("enemy.enemy", "rb");
	ENEMY_FILE enemyFile;
	memset(&enemyFile, 0, sizeof(ENEMY_FILE));
	fread(&enemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);
	enemyFile.efmt = new ENEMY_EFMT[enemyFile.enmy.enemyMax];
	fread(&enemyFile.efmt, sizeof(ENEMY_EFMT), enemyFile.enmy.enemyMax, fp);
	fclose(fp);

	// エネミーデータリストの追加
	EnemyData enemyData;
	enemyDataList.reserve(enemyFile.enmy.enemyMax + DYNAMIC_SPAWN_MAX);
	for (int i = 0; i < enemyDataList.size(); i++)
	{
		enemyData.zeroClear();
		if (i >= enemyFile.enmy.enemyMax) { continue; }

		//enemyFile.efmt[i].chunkId;
		//enemyFile.efmt[i].posX;

		enemyDataList.push_back(enemyData);
	}
	SAFE_DELETE_ARRAY(enemyFile.efmt);
	nextID = enemyDataList.back().id + 1;	// エネミーデータIDの最大値を更新
#endif

	// update()で動的な確保をせず済むようエネミーリストのキャパシティ（メモリ）を増やしておく
	enemyList.reserve(ENEMY_OBJECT_MAX);

	// エネミーオブジェクトの作成
	for (size_t i = 0; enemyDataList.size(); i++)
	{
		if (/* プレイヤーの初期位置と近ければ */0)
		{
			createEnemy(&enemyDataList[i]);
		}
	}

	// 描画オブジェクトの作成
	wolfRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	tigerRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	bearRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
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
			SAFE_DELETE(enemyList[i]);
			enemyList.erase(enemyList.begin() + i);
			break;
		}
	}
	// 今はStaticMeshObjectに登録されたオブジェクトを個別に破棄できない
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

	enemyList.clear();
	SAFE_DELETE(wolfRenderer);
	SAFE_DELETE(tigerRenderer);
	SAFE_DELETE(bearRenderer);
	wolfRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	tigerRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	bearRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
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