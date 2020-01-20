#include "tmpOcean.h"
#include "Direct3D9.h"


TmpOcean::TmpOcean()
{
	setSomethingDirectory("\\tmp");

	pDevice = getDevice();

	D3DXMatrixIdentity(&mScaling);
	D3DXMatrixIdentity(&mWorld);

	//頂点宣言
	D3DVERTEXELEMENT9 decl[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,	0},
		{0, 24, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,	0},
		{0, 36, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};	
	//頂点宣言の作成
	if (FAILED(pDevice->CreateVertexDeclaration(decl, &g_pDecl)))
	{
		MessageBox(NULL, "CreateVertexDeclaration失敗しました", NULL, MB_OK);
	}

	// メッシュを作成する
	// メッシュの初期位置
	memcpy(&vPosition, D3DXVECTOR3(0, 0, 0), sizeof(D3DXVECTOR3));
	// Xファイルからメッシュをロードする	
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;
	if (FAILED(D3DXLoadMeshFromX("Water16.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", "Water4.x", MB_OK);
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMeshMaterials	= new D3DMATERIAL9[dwNumMaterials];
	pMeshTextures	= new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pMeshMaterials[i]			= d3dxMaterials[i].MatD3D;
		pMeshMaterials[i].Ambient	= pMeshMaterials[i].Diffuse;
		pMeshTextures[i]			= NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[i].pTextureFilename,
				&pMeshTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();


	//接線情報を追加
	LPD3DXMESH cloneMesh;
	if (FAILED(pMesh->CloneMesh(pMesh->GetOptions(), decl, pDevice, &cloneMesh)))
	{
		MessageBox(NULL, "CloneMesh", NULL, MB_OK);
	}

	if (FAILED(D3DXComputeNormals(cloneMesh, NULL)))
	{
		MessageBox(NULL, "D3DXComputeNormals", NULL, MB_OK);
	}

	if (FAILED(D3DXComputeTangent(cloneMesh, 0, 0,
		D3DX_DEFAULT,//従法線が無い場合、April2005では、このように指定しないと失敗する(D3DX_DEFAULT = -1)
		true, NULL)))
	{
		MessageBox(NULL, "D3DXComputeTangent", NULL, MB_OK);
	}

	SAFE_RELEASE(pMesh);
	pMesh = cloneMesh;


	//バンプマッピング用のテクスチャイメージを読み込む
	LPDIRECT3DTEXTURE9	pHeightTexture;
	D3DSURFACE_DESC desc;

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "WaterBump.tga", &pHeightTexture)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}
	pHeightTexture->GetLevelDesc(0, &desc);
	if (FAILED(D3DXCreateTexture(pDevice, desc.Width, desc.Height, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &g_pBumpTex)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}
	if (FAILED(D3DXComputeNormalMap(g_pBumpTex, pHeightTexture, NULL, 0, D3DX_CHANNEL_LUMINANCE, 20.0f)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}
	SAFE_RELEASE(pHeightTexture);
	
	
	//エフェクト作成
	if (FAILED(D3DXCreateEffectFromFile(pDevice, "Ocean.fx", NULL, NULL, 0, NULL, &g_pEffect, NULL)))
	{
		MessageBox(NULL, "エフェクト作成失敗", "", MB_OK);
	}
}



TmpOcean::~TmpOcean()
{
	SAFE_RELEASE(g_pDecl);
	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(g_pBumpTex);
	SAFE_DELETE(pMeshMaterials);
	for (DWORD k = 0; k < dwNumMaterials; k++)
	{
		SAFE_RELEASE(pMeshTextures[k]);
	}
	SAFE_RELEASE(pMesh);
}

void TmpOcean::render(D3DXMATRIX mView, D3DXMATRIX mProj)
{
	static D3DXVECTOR2 vWave(0, 0);
	static float fDelta = 0.01;
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

	D3DXMATRIX mWVP = mWorld *mView* mProj;
	D3DXMATRIX mWorldInvTranspose;
	D3DXMatrixInverse(&mWorldInvTranspose, NULL, &mWorld);
	D3DXMatrixTranspose(&mWorldInvTranspose, &mWorldInvTranspose);

	D3DXMATRIX mViewInverse;
	D3DXMatrixInverse(&mViewInverse, NULL, &mView);
	g_pEffect->SetMatrix("mWorldViewProj", &mWVP);

	g_pEffect->SetMatrix("mWorld", &mWorld);

	g_pEffect->SetMatrixTranspose("mWorldInverseTranspose", &mWorldInvTranspose);
	g_pEffect->SetMatrix("mViewInverse", &mViewInverse);

	D3DXMATRIX m;
	D3DXVECTOR4 v;
	D3DXVECTOR4 LightPos = D3DXVECTOR4(0, -10.1, -0.1, 0);
	D3DXMatrixInverse(&m, NULL, &mWorld);
	D3DXVec4Transform(&v, &LightPos, &m);
	D3DXVec3Normalize((D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v);
	g_pEffect->SetVector("vLightDir", &v);
	m = mWorld * mView;
	D3DXMatrixInverse(&m, NULL, &m);
	v = D3DXVECTOR4(0, 0, 0, 1);
	D3DXVec4Transform(&v, &v, &m);
	g_pEffect->SetVector("vEyePos", &v);
	//波の動き
	g_pEffect->SetValue("f2Wave", &vWave, sizeof(vWave));
	vWave.x += 0.001f;
	g_pEffect->SetValue("g_fHeight", &g_fHeight, sizeof(FLOAT));
	g_fHeight += fDelta;
	if (g_fHeight <= 0.8)
	{
		fDelta = 0.002;
	}
	else if (g_fHeight > 1.2)
	{
		fDelta = -0.002;
	}
	g_pEffect->SetTechnique("tecOcean");
	UINT Passes = 0;
	g_pEffect->SetTexture("NormalMap", g_pBumpTex);
	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		if (SUCCEEDED(g_pEffect->Begin(&Passes, 0)))
		{
			for (INT iPass = 0; iPass < Passes; iPass++)
			{
				if (SUCCEEDED(g_pEffect->BeginPass(iPass)))
				{
					pMesh->DrawSubset(i);
					g_pEffect->EndPass();
				}
			}
			g_pEffect->End();
		}
	}
}
