//-----------------------------------------------------------------------------
// メタAIイベント生成クラス [EventMaker.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "EventMaker.h"
using namespace aiNS;


//=============================================================================
// 初期化
//=============================================================================
void EventMaker::initialize(aiNS::AnalyticalData* _data,
	OperationGenerator* _opeGenerator, GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
	TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager)
{
	data = _data;
	opeGenerator = _opeGenerator;
	gameMaster = _gameMaster;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;
}


//=============================================================================
// 終了処理
//=============================================================================
void EventMaker::uninitialize()
{
}


//=============================================================================
// 更新処理
//=============================================================================
void EventMaker::update()
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		// エネミーをプレイヤー周りにスポーンすべきか解析
		if (enemyManager->getEnemyList().size() < enemyNS::ENEMY_OBJECT_MAX &&				// 同時存在数チェック
			data->lastSpawnTime[i] - gameMaster->getGameTime() > MANDATORY_SPAWN_INTERVAL)	// スポーン間隔チェック
		{
			// ファジー論理演算 
			float tmp1 = fuzzy.reverseGrade((float)data->numChasingPlayer[i], 0.0f, 4.0f);		
			float tmp2 = fuzzy.reverseGrade((float)data->numKilledRecently, 0.0f, 7.0f);
			data->weightSpawn[i] = fuzzy.AND(tmp1, tmp2);

			// エネミー動的作成イベントの最終的な実行判断（プレイヤーごとに判断される）
			if (data->weightSpawn[i] > WEIGHT_SPAWN)
			{
				/*具体例 WEIGHT_SPAWN = 0.7のとき
					[1]
					エネミーに追跡されていない→	temp1 = 1.0  ◎
					エネミー1体に追跡されている→	temp1 = 0.75 ◎
					エネミー2体に追跡されている→	temp1 = 0.5　×
					[2]
					最近1体も倒していない→			temp2 = 1.0   ◎
					最近1体倒した→					temp2 = 0.857 ◎
					最近2体倒した→					temp2 = 0.714 ◎
					最近3体倒した→					temp2 = 0.571 ×
					[1][2]で低い方の数値で判定される
				*/
				makeEventSpawningEnemyAroundPlayer(i);
				data->lastSpawnTime[i] = gameMaster->getGameTime();
			}
		}
	}

	// デジタルツリー襲撃イベント
	if (data->lastTimeEnemyAttaksTree - gameMaster->getGameTime() > MANDATOEY_INTERVAL_ENEMY_ATTAKS_TREE)
	{// 最低経過時間チェック
		float tmp1 = fuzzy.reverseGrade(gameMaster->getGameTime(), 120.0f, 180.0f);
		float tmp2 = fuzzy.grade((float)(data->numDigital - data->numBeingAttackedTree), 6.0f, 20.0f);
		data->weightEnemyAttacksTree = fuzzy.OR(tmp1, tmp2);
		data->weightEnemyAttacksTree *= (float)(rand() % 5 - 2);	// ランダム補正

		if (data->lastTimeCheckedWeightEnemyAttacksTree - gameMaster->getGameTime() > MANDATOEY_INTERVAL_CHECKING_WEIGHT)
		{// 評価値（重み）のチェック間隔を空ける
			data->lastTimeCheckedWeightEnemyAttacksTree = gameMaster->getGameTime();
			if (data->weightEnemyAttacksTree > WEIGHT_ENEMY_ATTACKS_TREE)
			{
				data->lastTimeEnemyAttaksTree = gameMaster->getGameTime();
				makeEventEnemyAttaksTree();
			}
		}
	}
}


//=============================================================================
// エネミー動的作成イベントの作成（SPAWN_ENEMY_AROUND_PLAYER)
//=============================================================================
void EventMaker::makeEventSpawningEnemyAroundPlayer(int playerType)
{
	enemyNS::ENEMYSET enemySet;
	enemySet.enemyID = enemyManager->issueNewEnemyID();
	enemySet.type = decideSpawnEnemyType();
	enemySet.defaultPosition = /*player[playerType].position;*/ createGroundedPositionFromPivot(player[playerType].position, 25.0f, 5.0f);
	enemySet.defaultDirection = *player[playerType].getPosition() - enemySet.defaultPosition;
	enemySet.defaultState = stateMachineNS::PATROL;
	opeGenerator->spawnEnemy(enemySet);
}


//=============================================================================
// エネミーリスポーンイベントの作成（RESPAWN_ENEMY)
//=============================================================================
void EventMaker::makeEventRespawnEnemy()
{
	// リスポーンは今のところしない
}


//=============================================================================
// エネミーデジタルツリー襲撃イベントの作成（ENEMY_ATTACKS_TREE）
//=============================================================================
void EventMaker::makeEventEnemyAttaksTree()
{
	if (data->numDigital <= data->numBeingAttackedTree) { return; }

	// 襲撃ツリーを決める
	int treeID = decideAttackTargetTree();

	// 襲撃対象ツリーの準備
	Tree* attackTarget = NULL;
	D3DXVECTOR3 treePosition;
	std::vector<Tree*> treeList = treeManager->getTreeList();
	for (size_t i = 0; i < treeList.size(); i++)
	{
		if (treeList[i]->getTreeData()->treeID == treeID)
		{
			attackTarget = treeList[i];						// 攻撃対象ツリーを記録
			treeList[i]->getTreeData()->isAttaked = true;	// ツリーの攻撃中フラグをOn
			treePosition = treeList[i]->position;			// ツリーの座標を取得
		}
	}

	// エネミーの数を決める(2体もしくは3体)
	int numEnemy = 2 + rand() % 2;

	// エネミーのパラメータ作成
	for (int i = 0; i < numEnemy; i++)
	{
		enemyNS::ENEMYSET enemySet;
		enemySet.enemyID = enemyManager->issueNewEnemyID();
		enemySet.type = decideSpawnEnemyType();
		enemySet.defaultState = stateMachineNS::ATTACK_TREE;
		enemySet.defaultPosition = createGroundedPositionFromPivot(treePosition, 10.0f, 3.0f);
		enemySet.defaultDirection = treePosition - enemySet.defaultPosition;

		// 実行
		opeGenerator->enemyAttaksTree(enemySet, attackTarget);
	}	
}


//=============================================================================
// 動的作成するエネミーのパラメータを決める
//=============================================================================
enemyNS::ENEMY_TYPE EventMaker::decideSpawnEnemyType()
{
	enemyNS::ENEMY_TYPE ans;
	float score = 0.0f;

	// 最近倒した敵の数が多いほど高くなるようスコアをプロット
	score = fuzzy.grade((float)data->numKilledRecently, 0.0f, 5.0f);

	// プレイヤー2名のコンディションによってスコアを補正
	int sum = (data->playerCondition[gameMasterNS::PLAYER_1P] + data->playerCondition[gameMasterNS::PLAYER_2P]);
	switch (sum)
	{
	case 0:	// GOOD + GOOD
		score += 0.0;
		break;

	case 1:	// GOOD + NORMAL
		score += 0.1;
		break;

	case 2: // GOOD + BAD, NORMAL + NORMAL
		score += 0.3f;
		break;

	case 3:	// NORMAL + BAD
		score += 0.4f;
		break;

	case 4:	// BAD + BAD
		score += 0.5f;
		break;
	}

	// 乱数でスコアを撹拌
	score += (float)((rand() % 5) - 2);

	// スコアによってエネミーのタイプを変更する
	if (score > 0.2f)
	{
		ans = enemyNS::WOLF;
	}
	else
	{
		ans = enemyNS::TIGER;
	}

	return ans;
}


//=============================================================================
// 襲撃イベント対象デジタルツリーを選定する
//=============================================================================
int EventMaker::decideAttackTargetTree()
{
	using gameMasterNS::PLAYER_1P;
	using gameMasterNS::PLAYER_2P;
	using gameMasterNS::PLAYER_NUM;

	// 候補リストを作成（要素数 = デジタルツリーの数 - 例外ツリーの数 - 攻撃中ツリーの数）
	int candidateMax = data->numDigital /*- EXCEPTION_TREE_MAX●ここ注意*/ - data->numBeingAttackedTree;
	AttackCandidate* candidateList = new AttackCandidate[candidateMax];
	int cntCandidate = 0;

	// 候補リストを埋めていく
	std::vector<Tree*> treeList = treeManager->getTreeList();
	bool isPass;				// ループパスフラグ
	float maxDistance = 0.0f;	// 最大距離
	for (size_t i = 0; i < treeList.size(); i++)
	{
		isPass = false;
		//---------------------------------------------------------------------
		// アナログツリーと対象外のデジタルツリーに対して処理をパスする
		if (treeList[i]->getTreeData()->type != treeNS::DIGITAL_TREE) { isPass = true; }
		for (int k = 0; k < EXCEPTION_TREE_MAX; k++)
		{
			if (treeList[i]->getTreeData()->treeID == EXCEPTION_TREE_ID[k])
			{
				isPass = true;
			}
		}
		if (treeList[i]->getTreeData()->isAttaked) { isPass = true; }
		if (isPass) { continue; }
		//---------------------------------------------------------------------
		// IDを記録
		candidateList[cntCandidate].treeID = treeList[i]->getTreeData()->treeID;
		// 距離を測る
		float distanceToPlayer[PLAYER_NUM];
		for (int k = 0; k < PLAYER_NUM; k++)
		{
			distanceToPlayer[k] = between2VectorLength(player[k].position, treeList[i]->getTreeData()->initialPosition);
		}
		// 近いプレイヤーとの距離を保管
		if (distanceToPlayer[PLAYER_1P] < distanceToPlayer[PLAYER_2P])
		{
			candidateList[cntCandidate].distanceToNearPlayer = distanceToPlayer[PLAYER_1P];
		}
		else
		{
			candidateList[cntCandidate].distanceToNearPlayer = distanceToPlayer[PLAYER_2P];
		}
		// 近いプレイヤーとの距離の中で最高距離を更新
		if (candidateList[cntCandidate].distanceToNearPlayer > maxDistance)
		{
			maxDistance = candidateList[cntCandidate].distanceToNearPlayer;
		}
		cntCandidate++;
	}

	// 候補のスコアを算出し規定値以上であれば選択リストに加える
	int* selectIndexList = new int[candidateMax];
	int cntSelectIndex = 0;
	for (int i = 0; i < candidateMax; i++)
	{
		candidateList[i].score = fuzzy.grade(candidateList[i].distanceToNearPlayer, 0.0f, maxDistance);
		if (candidateList[i].score > 0.7)
		{
			selectIndexList[cntSelectIndex++] = i;
		}
	}

	// 選択リストからランダムで候補リストのインデックスを選びツリーを選定する
	// 決まったツリーのIDを返却する

	int resultIndex = rand() % cntSelectIndex;// ●リリース版でゼロ除算が起きてバグった12/1エネミーを通り過ぎたとき？

	int resultCandidate = selectIndexList[resultIndex];
	int treeID = candidateList[resultCandidate].treeID;
	SAFE_DELETE_ARRAY(selectIndexList);
	SAFE_DELETE_ARRAY(candidateList);
	return treeID;
}


//=============================================================================
// 基準座標を基に接地座標を作成する
//=============================================================================
D3DXVECTOR3 EventMaker::createGroundedPositionFromPivot(D3DXVECTOR3 pivot, float distance, float reduction)
{
	D3DXVECTOR3 tempDirection;	// 方角
	D3DXVECTOR3 ans;			// 求める座標
	NavigationMesh* naviMesh = NavigationMesh::getNaviMesh();
	bool isFinish = false;
	float distanceToGround = 0.0f;
	int cnt = 0;

	while (isFinish == false)
	{//方向を変えて接地するか確認するループ
		tempDirection = createRandomDirectionXZ();

		while (distance != 1.0f)
		{// 距離を動かして接地するか確認するループ
			ans = pivot + tempDirection * distance;

			// 接地確認
			static float RAY_ADDITIONAL_HEIGHT = 20.0f;
			if (naviMesh->isHitGrounding(&distanceToGround, NULL, ans + D3DXVECTOR3(0, RAY_ADDITIONAL_HEIGHT, 0)))
			{
				// Y座標を地面の高さに修正
				if (distanceToGround < RAY_ADDITIONAL_HEIGHT)
				{
					ans.y += (RAY_ADDITIONAL_HEIGHT - distanceToGround);
				}
				else
				{
					ans.y -= (distanceToGround - RAY_ADDITIONAL_HEIGHT);
				}

				isFinish = true;
				break;
			}
			else
			{
				distance -= reduction;
				if (distance < 1.0f) { distance = 1.0f; }
			}
		}
		cnt++;

		if (cnt > 5/*この回数方向を変えたら終了する*/)
		{
			isFinish = true;
			ans = pivot;
		}
	}

	return ans;
}


//=============================================================================
// XZ平面上のベクトル(長さ1.0)をランダムに作成する
//=============================================================================
D3DXVECTOR3 EventMaker::createRandomDirectionXZ()
{
	// 方向
	D3DXVECTOR3 direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	// 回転させる
	D3DXQUATERNION quaternion;
	D3DXQuaternionIdentity(&quaternion);
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	D3DXVECTOR3 axisY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	anyAxisRotation(&quaternion, axisY, rand() % 361);
	D3DXMatrixRotationQuaternion(&matrix, &quaternion);
	D3DXVec3TransformCoord(&direction, &direction, &matrix);

	return direction;
}