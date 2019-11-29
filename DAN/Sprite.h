//=============================================================================
// スプライト処理 [Sprite.h]
//
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成日 : 2019/6/8
//=============================================================================
#pragma once
#include <d3dx9.h>
#include "Base.h"

namespace SpriteNS
{
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

	const  int	NUM_VERTEX = 4;		// 頂点数
	const  int	NUM_POLYGON = 2;	// ポリゴン数

	// 原点（回転軸）
	enum PIVOT
	{
		CENTER,						// 中心
		TOP_LEFT,					// 左上頂点
		TOP_CENTER,					// テロップ表示用頂点
		TOP_RIGHT,					// 染矢作、回転することは考慮してないです
		BOTTOM_LEFT,				// 左下頂点（未対応）
		BOTTOM_RIGHT,				// 右下頂点（未対応）
		PIVOT_MAX
	};

	// ２Ｄポリゴン頂点フォーマットに合わせた構造体
	struct Vertex2D
	{
		D3DXVECTOR3		vtx;		// 頂点座標
		float			rhw;		// テクスチャのパースペクティブコレクト用
		D3DCOLOR		diffuse;	// 反射光
		D3DXVECTOR2		uv;			// テクスチャ座標
	};
}


//*****************************************************************************
// スプライト処理クラス
//*****************************************************************************
class Sprite 
{
private:
	//------
	// Data
	//------
	SpriteNS::Vertex2D	vertexWk[SpriteNS::NUM_VERTEX]; // 頂点情報格納ワーク
	LPDIRECT3DTEXTURE9	texture;						// テクスチャへのポインタ
	int					pivot;							// 原点（回転軸）
	int					width;							// 横幅
	int					height;							// 高さ

	D3DXVECTOR3			position;						// 位置
	D3DXVECTOR3			rotation;						// 回転
	float				centralAngle;					// 中心座標から４頂点への角度
	float				radius;							// 半径
	float				initialRadius;					// 元の半径

	bool				onReadFile;						//このクラス内でファイルからテクスチャを読み込んだか

public:
	//--------
	// Method
	//--------
	// Regular use
	Sprite();
	~Sprite();
	void initialize(const char* fileName,										// 初期化処理（テクスチャをファイルから読み込む）
		int _pivot, int _width, int _height, D3DXVECTOR3 position,
		D3DXVECTOR3 rotation, D3DCOLOR color);
	void initialize(LPDIRECT3DTEXTURE9 _texture,								// 初期化処理（テクスチャをポインタで渡す）
		int _pivot, int _width, int _height, D3DXVECTOR3 _position,
		D3DXVECTOR3 _rotation, D3DCOLOR color);
	void render();										// 描画

	// Function to set parameters
	HRESULT setTextureFromFile(const char* fileName);							// テクスチャをファイルから読み込む
	void setTexturePointer(LPDIRECT3DTEXTURE9 _texture);						// テクスチャをポインタでセットする
	void setUVCoord(D3DXVECTOR2 v0, D3DXVECTOR2 v1,
		D3DXVECTOR2 v2, D3DXVECTOR2 v3);										// UV座標を設定
	void setSize(int sizeX, int sizeY);											// サイズを設定
	void setVertex();															// 頂点座標を設定
	void setColor(D3DCOLOR v0, D3DCOLOR v1, D3DCOLOR v2, D3DCOLOR v3);			// 頂点カラーを頂点別に設定
	void setColorAllVertex(D3DCOLOR color);										// 頂点カラーを一括設定
	void setAlpha(int v0, int v1, int v2, int v3);								// α値を頂点別に設定
	void setAlphaAllVertex(int _alpha);											// α値を一括設定
	void setPosition(D3DXVECTOR3 _position);									// 座標を設定
	void setRotation(D3DXVECTOR3 _rotation);									// 回転を設定
	void setPivot(int _pivot);													// 回転軸を設定
	void setRadius(float _radius);												// 半径を設定（回転中心軸なら頂点座標に反映）

	// Getter
	SpriteNS::Vertex2D*	getVertexPointer();			// 頂点情報のポインタを取得
	D3DXVECTOR3				getPosition();				// 座標取得
	D3DXVECTOR3*			getPositionPointer();		// 座標のポインタを取得
	D3DXVECTOR3				getRotation();				// 回転を取得
	D3DXVECTOR3*			getRotationPointer();		// 回転のポインタを取得
	float					getInitialRadius();			// 半径を初期値を取得
	LPDIRECT3DTEXTURE9		getTexture();				// テクスチャを取得
};
