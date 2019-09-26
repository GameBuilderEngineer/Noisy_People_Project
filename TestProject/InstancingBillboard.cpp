//===================================================================================================================================
//【InstancingBillboard.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/08/07
// [更新日]2019/09/12
//===================================================================================================================================
#include "InstancingBillboard.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
InstancingBillboard::InstancingBillboard()
{
	renderNum = 0;
	onRender = false;

	vertexBuffer = NULL;
	indexBuffer = NULL;
	positionBuffer = NULL;
	colorBuffer = NULL;
	declation = NULL;
	effect = NULL;
	position = NULL;
	color = NULL;
	texture = NULL;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
InstancingBillboard::~InstancingBillboard()
{
	//SAFE_RELEASE
	if (vertexBuffer != NULL)
		SAFE_RELEASE(vertexBuffer);
	if (indexBuffer != NULL)
		SAFE_RELEASE(indexBuffer);
	if (positionBuffer != NULL)
		SAFE_RELEASE(positionBuffer);
	if (colorBuffer != NULL)
		SAFE_RELEASE(colorBuffer);
	if (declation != NULL)
		SAFE_RELEASE(declation);

	//SAFE_DELETE
	if (position != NULL)			
		SAFE_DELETE_ARRAY(position);
	if (color != NULL)
		SAFE_DELETE_ARRAY(color);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT InstancingBillboard::initialize(LPDIRECT3DDEVICE9 device, LPD3DXEFFECT _effect, LPDIRECT3DTEXTURE9 _texture)
{
	InstancingBillboardVertex vertex[4] = {
		{D3DXVECTOR2( -1.0f,  1.0f),D3DXVECTOR2(0.0f,0.0f)},
		{D3DXVECTOR2(  1.0f,  1.0f),D3DXVECTOR2(1.0f,0.0f)},
		{D3DXVECTOR2( -1.0f, -1.0f),D3DXVECTOR2(0.0f,1.0f)},
		{D3DXVECTOR2(  1.0f, -1.0f),D3DXVECTOR2(1.0f,1.0f)},
	};
	//頂点バッファの作成
	device->CreateVertexBuffer(sizeof(vertex), 0, 0, D3DPOOL_MANAGED, &vertexBuffer, 0);//サイズ確保
	copyVertexBuffer(sizeof(vertex), &vertex, vertexBuffer);

	WORD index[6] = { 
		0,1,2,
		2,1,3 
	};

	//インデックスバッファの作成
	device->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuffer, 0);
	void *p = 0;
	indexBuffer->Lock(0, 0, &p, 0);
	memcpy(p, index, sizeof(index));
	indexBuffer->Unlock();

	//位置情報バッファの作成
	//device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	//copyVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, position, positionBuffer);

	//頂点宣言
	D3DVERTEXELEMENT9 vertexElement[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	//頂点座標
		{ 0, sizeof(D3DXVECTOR2), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },//UV
		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 },	//位置
		{ 2, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR, 0 },	//カラー
		D3DDECL_END()
	};
	device->CreateVertexDeclaration(vertexElement, &declation);

	//シェーダーを設定
	effect = _effect;

	//setShaderDirectory();
	//HRESULT hr;
	//LPD3DXBUFFER err = NULL;
	//if (FAILED(hr = D3DXCreateEffectFromFile(device, "InstancingBillboard.fx", NULL, NULL, 0, NULL, &effect, &err)))
	//{
	//	MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "ERROR", MB_OK);
	//}
	
	//テクスチャを設定
	texture = _texture;
	//setVisualDirectory();
	//D3DXCreateTextureFromFileA(device, "ring.png", &texture);

	return S_OK;
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void InstancingBillboard::render(LPDIRECT3DDEVICE9 device, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (!onRender)return;
	if (renderNum <= 0)return;
	//回転を打ち消す。
	//D3DXMATRIX cancelRotation = view;
	//cancelRotation._41 = cancelRotation._42 = cancelRotation._43 = 0;
	//D3DXMatrixInverse(&cancelRotation, NULL,&cancelRotation);



	//αテスト
	//device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// αブレンドを行う
	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// αソースカラーの指定
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// αデスティネーションカラーの指定
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//加算合成を行う
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

	// αブレンドを行う
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// αソースカラーの指定
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// αデスティネーションカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	//インスタンス宣言
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | renderNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// 頂点宣言を通知
	device->SetVertexDeclaration(declation);
	
	//デバイスデータストリームにメッシュの各バッファをバインド
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(InstancingBillboardVertex));	//頂点バッファ
	device->SetStreamSource(1, positionBuffer, 0, sizeof(D3DXVECTOR3));				//位置バッファ
	device->SetStreamSource(2, colorBuffer, 0, sizeof(D3DXCOLOR));					//カラーバッファ

	//インデックスバッファをセット
	device->SetIndices(indexBuffer);

	effect->SetTechnique("mainTechnique");
	//effect->SetMatrix("cancelRotation", &cancelRotation);
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);
	effect->SetTexture("planeTexture", texture);
	effect->Begin(0, 0);
	effect->BeginPass(0);
		
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	effect->EndPass();
	effect->End();

	//後始末
	device->SetStreamSourceFreq(0, 1);
	device->SetStreamSourceFreq(1, 1);
	device->SetStreamSourceFreq(2, 1);
	// αブレンドを切る
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//===================================================================================================================================
//【球状に位置情報をランダムに作成する】
//===================================================================================================================================
void InstancingBillboard::createPositionSpherical(LPDIRECT3DDEVICE9 device, int num ,float radius)
{
	renderNum = num;
	//位置情報配列の作成
	position = new D3DXVECTOR3[num];
	for (int i = 0; i < num; i++)
	{
		D3DXVECTOR3 pos((float)(rand() % 100 - 50),(float)(rand() % 100 - 50),(float)(rand() % 100 - 50));
		D3DXVec3Normalize(&pos, &pos);
		pos *= radius;
		position[i] = pos;
	}
	//位置情報バッファの作成
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	copyVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, position, positionBuffer);

	//カラー情報配列の作成
	D3DXCOLOR* col = new D3DXCOLOR[num];
	for (int i = 0; i < num; i++)
	{
		col[i] = D3DXCOLOR( 
			((float)(rand() % 100)) / 100, 
			((float)(rand() % 100)) / 100, 
			((float)(rand() % 100)) / 100, 
			1.0f);
	}
	//カラー
	setColorBuffer(device, renderNum, col);

	SAFE_DELETE_ARRAY(col);

	onRender = true;
}

//===================================================================================================================================
//【カラーバッファをセットする】
//・再セットも可能
//===================================================================================================================================
void InstancingBillboard::setColorBuffer(LPDIRECT3DDEVICE9 device, int num, D3DXCOLOR* colorList)
{
	if (num <= 0) {
		return;
	}
	if(colorBuffer!=NULL)
		SAFE_RELEASE(colorBuffer);
	if(color!=NULL)
		SAFE_DELETE_ARRAY(color);
	
	renderNum = num;

	//カラーバッファの作成
	color = new D3DXCOLOR[num];
	for (int i = 0; i < num; i++)
	{
		color[i] = colorList[i];
	}

	//カラーバッファの作成
	device->CreateVertexBuffer(sizeof(D3DXCOLOR)*renderNum, 0, 0, D3DPOOL_MANAGED, &colorBuffer, 0);
	copyVertexBuffer(sizeof(D3DXCOLOR)*renderNum, color, colorBuffer);
}

//===================================================================================================================================
//【描画数の位置情報バッファをセットする】
//===================================================================================================================================
void InstancingBillboard::setNumOfRender(LPDIRECT3DDEVICE9 device, int num, D3DXVECTOR3* positionList)
{
	if (num <= 0) {
		if(positionBuffer != NULL)SAFE_RELEASE(positionBuffer);
		onRender = false;
		return;
	}
	renderNum = num;

	position = new D3DXVECTOR3[num];
	for (int i = 0; i < num; i++)
	{
		position[i] = positionList[i];
	}

	if (positionBuffer != NULL)SAFE_RELEASE(positionBuffer);
	//位置情報バッファの作成
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	copyVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, position, positionBuffer);
	SAFE_DELETE_ARRAY(position);

	if (onRender == false)
	{
		//カラー情報配列の作成
		D3DXCOLOR* col = new D3DXCOLOR[num];
		for (int i = 0; i < num; i++)
		{
			col[i] = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		}
		//カラー
		setColorBuffer(device, renderNum, col);
		SAFE_DELETE_ARRAY(col);
	}

	onRender = true;
}

void InstancingBillboard::offRender()
{
	onRender = false;
}