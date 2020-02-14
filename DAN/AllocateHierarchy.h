//=============================================================================
//
// アニメ処理 [AllocateHierarchy.h]
// Author : HAL東京 周子皓
//
//=============================================================================
#pragma once

#include <d3dx9.h>
#include <stdio.h>
#include "Base.h"


struct D3DXFRAME_DERIVED: public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};


struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;            // テクスチャ情報

	// SkinMesh info
	LPD3DXMESH           pOrigMesh;             // オリジナルメッシュ

	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD                BoneNum;					 // ボーンの数
	DWORD                BoneWeightNum;               // １つの頂点に影響を及ぼす重みの数
	LPD3DXBUFFER         pBoneCombinationBuf;	 // ボーン情報のバッファ
	D3DXMATRIX**         ppBoneMatrixPtrs;      // 全ボーンのマトリックスのポインタの配列
	D3DXMATRIX*          pBoneOffsetMatrices;   // ボーンのオフセット行列
	DWORD                NumPaletteEntries;     // パレットサイズ
	bool                 UseSoftwareVP;        

	D3DXMATRIXA16*			g_pBoneMatrices;

};


class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)( THIS_ LPCSTR              Name, 
		CONST D3DXMESHDATA*       pMeshData,
		CONST D3DXMATERIAL*       pMaterials, 
		CONST D3DXEFFECTINSTANCE* pEffectInstances, 
		DWORD                     NumMaterials, 
		CONST DWORD *             pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);    
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};




void DrawFrame( IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame );
void DrawMeshContainer( IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );
//HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot );
void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
