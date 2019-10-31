//============================================================================================================================================
// Document
//============================================================================================================================================
// AllocateHierarchy.h
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/09/03
//============================================================================================================================================
#pragma once
#include "Base.h"
//============================================================================================================================================
// Class
// クラス
//============================================================================================================================================
// D3DXFrameDerived
class D3DXFrameDerived : public D3DXFRAME	// "D3DXFRAME"から派生した構造体：各フレームに保存されるアプリ固有の情報を追加できる
{
public:
	D3DXMATRIXA16 combinedTransformationMatrix;

#ifndef _DEBUG
	void *operator new(size_t index) { return _aligned_malloc(index, 16); }
#endif
};
// D3DXMeshContainerDerived
class D3DXMeshContainerDerived : public D3DXMESHCONTAINER	// "D3DXMESHCONTAINER"から派生した構造体：各メッシュと共に、保存されるアプリ固有の情報を追加できる
{
public:
	LPDIRECT3DTEXTURE9* texture;		//	テクスチャ
	LPD3DXMESH originalMesh;			//	オリジナルメッシュ
	LPD3DXBUFFER boneCombinationBuffer;	//	ボーン情報のバッファ
	D3DXMATRIX** boneMatrix;			//	全てのボーンのマトリックスポインタの配列
	D3DXMATRIX* boneOffsetMatrix;		//	ボーンのオフセットマトリクス
	DWORD boneMax;						//	ボーンの最大数
	DWORD boneWeightMax;				//	１つの頂点に影響を及ぼすウェイトの最大数
	DWORD paletteEntryMax;				//	パレットサイズ
};
// AllocateHierarchy
class AllocateHierarchy : public ID3DXAllocateHierarchy	// フレームとメッシュコンテナを作成するためのカスタムメソッドを持つ"ID3DXAllocateHierarchy"のカスタムバージョン：DirectX SDKサンプルに含まれるボーンの動的カテゴリ( カテゴリの中から使用される )
{
private:
	LPDIRECT3DDEVICE9 device;
	HRESULT allocateName(LPCSTR _name, LPSTR* _newName);
	HRESULT createSkinMesh(D3DXMeshContainerDerived* _meshContainer);
public:
	AllocateHierarchy(LPDIRECT3DDEVICE9 _device);
	~AllocateHierarchy(void);
	STDMETHOD(CreateFrame)(THIS_ LPCSTR _name, LPD3DXFRAME* _newFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR _name, CONST D3DXMESHDATA* _meshData, CONST D3DXMATERIAL* _material, CONST D3DXEFFECTINSTANCE* _effectInstance, DWORD _materialNumber, CONST DWORD* _adjacency, LPD3DXSKININFO _skinInformation, LPD3DXMESHCONTAINER* _newMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME _frame);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER _baseMeshContainer);
};