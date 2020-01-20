#pragma once
#include "Base.h"
class TmpOcean :public Base
{
public:
	//Objectパラメータ
	LPD3DXMESH						pMesh;
	D3DMATERIAL9*					pMeshMaterials;
	LPDIRECT3DTEXTURE9*				pMeshTextures;
	DWORD							dwNumMaterials;
	D3DXVECTOR3						vPosition;
	D3DXMATRIX						mScaling;
	D3DXMATRIX						mRotation;
	D3DXMATRIX						mWorld;
	FLOAT							fYaw;
	FLOAT							fScale;

	//リソース
	LPDIRECT3DDEVICE9				pDevice		= NULL;
	LPD3DXEFFECT					g_pEffect	= NULL;
	LPDIRECT3DTEXTURE9				g_pBumpTex	= NULL;
	LPDIRECT3DVERTEXDECLARATION9	g_pDecl		= NULL;

	//パラメータ
	FLOAT							g_fHeight	= 1;


public:
	TmpOcean();
	~TmpOcean();

	void render(D3DXMATRIX mView, D3DXMATRIX mProj);
};

