//===================================================================================================================================
//【MpojTools.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#include "MapObjectTool.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
MPOJ_TOOLS::MPOJ_TOOLS()
{
	//マップオブジェクトファイル構造体の初期化
	mpojFile = { 0 };
	mpojFile.mfmt = nullptr;

	//パス
	setToolsDirectory();

#ifdef _DEBUG
	//レンダラーの初期化
	initRender();
#endif

	//ファイル
	FILE	*fp = NULL;
	fp = fopen(MPOJ_FILE_PATH, "rb");
	if (fp == NULL)	//ファイルが見つかりません
	{
		//マップオブジェクトファイルの新規作成
		CreatNewMpojFile();
	}
	else
	{
		//MPOJの読み込み
		fread(&mpojFile.mpoj, sizeof(MPOJ_MPOJ), 1, fp);

		//メモリ確保とMFMTの読み込み
		if (mpojFile.mpoj.mpojMax != NULL)
		{
			//メモリ確保
			mpojFile.mfmt = new MPOJ_MFMT[mpojFile.mpoj.mpojMax];

			for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
			{
				//MFMTの読み込み
				fread(&mpojFile.mfmt[i], sizeof(MPOJ_MFMT), 1, fp);
			}
		}

		//ファイル
		fclose(fp);

#ifdef _DEBUG
		//オブジェクトの初期化
		initObject();
#endif
	}

#ifdef _DEBUG
	//マップオブジェクト情報
	MpojListboxCurrent = 0;
	MpojListboxType = mapObjectNS::MAPOBJECT_TYPE::STONE_01;
	Model[mapObjectNS::MAPOBJECT_TYPE::STONE_01] = staticMeshNS::STONE_003;
	Model[mapObjectNS::MAPOBJECT_TYPE::STONE_02] = staticMeshNS::STONE_003;
	Model[mapObjectNS::MAPOBJECT_TYPE::STONE_03] = staticMeshNS::STONE_003;

	//レンダラーのリセット処理
	ResetRenderer();

#endif
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
MPOJ_TOOLS::~MPOJ_TOOLS()
{
	//ファイルの書き出し
	OutputMpojFile();

	if (mpojFile.mpoj.mpojMax != NULL)
	{
		SAFE_DELETE_ARRAY(mpojFile.mfmt);
#ifdef _DEBUG
		for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
		{
			SAFE_DELETE(object[i]);
		}
		SAFE_DELETE_ARRAY(object);
		SAFE_DELETE_ARRAY(bodyCollide);
#endif
	}

#ifdef _DEBUG
	//レンダラー
	for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
	{
		SAFE_DELETE(mapobjRenderer[i]);
	}
	SAFE_DELETE_ARRAY(mapobjRenderer);
#endif
}

//===================================================================================================================================
//【マップオブジェクトの最大数を取得】
//===================================================================================================================================
int MPOJ_TOOLS::GetMpojMax(void)
{
	return mpojFile.mpoj.mpojMax;
}

//===================================================================================================================================
//【マップオブジェクト構造体を取得】
//===================================================================================================================================
mapObjectNS::MAPOBJECTSET MPOJ_TOOLS::GetMpojSet(short mpojId)
{
	mapObjectNS::MAPOBJECTSET tmpMpojSet = { 0 };
	tmpMpojSet.mapObjectID = mpojFile.mfmt[mpojId].mpojId;
	tmpMpojSet.type = (mapObjectNS::MAPOBJECT_TYPE)mpojFile.mfmt[mpojId].mpojType;
	tmpMpojSet.defaultPosition = D3DXVECTOR3(mpojFile.mfmt[mpojId].posX, mpojFile.mfmt[mpojId].posY, mpojFile.mfmt[mpojId].posZ);
	tmpMpojSet.defaultDirection = D3DXVECTOR3(mpojFile.mfmt[mpojId].dirX, mpojFile.mfmt[mpojId].dirY, mpojFile.mfmt[mpojId].dirZ);

	return tmpMpojSet;
}

#ifdef _DEBUG
//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void MPOJ_TOOLS::outputMpojToolsGUI(int GUIid, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	if (GUIid == MPOJ_GUI_ID)
	{
		//フラグ
		bool creatFlag = false;
		bool deleteFlag = false;

		//機能
		ImGui::Checkbox("New Mpoj", &creatFlag);
		ImGui::Checkbox("Delete", &deleteFlag);

		//マップオブジェクトの種類
		const char* listboxMpojType[] = { "STONE_003" ,"STONE_003", "STONE_003"};
		ImGui::ListBox("Mpoj Type", &MpojListboxType, listboxMpojType, mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX);

		//マップオブジェクトの情報
		if (mpojFile.mpoj.mpojMax != NULL)
		{
			//メモリ確保
			char **ListboxIndexName = new char*[mpojFile.mpoj.mpojMax];
			for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
			{
				ListboxIndexName[i] = new char[CHAR_MAX];
				sprintf(ListboxIndexName[i], "%d", i);
			}

			ImGui::ListBox("Mpoj ", &MpojListboxCurrent, ListboxIndexName, mpojFile.mpoj.mpojMax);
			ImGui::Text("ITem ID:%d", mpojFile.mfmt[MpojListboxCurrent].mpojId);
			ImGui::Text("Mpoj pos:%f %f %f", mpojFile.mfmt[MpojListboxCurrent].posX,
				mpojFile.mfmt[MpojListboxCurrent].posY,
				mpojFile.mfmt[MpojListboxCurrent].posZ);
			ImGui::Text("Mpoj dir:%f %f %f", mpojFile.mfmt[MpojListboxCurrent].dirX,
				mpojFile.mfmt[MpojListboxCurrent].dirY,
				mpojFile.mfmt[MpojListboxCurrent].dirZ);
			ImGui::Text("Mpoj Type:%s",
				listboxMpojType[mpojFile.mfmt[MpojListboxCurrent].mpojType]);
			//メモリ解放
			for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
			{
				delete[] ListboxIndexName[i];
			}
			delete[] ListboxIndexName;
		}

		//削除
		if (deleteFlag)
		{
			//削除
			DeleteMpojFormat(MpojListboxCurrent);
		}

		//新規作成
		if (creatFlag)
		{
			//作成
			AddMpojFormat(MpojListboxType, pos, dir);
		}
	}
#endif
}

//===================================================================================================================================
//【レンダラーの初期化】描画用
//===================================================================================================================================
void MPOJ_TOOLS::initRender()
{
	//レンダーの初期化
	mapobjRenderer = new StaticMeshRenderer*[mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX];
	for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
	{
		mapobjRenderer[i] = new StaticMeshRenderer(staticMeshNS::reference(Model[i]));
	}

	//更新フラグ
	needUpdate = true;
};

//===================================================================================================================================
//【オブジェクトの初期化】描画用
//===================================================================================================================================
void MPOJ_TOOLS::initObject()
{
	//オブジェクト
	object = new Object*[mpojFile.mpoj.mpojMax];
	bodyCollide = new BoundingSphere[mpojFile.mpoj.mpojMax];
	for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
	{
		//オブジェクトの生成
		object[i] = new Object();
		generateMapobj(object[i], mpojFile.mfmt[i].mpojType,
			D3DXVECTOR3(mpojFile.mfmt[i].posX, mpojFile.mfmt[i].posY, mpojFile.mfmt[i].posZ),
			D3DXVECTOR3(mpojFile.mfmt[i].dirX, mpojFile.mfmt[i].dirY, mpojFile.mfmt[i].dirZ));
		mapobjRenderer[mpojFile.mfmt[i].mpojType]->updateAccessList();

		// コライダの初期化
		bodyCollide->initialize(&D3DXVECTOR3(mpojFile.mfmt[i].posX, mpojFile.mfmt[i].posY, mpojFile.mfmt[i].posZ),
			staticMeshNS::reference(Model[mpojFile.mfmt[i].mpojType])->mesh);
	}
}

//===================================================================================================================================
//【更新】描画用
//===================================================================================================================================
void MPOJ_TOOLS::update()
{
	if (!needUpdate)return;
	for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
	{
		mapobjRenderer[i]->updateBuffer();
		mapobjRenderer[i]->updateArray();
		mapobjRenderer[i]->update();
	}

	needUpdate = false;
}

//===================================================================================================================================
//【描画】描画用
//===================================================================================================================================
void MPOJ_TOOLS::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (mpojFile.mpoj.mpojMax != NULL)
	{
		for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
		{
			mapobjRenderer[i]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
		}
	}
}

//===================================================================================================================================
//【生成】描画用
//===================================================================================================================================
void MPOJ_TOOLS::generateMapobj(Object *object, short mpojType, D3DXVECTOR3 position, D3DXVECTOR3 dir)
{
	object->initialize(&position);
	object->postureControl(object->axisZ.direction, dir, 1.0f);
	object->existenceTimer = 1.0f;		// < 0 なら消える
	mapobjRenderer[mpojType]->registerObject(object);
}
#endif

//===================================================================================================================================
//【マップオブジェクトファイルの書き出し処理】
//===================================================================================================================================
void MPOJ_TOOLS::OutputMpojFile(void)
{
	//パス
	setToolsDirectory();

	FILE	*fp = NULL;					//ファイル
	fp = fopen(MPOJ_FILE_PATH, "wb");

	//MPOJの書き出し
	fwrite(&mpojFile.mpoj, sizeof(MPOJ_MPOJ), 1, fp);

	//MFMTの書き出し
	for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
	{
		//MFMTの書き出し
		fwrite(&mpojFile.mfmt[i], sizeof(MPOJ_MFMT), 1, fp);
	}

	fclose(fp);
}

//===================================================================================================================================
//【マップオブジェクトファイルの新規作成】
//===================================================================================================================================
void MPOJ_TOOLS::CreatNewMpojFile(void)
{
	//ファイル
	FILE	 *fp = NULL;
	fp = fopen(MPOJ_FILE_PATH, "wb");

	//マップオブジェクトファイル構造体
	MPOJ_FILE tmpEnemyFile;

	//チャンク
	memcpy(tmpEnemyFile.mpoj.chunkId, MPOJ_CHUNK, sizeof(tmpEnemyFile.mpoj.chunkId));

	//マップオブジェクトの初期数
	tmpEnemyFile.mpoj.mpojMax = 0;

	//ENMYのサイズ
	tmpEnemyFile.mpoj.size = sizeof(MPOJ_MPOJ);

	//書き出し
	fwrite(&tmpEnemyFile.mpoj, sizeof(MPOJ_MPOJ), 1, fp);

	//ファイル
	fclose(fp);
}

#ifdef _DEBUG
//===================================================================================================================================
//【レンダラーのリセット処理】描画用
//===================================================================================================================================
void MPOJ_TOOLS::ResetRenderer(void)
{
	//終了処理
	for (int i = 0; i < mpojFile.mpoj.mpojMax - 1; i++)
	{
		SAFE_DELETE(object[i]);
	}
	SAFE_DELETE_ARRAY(object);

	if (mpojFile.mpoj.mpojMax != NULL)
	{
		for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
		{
			SAFE_DELETE(mapobjRenderer[i]);
		}
		SAFE_DELETE_ARRAY(mapobjRenderer);

		//レンダラーの初期化
		initRender();

		//オブジェクトの初期化
		initObject();
	}
}

//===================================================================================================================================
//【マップオブジェクトの種類を設定】
//===================================================================================================================================
void MPOJ_TOOLS::SetMpojType(short mpojId, short mpojType)
{
	mpojFile.mfmt[mpojId].mpojType = mpojType;
}

//===================================================================================================================================
//【マップオブジェクトの位置を設定】
//===================================================================================================================================
void MPOJ_TOOLS::SetMpojPos(short mpojId, const D3DXVECTOR3 pos)
{
	mpojFile.mfmt[mpojId].posX = pos.x;
	mpojFile.mfmt[mpojId].posY = pos.y;
	mpojFile.mfmt[mpojId].posZ = pos.z;
}

//===================================================================================================================================
//【マップオブジェクトの回転軸を設定】
//===================================================================================================================================
void MPOJ_TOOLS::SetMpojDir(short mpojId, const D3DXVECTOR3 dir)
{
	mpojFile.mfmt[mpojId].dirX = dir.x;
	mpojFile.mfmt[mpojId].dirY = dir.y;
	mpojFile.mfmt[mpojId].dirZ = dir.z;
}

//===================================================================================================================================
//【マップオブジェクトの設置】
//===================================================================================================================================
void MPOJ_TOOLS::SetMpoj(short mpojId, short mpojType,
	 const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//チャンク
	memcpy(mpojFile.mfmt[mpojId].chunkId, MFMT_CHUNK, sizeof(mpojFile.mfmt[mpojId].chunkId));

	//マップオブジェクト情報
	SetMpojType(mpojId, mpojType);
	SetMpojPos(mpojId, pos);
	SetMpojDir(mpojId, dir);

	//サイズ
	mpojFile.mfmt[mpojId].size = (short)sizeof(MPOJ_MFMT);
	//ID
	mpojFile.mfmt[mpojId].mpojId = mpojId;
}

//===================================================================================================================================
//【マップオブジェクトのフォーマット構造体を整理】
//===================================================================================================================================
void MPOJ_TOOLS::UpdateMfmt(int oldMpojMax)
{
	//バックアップ用
	MPOJ_MFMT *backUpMfmt = new MPOJ_MFMT[mpojFile.mpoj.mpojMax];

	//整理(mpojTypeはMPOJ_TYPE::MPOJ_TYPE_MAXなら解放)
	int newMpojMax = 0;	//中身があるmfmt
	for (int i = 0; i < oldMpojMax; i++)
	{
		if (mpojFile.mfmt[i].mpojType == mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX)
		{
			//スキップ
			continue;
		}
		else
		{
			//バックアップを取る
			backUpMfmt[newMpojMax] = mpojFile.mfmt[i];
			newMpojMax++;
		}
	}

	//もし作り直す必要があれば
	if (oldMpojMax > 0)
	{
		//メモリ解放
		SAFE_DELETE_ARRAY(mpojFile.mfmt);
	}

	//メモリ確保し直す
	if (mpojFile.mpoj.mpojMax > 0)
	{
		mpojFile.mfmt = new MPOJ_MFMT[mpojFile.mpoj.mpojMax];

		//ロードバックアップデータ
		for (int i = 0; i < newMpojMax; i++)
		{
			mpojFile.mfmt[i] = backUpMfmt[i];
			mpojFile.mfmt[i].mpojId = i;	//Idの更新
		}

		//MPOJの書き換え
		mpojFile.mpoj.size = (short)(sizeof(MPOJ_MPOJ) + (sizeof(MPOJ_MFMT) * mpojFile.mpoj.mpojMax));
	}
	//メモリ解放
	SAFE_DELETE_ARRAY(backUpMfmt);
}

//===================================================================================================================================
//【マップオブジェクトのフォーマット構造体を消す】
//===================================================================================================================================
void MPOJ_TOOLS::DeleteMpojFormat(short mpojId)
{
	//消す用(MAXなら消す)
	if (mpojFile.mpoj.mpojMax != NULL)
	{
		//マップオブジェクトの数-1
		mpojFile.mpoj.mpojMax--;
		if (mpojFile.mpoj.mpojMax < 0)
		{
			mpojFile.mpoj.mpojMax = 0;
		}

		//TYPE_MAXなら消す
		mpojFile.mfmt[mpojId].mpojType = mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX;

		//マップオブジェクトのフォーマット構造体を整理
		UpdateMfmt(mpojFile.mpoj.mpojMax + 1);
	}

	//戻す
	MpojListboxCurrent--;
	if (MpojListboxCurrent < 0)
	{
		MpojListboxCurrent = 0;
	}

	//ファイルのアップデート
	OutputMpojFile();

	//レンダラーのリセット処理
	ResetRenderer();
}

//===================================================================================================================================
//【マップオブジェクトのフォーマット構造体を追加】
//===================================================================================================================================
void MPOJ_TOOLS::AddMpojFormat(short mpojType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//マップオブジェクトの数+1
	mpojFile.mpoj.mpojMax++;

	//マップオブジェクトのフォーマット構造体を整理
	UpdateMfmt(mpojFile.mpoj.mpojMax - 1);

	//マップオブジェクトのフォーマット構造体の最後に追加
	SetMpoj(mpojFile.mpoj.mpojMax - 1, mpojType,
	 pos, dir);

	//進む
	MpojListboxCurrent = mpojFile.mpoj.mpojMax - 1;

	//ファイルのアップデート
	OutputMpojFile();

	//レンダラーのリセット処理
	ResetRenderer();
}
#endif