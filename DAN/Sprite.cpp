//=============================================================================
// スプライト処理 [Sprite.cpp]
//
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成日 : 2019/9/19
//=============================================================================
#include "Sprite.h"
#include "Direct3D9.h"
using namespace SpriteNS;

//=============================================================================
// コンストラクタ
//=============================================================================
Sprite::Sprite()
{
	onReadFile = false;
	texture = NULL;
}


//=============================================================================
// デストラクタ
//=============================================================================
Sprite::~Sprite()
{
	if (onReadFile)	SAFE_RELEASE(texture);
}


//=============================================================================
// 初期化処理（テクスチャをファイルから読み込む）
//=============================================================================
void Sprite::initialize(const char* fileName, int _pivot,
	int _width, int _height, D3DXVECTOR3 _position, D3DXVECTOR3 _rotation, D3DCOLOR color)
{
	if (fileName)
	{
		setTextureFromFile(fileName);
	}

	setUVCoord(D3DXVECTOR2(0.0, 0.0), D3DXVECTOR2(1.0, 0.0), D3DXVECTOR2(0.0, 1.0), D3DXVECTOR2(1.0, 1.0));
	setPivot(_pivot);
	setSize(_width, _height);
	initialRadius = radius;
	position = _position;
	rotation = _rotation;
	setVertex();
	setColorAllVertex(color);
	vertexWk[0].rhw = vertexWk[1].rhw = vertexWk[2].rhw = vertexWk[3].rhw = 1.0f;
}


//=============================================================================
// 初期化処理（テクスチャをポインタで渡す）
//=============================================================================
void Sprite::initialize(LPDIRECT3DTEXTURE9 _texture, int _pivot,
	int _width, int _height, D3DXVECTOR3 _position, D3DXVECTOR3 _rotation, D3DCOLOR color)
{
	if (_texture)
	{
		setTexturePointer(_texture);
	}

	setUVCoord(D3DXVECTOR2(0.0, 0.0), D3DXVECTOR2(1.0, 0.0), D3DXVECTOR2(0.0, 1.0), D3DXVECTOR2(1.0, 1.0));
	setPivot(_pivot);
	setSize(_width, _height);
	initialRadius = radius;
	position = _position;
	rotation = _rotation;
	setVertex();
	setColorAllVertex(color);
	vertexWk[0].rhw = vertexWk[1].rhw = vertexWk[2].rhw = vertexWk[3].rhw = 1.0f;
}


//=============================================================================
// 描画処理
//=============================================================================
void Sprite::render()
{
	LPDIRECT3DDEVICE9 device = getDevice();								//デバイスの取得

	// αテストを有効に
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// 透過処理の設定
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 透過処理の設定
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// 透過処理の設定

	device->SetFVF(FVF_VERTEX_2D);
	device->SetTexture(0, texture);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(SpriteNS::Vertex2D));

	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//=============================================================================
// テクスチャをファイルから読み込む
//=============================================================================
HRESULT Sprite::setTextureFromFile(const char* fileName)
{
	LPDIRECT3DDEVICE9 device = getDevice();	//デバイスの取得

	//setVisualDirectory();	// カレントディレクトリの変更

	if (FAILED(D3DXCreateTextureFromFile(device, fileName, &texture)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", fileName, MB_OK);
		return E_FAIL;
	}
	onReadFile = true;
	return S_OK;
}


//=============================================================================
// テクスチャをポインタでセットする
//=============================================================================
void Sprite::setTexturePointer(LPDIRECT3DTEXTURE9 _texture)
{
	texture = _texture;
}


//=============================================================================
// UV座標を設定
//=============================================================================
void Sprite::setUVCoord(D3DXVECTOR2 v0, D3DXVECTOR2 v1, D3DXVECTOR2 v2, D3DXVECTOR2 v3)
{
	vertexWk[0].uv = v0;
	vertexWk[1].uv = v1;
	vertexWk[2].uv = v2;
	vertexWk[3].uv = v3;
}


//=============================================================================
// サイズを設定
//=============================================================================
void Sprite::setSize(int _width, int _height)
{
	width = _width;
	height = _height;

	if (pivot == CENTER)
	{
		// 中心座標から頂点への角度を初期化
		centralAngle = atan2f((float)height / 2, (float)width / 2);	
		// 中心座標から頂点へのベクトルを求める
		D3DXVECTOR2 vec = D3DXVECTOR2((float) width / 2, (float)height / 2);
		// 半径の初期化
		radius = D3DXVec2Length(&vec);
	}
	else
	{
		// 三平方の定理
		radius = sqrtf((float)(width * width + height * height));
	}
}


//=============================================================================
// 頂点座標を設定
//=============================================================================
void Sprite::setVertex()
{
	switch (pivot)
	{
	case CENTER:
		vertexWk[0].vtx.x = position.x - cosf(centralAngle + rotation.z) * radius;
		vertexWk[0].vtx.y = position.y - sinf(centralAngle + rotation.z) * radius;
		vertexWk[1].vtx.x = position.x + cosf(centralAngle - rotation.z) * radius;
		vertexWk[1].vtx.y = position.y - sinf(centralAngle - rotation.z) * radius;
		vertexWk[2].vtx.x = position.x - cosf(centralAngle - rotation.z) * radius;
		vertexWk[2].vtx.y = position.y + sinf(centralAngle - rotation.z) * radius;
		vertexWk[3].vtx.x = position.x + cosf(centralAngle + rotation.z) * radius;
		vertexWk[3].vtx.y = position.y + sinf(centralAngle + rotation.z) * radius;
		break;

	case TOP_LEFT:
		vertexWk[0].vtx.x = position.x;
		vertexWk[0].vtx.y = position.y;
		vertexWk[1].vtx.x = position.x + cosf(rotation.z) * width;
		vertexWk[1].vtx.y = position.y + sinf(rotation.z) * width;
		vertexWk[2].vtx.x = position.x - sin(rotation.z) * height;
		vertexWk[2].vtx.y = position.y + cos(rotation.z) * height;
		vertexWk[3].vtx.x = position.x + cosf(rotation.z) * width - sin(rotation.z) * height;
		vertexWk[3].vtx.y = position.y + sinf(rotation.z) * width + cos(rotation.z) * height;
		break;
	case TOP_CENTER:
		break;
	case TOP_RIGHT://染矢作、回転することは考慮してないです
		vertexWk[0].vtx.x = position.x - width;
		vertexWk[0].vtx.y = position.y ;
		vertexWk[1].vtx.x = position.x;
		vertexWk[1].vtx.y = position.y;
		vertexWk[2].vtx.x = position.x - width;
		vertexWk[2].vtx.y = position.y + height;
		vertexWk[3].vtx.x = position.x ;
		vertexWk[3].vtx.y = position.y + height;
		break;

	}
	vertexWk[0].vtx.z = 0.0f;
	vertexWk[1].vtx.z = 0.0f;
	vertexWk[2].vtx.z = 0.0f;
	vertexWk[3].vtx.z = 0.0f;
}


//=============================================================================
// 頂点カラーを頂点別に設定
//=============================================================================
void Sprite::setColor(D3DCOLOR v0, D3DCOLOR v1, D3DCOLOR v2, D3DCOLOR v3)
{
	vertexWk[0].diffuse = v0;
	vertexWk[1].diffuse = v1;
	vertexWk[2].diffuse = v2;
	vertexWk[3].diffuse = v3;
}


//=============================================================================
// 頂点カラーを一括設定
//=============================================================================
void Sprite::setColorAllVertex(D3DCOLOR color)
{
	vertexWk[0].diffuse =
	vertexWk[1].diffuse =
	vertexWk[2].diffuse =
	vertexWk[3].diffuse = color;
}


//=============================================================================
// α値を頂点別に設定
//=============================================================================
void Sprite::setAlpha(int v0, int v1, int v2, int v3)
{
	vertexWk[0].diffuse &= 0x00ffffff;
	vertexWk[1].diffuse &= 0x00ffffff;
	vertexWk[2].diffuse &= 0x00ffffff;
	vertexWk[3].diffuse &= 0x00ffffff;

	vertexWk[0].diffuse |= ((v0) & 0xff) << 24;
	vertexWk[1].diffuse |= ((v1) & 0xff) << 24;
	vertexWk[2].diffuse |= ((v2) & 0xff) << 24;
	vertexWk[3].diffuse |= ((v3) & 0xff) << 24;
}


//=============================================================================
// α値を一括設定
//=============================================================================
void Sprite::setAlphaAllVertex(int _alpha)
{
	vertexWk[0].diffuse &= 0x00ffffff;
	vertexWk[1].diffuse &= 0x00ffffff;
	vertexWk[2].diffuse &= 0x00ffffff;
	vertexWk[3].diffuse &= 0x00ffffff;

	vertexWk[0].diffuse |= ((_alpha) & 0xff) << 24;
	vertexWk[1].diffuse |= ((_alpha) & 0xff) << 24;
	vertexWk[2].diffuse |= ((_alpha) & 0xff) << 24;
	vertexWk[3].diffuse |= ((_alpha) & 0xff) << 24;
}


//=============================================================================
// 座標を設定
//=============================================================================
void Sprite::setPosition(D3DXVECTOR3 _position)
{
	position = _position;

}


//=============================================================================
// 回転を設定
//=============================================================================
void Sprite::setRotation(D3DXVECTOR3 _rotation)
{ 
	rotation = _rotation;
}


//=============================================================================
// 回転軸を設定
//=============================================================================
void Sprite::setPivot(int _pivot)
{
	pivot = _pivot;
}


//=============================================================================
// 半径を設定（回転軸が中心のときのみ有効）
//=============================================================================
void Sprite::setRadius(float _radius)
{
	radius = _radius;
}


//=============================================================================
// Getter
//=============================================================================
SpriteNS::Vertex2D* Sprite::getVertexPointer() { return vertexWk; }
D3DXVECTOR3	 Sprite::getPosition() { return position; }
D3DXVECTOR3* Sprite::getPositionPointer() { return &position; }
D3DXVECTOR3	Sprite::getRotation() { return rotation; }
D3DXVECTOR3* Sprite::getRotationPointer() { return &rotation; }
float Sprite::getInitialRadius() { return initialRadius; }
LPDIRECT3DTEXTURE9 Sprite::getTexture() { return texture; }

