//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 
//
//=============================================================================
#include "model.h"
#include "input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_CAR			"data/MODEL/airplane000.x"						// 読み込むモデル名
#define	MODEL_CAR2			"data/MODEL/ball.x"								// 読み込むモデル名
#define TEXTURE_FILENAME	"data/TEXTURE/カービー.dds"						// 読み込むテクスチャ
#define TEXTURE_FILENAME2	"data/TEXTURE/starfy.dds"						// 読み込むテクスチャ

#define	POS_CAMERA_P_X		(0.0f)											// カメラ視点の初期位置(X座標)
#define	POS_CAMERA_P_Y		(10.0f)											// カメラ視点の初期位置(Y座標)
#define	POS_CAMERA_P_Z		(-10.0f)										// カメラ視点の初期位置(Z座標)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z			(1.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(1000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE			(5.0f)											// 移動量
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)								// 回転量

#define	NUM_LIGHT			(3)												// ライトの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void InitLight(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureModel;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9	g_pD3DTextureModel2;	// テクスチャへのポインタ2（インターフェース）
LPD3DXMESH			g_pD3DXMeshModel;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatModel;	// マテリアル情報へのポインタ
DWORD				g_nNumMatModel;			// マテリアル情報の数

LPD3DXMESH			g_pD3DXMeshMask;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatMask;	// マテリアル情報へのポインタ
DWORD				g_nNumMatMask;			// マテリアル情報の数


D3DXVECTOR3			g_posModel;				// モデルの位置
D3DXVECTOR3			g_rotModel;				// モデルの向き(回転)
D3DXVECTOR3			g_sclModel;				// モデルの大きさ(スケール)

D3DXVECTOR3			g_posMask;				// モデルの位置
D3DXVECTOR3			g_rotMask;				// モデルの向き(回転)
D3DXVECTOR3			g_sclMask;				// モデルの大きさ(スケール)


D3DXVECTOR3			g_posCameraP;			// カメラの視点
D3DXVECTOR3			g_posCameraR;			// カメラの注視点
D3DXVECTOR3			g_vecCameraU;			// カメラの上方向ベクトル

D3DXMATRIX			g_mtxView;				// ビューマトリックス
D3DXMATRIX			g_mtxProjection;		// プロジェクションマトリックス
D3DXMATRIX			g_mtxWorld;				// ワールドマトリックス

D3DLIGHT9			g_aLight[NUM_LIGHT];	// ライト情報


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// 位置・回転・スケールの初期設定
	g_posModel = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// ●スケールをいじると法線がぶっ壊れるのでライトが機能しなくなる

	g_posMask = D3DXVECTOR3(0.0f,11.0f, 0.0f);
	g_rotMask = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclMask = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// ●スケールをいじると法線がぶっ壊れるのでライトが機能しなくなる


	g_pD3DTextureModel = NULL;
	g_pD3DXMeshModel = NULL;
	g_pD3DXBuffMatModel = NULL;
	g_nNumMatModel = 0;

	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(MODEL_CAR2,				// 読み込むモデルファイル名(Xファイル)
								D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
								pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
								NULL,					// 隣接性データを含むバッファへのポインタ
								&g_pD3DXBuffMatModel,	// マテリアルデータを含むバッファへのポインタ
								NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
								&g_nNumMatModel,		// D3DXMATERIAL構造体の数
								&g_pD3DXMeshModel)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}
	LPD3DXMESH			g_pD3DXMeshMask;		// メッシュ情報へのポインタ
	LPD3DXBUFFER		g_pD3DXBuffMatMask;	// マテリアル情報へのポインタ
	DWORD				g_nNumMatMask;			// マテリアル情報の数

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&g_pD3DXBuffMatMask,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMatMask,			// D3DXMATERIAL構造体の数
		&g_pD3DXMeshMask)))		// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}


#if 1
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,								// ファイルの名前
		&g_pD3DTextureModel);							// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME2,								// ファイルの名前
		&g_pD3DTextureModel2);							// 読み込むメモリー
#endif

	// 一旦必要ないから余計な処理を切る
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// ステージ0のカラー
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);// 色を掛け算してください
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	// ポリゴンの頂点の色を変えるのがライトのため、ポリゴンの色をここで設定していないとライトにならない

	// ステージ0のαも設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	// ステージ1のカラー
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);// 色を掛け算してください
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	// ポリゴンの頂点の色を変えるのがライトのため、ポリゴンの色をここで設定していないとライトにならない

	// ステージ1のαも設定
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	if(g_pD3DTextureModel != NULL)
	{// テクスチャの開放
		g_pD3DTextureModel->Release();
		g_pD3DTextureModel = NULL;
	}

	if(g_pD3DXMeshModel != NULL)
	{// メッシュの開放
		g_pD3DXMeshModel->Release();
		g_pD3DXMeshModel = NULL;
	}

	if(g_pD3DXBuffMatModel != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatModel->Release();
		g_pD3DXBuffMatModel = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
	g_rotModel.y += D3DXToRadian( 0.1f );

	g_posCameraR = g_posModel;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;



	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_mtxView, 
						&g_posCameraP,		// カメラの視点
						&g_posCameraR,		// カメラの注視点
						&g_vecCameraU);		// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
								VIEW_ANGLE,			// ビュー平面の視野角
								VIEW_ASPECT,		// ビュー平面のアスペクト比
								VIEW_NEAR_Z,		// ビュー平面のNearZ値
								VIEW_FAR_Z);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_sclModel.x, g_sclModel.y, g_sclModel.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posModel.x, g_posModel.y, g_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	//
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureModel);

		// 描画
		g_pD3DXMeshModel->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}


//=============================================================================
// マスクの描画
//=============================================================================
void DrawMask(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_mtxView,
		&g_posCameraP,		// カメラの視点
		&g_posCameraR,		// カメラの注視点
		&g_vecCameraU);		// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE,			// ビュー平面の視野角
		VIEW_ASPECT,		// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値
		VIEW_FAR_Z);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_sclMask.x, g_sclMask.y, g_sclMask.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMask.y, g_rotMask.x, g_rotMask.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posMask.x, g_posMask.y, g_posMask.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	//
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureModel);

		// 描画
		g_pD3DXMeshModel->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}


//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	g_posCameraP = D3DXVECTOR3(POS_CAMERA_P_X, POS_CAMERA_P_Y, POS_CAMERA_P_Z);
	g_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}


//=============================================================================
// ライトの初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;

	/* 0 */
	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ライト0のタイプの設定
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// ライト0の拡散光の設定
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

	// ライト0の環境光の設定
	g_aLight[0].Ambient = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

	// ライト0の方向の設定
	vecDir = D3DXVECTOR3(0.5f, -0.2f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ライト0をレンダリングパイプラインに設定
	pDevice->SetLight(0, &g_aLight[0]);

	// ライト0を使用使用状態に
	pDevice->LightEnable(0, TRUE);


	/* 1 */
	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	// ライト1のタイプの設定
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// ライト1の拡散光の設定
	g_aLight[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// ライト1の環境光の設定
	g_aLight[1].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    // ライト1の方向の設定
	vecDir = D3DXVECTOR3(-0.0f, -1.0f, 0.8f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// ライト1をレンダリングパイプラインに設定
	pDevice->SetLight(1, &g_aLight[1]);

	// ライト1の設定
	pDevice->LightEnable(1, TRUE);


	/* 2 */
	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));

	// ライト2のタイプの設定
	g_aLight[2].Type = D3DLIGHT_POINT;

	// ライト2の拡散光の設定
	g_aLight[2].Diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	// ライト2の環境光の設定
	g_aLight[2].Ambient = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	// ライト２のポジションを設定
	g_aLight[2].Position = D3DXVECTOR3(20.0f,20.0f, 0.0f);

	// ライト2の方向の設定
	vecDir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	g_aLight[2].Attenuation0 = 1.0f;	// Attenuation = 減衰　0, 1, 2で減衰の掛かり方が違う？　0が一番きれい。
	g_aLight[2].Range = 30.0f;			// 点光源の場合これがライトの距離になる模様　【これがないと光らない！】
	g_aLight[2].Falloff = 5.0f;			// よくわからん

	// ライト2をレンダリングパイプラインに設定
	pDevice->SetLight(2, &g_aLight[2]);

	// ライト2の設定
	pDevice->LightEnable(2, TRUE);


	// ライティングモードをON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

