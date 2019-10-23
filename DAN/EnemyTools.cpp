//===================================================================================================================================
//【EnemyTools.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#include "EnemyTools.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
ENEMY_TOOLS::ENEMY_TOOLS()
{
#ifdef _DEBUG
	//エネミーファイル構造体の初期化
	enemyFile = { 0 };
	enemyFile.efmt = nullptr;

	//パス
	setToolsDirectory();

	//ファイル
	FILE	*fp = NULL;
	fp = fopen(ENEMY_FILE_PATH, "rb");
	if (fp == NULL)	//ファイルが見つかりません
	{
		//エネミーファイルの新規作成
		CreatNewEnemyFile();
	}
	else
	{
		//ENMYの読み込み
		fread(&enemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);

		//メモリ確保とEFMTの読み込み
		if (enemyFile.enmy.enemyMax != NULL)
		{
			//メモリ確保
			enemyFile.efmt = new ENEMY_EFMT[enemyFile.enmy.enemyMax];

			for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
			{
				//EFMTの読み込み
				fread(&enemyFile.efmt[i], sizeof(ENEMY_EFMT), 1, fp);
			}
		}


		fclose(fp);

		//エネミー情報
		EnemyListboxCurrent = 0;
		EnemyListboxType = enemyNS::ENEMY_TYPE::WOLF;
		EnemyListboxState = enemyNS::ENEMY_STATE::CHASE;

		//描画用
		renderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
		initialize();
		for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
		{
			generate(D3DXVECTOR3(enemyFile.efmt[i].posX, enemyFile.efmt[i].posY, enemyFile.efmt[i].posZ));
			renderer->updateAccessList();
		}
	}
#endif
}

//===================================================================================================================================
//【エネミーファイルの新規作成】
//===================================================================================================================================
void ENEMY_TOOLS::CreatNewEnemyFile(void)
{
	//ファイル
	FILE	 *fp = NULL;
	fp = fopen(ENEMY_FILE_PATH, "wb");

	//エネミーファイル構造体
	ENEMY_FILE tmpEnemyFile;
	
	//チャンク
	memcpy(tmpEnemyFile.enmy.chunkId, ENMY_CHUNK, sizeof(tmpEnemyFile.enmy.chunkId));

	//エネミーの初期数
	tmpEnemyFile.enmy.enemyMax = 0;

	//ENMYのサイズ
	tmpEnemyFile.enmy.size = sizeof(ENEMY_ENMY);

	//書き出し
	fwrite(&tmpEnemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);

	fclose(fp);
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
ENEMY_TOOLS::~ENEMY_TOOLS()
{
#ifdef _DEBUG
	//ファイルの書き出し
	OutputEnemyFile();

	if (enemyFile.enmy.enemyMax != NULL)
	{
		SAFE_DELETE_ARRAY(enemyFile.efmt);
	}

	//描画用
	SAFE_DELETE(renderer);

#endif
}

//===================================================================================================================================
//【初期化】描画用
//===================================================================================================================================
void ENEMY_TOOLS::initialize()
{
	needUpdate = true;
};

//===================================================================================================================================
//【更新】描画用
//===================================================================================================================================
void ENEMY_TOOLS::update()
{
	if (!needUpdate)return;
	renderer->updateBuffer();
	renderer->updateArray();
	renderer->update();
	needUpdate = false;
}

//===================================================================================================================================
//【描画】描画用
//===================================================================================================================================
void ENEMY_TOOLS::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
}

//===================================================================================================================================
//【生成】描画用
//===================================================================================================================================
void ENEMY_TOOLS::generate(D3DXVECTOR3 position)
{
	Object* object = new Object();
	renderer->registerObject(object);
	object->existenceTimer = -1;		// < 0 なら消える
	object->initialize(&position);
}

//===================================================================================================================================
//【エネミーの種類を設定】
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemyType(short enemyId, short enemyType)
{
	enemyFile.efmt[enemyId].enemyType = enemyType;
}

//===================================================================================================================================
//【エネミーの状態を設定】
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemyState(short enemyId, short enemyState)
{
	enemyFile.efmt[enemyId].enemyState = enemyState;
}

//===================================================================================================================================
//【エネミーの位置を設定】
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemyPos(short enemyId, const D3DXVECTOR3 pos)
{
	enemyFile.efmt[enemyId].posX = pos.x;
	enemyFile.efmt[enemyId].posY = pos.y;
	enemyFile.efmt[enemyId].posZ = pos.z;
}

//===================================================================================================================================
//【エネミーの回転Y軸を設定】
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemyDir(short enemyId, const D3DXVECTOR3 dir)
{
	enemyFile.efmt[enemyId].dirX = dir.x;
	enemyFile.efmt[enemyId].dirY = dir.y;
	enemyFile.efmt[enemyId].dirZ = dir.z;
}

//===================================================================================================================================
//【エネミーの設置】
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemy(short enemyId, short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//チャンク
	memcpy(enemyFile.efmt[enemyId].chunkId, EFMT_CHUNK, sizeof(enemyFile.efmt[enemyId].chunkId));

	//エネミー情報
	SetEnemyType(enemyId, enemyType);
	SetEnemyState(enemyId, enemyState);
	SetEnemyPos(enemyId, pos);
	SetEnemyDir(enemyId, dir);

	//サイズ
	enemyFile.efmt[enemyId].size = (short)sizeof(ENEMY_EFMT);
}

//===================================================================================================================================
//【エネミーファイルの書き出し処理】
//===================================================================================================================================
void ENEMY_TOOLS::OutputEnemyFile(void)
{
	//パス
	setToolsDirectory();

	FILE	*fp = NULL;					//ファイル
	fp = fopen(ENEMY_FILE_PATH, "wb");

	//ENMYの書き出し
	fwrite(&enemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);

	//EFMTの書き出し
	for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
	{
		//EFMTの書き出し
		fwrite(&enemyFile.efmt[i], sizeof(ENEMY_EFMT), 1, fp);
	}

	fclose(fp);
}

//===================================================================================================================================
//【エネミーのフォーマット構造体を追加】
//===================================================================================================================================
void ENEMY_TOOLS::AddEnemyFormat(short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//エネミーの数+1
	enemyFile.enmy.enemyMax++;

	//エネミーのフォーマット構造体を整理
	UpdateEfmt(enemyFile.enmy.enemyMax - 1);

	//エネミーのフォーマット構造体の最後に追加
	SetEnemy(enemyFile.enmy.enemyMax - 1, enemyType, enemyState, pos, dir);

	//描画用
	generate(pos);
	renderer->updateAccessList();
	needUpdate = true;
}

//===================================================================================================================================
//【エネミーの最大数を取得】
//===================================================================================================================================
int ENEMY_TOOLS::GetEnemyMax(void)
{
	return enemyFile.enmy.enemyMax;
}

//===================================================================================================================================
//【エネミーのフォーマット構造体を整理】
//===================================================================================================================================
void ENEMY_TOOLS::UpdateEfmt(int oldEnemyMax)
{
	//バックアップ用
	ENEMY_EFMT *backUpEmft = new ENEMY_EFMT[enemyFile.enmy.enemyMax];

	//整理(enemyTypeはENEMY_TYPE::TYPE_MAXなら解放)
	int newEnemyMax = 0;	//中身があるefmt
	for (int i = 0; i < oldEnemyMax; i++)
	{
		if (enemyFile.efmt[i].enemyType == enemyNS::ENEMY_TYPE::TYPE_MAX)
		{
			//スキップ
			continue;
		}
		else
		{
			//バックアップを取る
			backUpEmft[newEnemyMax] = enemyFile.efmt[i];
			newEnemyMax++;
		}
	}

	//もし作り直す必要があれば
	if (oldEnemyMax > 0)
	{
		//メモリ解放
		SAFE_DELETE_ARRAY(enemyFile.efmt);
	}

	//メモリ確保し直す
	if (enemyFile.enmy.enemyMax > 0)
	{
		enemyFile.efmt = new ENEMY_EFMT[enemyFile.enmy.enemyMax];

		//ロードバックアップデータ
		for (int i = 0; i < newEnemyMax; i++)
		{
			enemyFile.efmt[i] = backUpEmft[i];
			enemyFile.efmt[i].enemyId = i;	//Idの更新
		}

		//ENMYの書き換え
		enemyFile.enmy.size = (short)(sizeof(ENEMY_ENMY) + (sizeof(ENEMY_EFMT) * enemyFile.enmy.enemyMax));
	}
	//メモリ解放
	SAFE_DELETE_ARRAY(backUpEmft);
}

//===================================================================================================================================
//【エネミーセット構造体を取得】
// 使い例:
// ENEMYSET *enemySet = new (ENEMYSET[GetEnemyMax()]);
// for(int i=0;i<GetEnemyMax();i++)
// {  
//   enemySet[i]=GetEnemySet(i);
// }
// ...
// SAFE_DELETE_ARRAY(enemySet);
//===================================================================================================================================
enemyNS::ENEMYSET ENEMY_TOOLS::GetEnemySet(short enemyId)
{
	enemyNS::ENEMYSET tmpEnemySet	= { 0 };
	tmpEnemySet.id					= enemyFile.efmt[enemyId].enemyId;
	tmpEnemySet.type				= enemyFile.efmt[enemyId].enemyType;
	tmpEnemySet.defaultState		= enemyFile.efmt[enemyId].enemyState;
	tmpEnemySet.defaultPosition		= D3DXVECTOR3(enemyFile.efmt[enemyId].posX, enemyFile.efmt[enemyId].posY, enemyFile.efmt[enemyId].posZ);
	tmpEnemySet.defaultDirection	= D3DXVECTOR3(enemyFile.efmt[enemyId].dirX, enemyFile.efmt[enemyId].dirY, enemyFile.efmt[enemyId].dirZ);
	
	return tmpEnemySet;
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void ENEMY_TOOLS::outputEnemyToolsGUI(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	//フラグ
	bool creatFlag = false;
	bool deleteFlag = false;
	
	ImGui::Begin("Enemy Tools");

	//機能
	ImGui::Checkbox("New Enemy", &creatFlag);
	ImGui::Checkbox("Delete", &deleteFlag);

	//エネミーの種類
	const char* listboxEnemyType[] = { "WOLF", "TIGER", "BEAR" };
	ImGui::ListBox("Enemy Type", &EnemyListboxType, listboxEnemyType, enemyNS::ENEMY_TYPE::TYPE_MAX);

	//エネミーの状態
	const char* listboxEnemyState[] = { "CHASE", "PATROL", "REST","DIE","DEAD" };
	ImGui::ListBox("Enemy State", &EnemyListboxState, listboxEnemyState, enemyNS::ENEMY_STATE::STATE_MAX);

	//エネミーの情報
	if (enemyFile.enmy.enemyMax != NULL)
	{
		//メモリ確保
		char **ListboxIndexName = new char*[enemyFile.enmy.enemyMax];
		for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
		{
			ListboxIndexName[i] = new char[CHAR_MAX];
			sprintf(ListboxIndexName[i], "%d", i);
		}

		ImGui::ListBox("Enemy ", &EnemyListboxCurrent, ListboxIndexName, enemyFile.enmy.enemyMax);
		ImGui::Text("Enemy pos:%f %f %f", enemyFile.efmt[EnemyListboxCurrent].posX,
			enemyFile.efmt[EnemyListboxCurrent].posY,
			enemyFile.efmt[EnemyListboxCurrent].posZ);
		ImGui::Text("Enemy dir:%f %f %f", enemyFile.efmt[EnemyListboxCurrent].dirX,
			enemyFile.efmt[EnemyListboxCurrent].dirY,
			enemyFile.efmt[EnemyListboxCurrent].dirZ);
		ImGui::Text("Enemy Type:%s  Enemy State:%s",
			listboxEnemyType[enemyFile.efmt[EnemyListboxCurrent].enemyType],
			listboxEnemyState[enemyFile.efmt[EnemyListboxCurrent].enemyState]);

		//メモリ解放
		for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
		{
			delete[] ListboxIndexName[i];
		}
		delete[] ListboxIndexName;
	}

	ImGui::End();

	//選択中


	//削除
	if (deleteFlag)
	{
		//削除
		DeleteEnemyFormat(EnemyListboxCurrent);
	
		//戻す
		EnemyListboxCurrent--;
		if (EnemyListboxCurrent < 0)
		{
			EnemyListboxCurrent = 0;
		}
	}

	//新規作成
	if (creatFlag)
	{
		//作成
		AddEnemyFormat(EnemyListboxType, EnemyListboxState, pos, dir);

		//進む
		EnemyListboxCurrent = enemyFile.enmy.enemyMax - 1;
	}

#endif
}
//
////===================================================================================================================================
////【エネミーツールの選択中を表す処理】
////===================================================================================================================================
//void ENEMY_TOOLS::SpinEnemy(short enemyId)
//{
//	//消す用(MAXなら消す)
//	for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
//	{
//		renderer->
//	}
//}

//===================================================================================================================================
//【エネミーのフォーマット構造体を消す】
//===================================================================================================================================
void ENEMY_TOOLS::DeleteEnemyFormat(short enemyId)
{
	//消す用(MAXなら消す)
	if (enemyFile.enmy.enemyMax != NULL)
	{
		//一つを消す
		enemyFile.enmy.enemyMax--;
		if (enemyFile.enmy.enemyMax < 0)
		{
			enemyFile.enmy.enemyMax = 0;
		}

		//TYPE_MAXなら消す
		enemyFile.efmt[enemyId].enemyType = enemyNS::ENEMY_TYPE::TYPE_MAX;

		//エネミーのフォーマット構造体を整理
		UpdateEfmt(enemyFile.enmy.enemyMax + 1);

		//描画用
		renderer->unRegisterObject(enemyFile.enmy.enemyMax - enemyId);
		renderer->updateAccessList();
		needUpdate = true;
	}
}
