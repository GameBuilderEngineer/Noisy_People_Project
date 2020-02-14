#include "Equipment.h"



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
SWORD		PlayerSword;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEquipment(bool FirstInit)
{
	MOVEP *MoveP = GetMovePAdr();

	LPDIRECT3DDEVICE9 Device = getDevice();

	// 位置・回転・スケールの初期設定
	PlayerSword.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerSword.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerSword.Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	// 初めて初期化
	if (FirstInit == true)
	{
		PlayerSword.Texture = NULL;
		PlayerSword.MaterialBuffer = NULL;
		PlayerSword.Mesh = NULL;
		PlayerSword.MaterialNum = 0;


		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(Model_MovePSword,	// 読み込むモデルファイル名(Xファイル)
			D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
			Device,										// IDirect3DDevice9インターフェイスへのポインタ
			NULL,										// 隣接性データを含むバッファへのポインタ
			&PlayerSword.MaterialBuffer,				// マテリアルデータを含むバッファへのポインタ
			NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
			&PlayerSword.MaterialNum,					// D3DXMATERIAL構造体の数
			&PlayerSword.Mesh)))						// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}


		// モデル用のテクスチャのメモリ空間を配る
		PlayerSword.Texture = (LPDIRECT3DTEXTURE9*)calloc(PlayerSword.MaterialNum, sizeof(LPDIRECT3DTEXTURE9));
		if (PlayerSword.Texture == NULL)
		{
			MessageBox(0, "Alloc PlayerSword Texture Memory Failed！", "Error", 0);
			return E_FAIL;
		}

		// テクスチャを読み込む
		if (FAILED(SafeLoadModelTexture(PlayerSword.Texture, PlayerSword.MaterialBuffer, PlayerSword.MaterialNum, "PlayerSword")))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEquipment(void)
{
	int i = 0;

	// テクスチャの開放
	for (i = 0; i < (int)PlayerSword.MaterialNum; i++)
	{
		SAFE_RELEASE(PlayerSword.Texture[i]);
	}
	// マテリアルの開放
	SAFE_RELEASE(PlayerSword.MaterialBuffer);

	// メッシュの開放
	SAFE_RELEASE(PlayerSword.Mesh);


	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEquipment(void)
{

	MOVEP *MoveP = GetMovePAdr();
	D3DXMATRIX PlayerSwordMatrix;

	PlayerSwordMatrix = GetBoneMatrix(MoveP->Animation, "mixamorig_LeftHand");

	PlayerSword.Pos.x = PlayerSwordMatrix._41 ;
	PlayerSword.Pos.y = PlayerSwordMatrix._42  ;
	PlayerSword.Pos.z = PlayerSwordMatrix._43 ;

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEquipment(void)
{
	LPDIRECT3DDEVICE9 Device = getDevice();
	D3DXMATRIX ScaleMatrix, RotMatrix, TransMatrix;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 DefaultMat;
	D3DXMATRIX PlayerSwordMatrix;
	MOVEP *MoveP1 = GetMovePAdr();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&PlayerSword.WorldMatrix);

	// スケールを反映
	D3DXMatrixScaling(&ScaleMatrix, PlayerSword.Scale.x, PlayerSword.Scale.y, PlayerSword.Scale.z);
	D3DXMatrixMultiply(&PlayerSword.WorldMatrix, &PlayerSword.WorldMatrix, &ScaleMatrix);

	D3DXMatrixRotationQuaternion(&RotMatrix, &PlayerSword.Quaternion);
	D3DXMatrixMultiply(&PlayerSword.WorldMatrix, &PlayerSword.WorldMatrix, &RotMatrix);

	// 移動を反映
	D3DXMatrixTranslation(&TransMatrix, PlayerSword.Pos.x, PlayerSword.Pos.y, PlayerSword.Pos.z);
	D3DXMatrixMultiply(&PlayerSword.WorldMatrix, &PlayerSword.WorldMatrix, &TransMatrix);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &PlayerSword.WorldMatrix);

	// 現在のマテリアルを取得
	Device->GetMaterial(&DefaultMat);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)PlayerSword.MaterialBuffer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)PlayerSword.MaterialNum; nCntMat++)
	{
		// マテリアルの設定
		Device->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		Device->SetTexture(0, PlayerSword.Texture[nCntMat]);

		// 描画
		PlayerSword.Mesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	Device->SetMaterial(&DefaultMat);

	return;
}

//=============================================================================
// ポインタを取得する
//=============================================================================
SWORD *GetSword(const char* Owner)
{
	if (strcmp(Owner, "MoveP1") == 0)
	{
		return &PlayerSword;
	}
	else
	{
		MessageBox(0, "Can't Find Sword Owner", "Error", 0);
		return NULL;
	}
}

//=============================================================================
// モデルのテクスチャを読み込む関数
//=============================================================================
HRESULT SafeLoadModelTexture(LPDIRECT3DTEXTURE9	*Texture, LPD3DXBUFFER MaterialBufferconst, DWORD MaterialNum, const char* ErrorSrc)
{
	LPDIRECT3DDEVICE9 Device = getDevice();

	char Message[256];
	char TextureName[256];

	D3DXMATERIAL *pMtrls = (D3DXMATERIAL*)MaterialBufferconst->GetBufferPointer();

	for (int i = 0; i < (int)MaterialNum; i++)
	{
		Texture[i] = NULL;

		ZeroMemory(TextureName, sizeof(TextureName));
		sprintf_s(TextureName, "data/Model/%s", pMtrls[i].pTextureFilename);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(Device,		// デバイスへのポインタ
			TextureName,							// ファイルの名前
			&Texture[i]);							// 読み込むメモリー

	}

	return S_OK;
}
