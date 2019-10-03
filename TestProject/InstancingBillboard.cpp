//===================================================================================================================================
//【InstancingBillboard.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/27
// [更新日]2019/09/28
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "InstancingBillboard.h"
#include "Direct3D9.h"
#include "Input.h"
#include <time.h>

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
InstancingBillboard::InstancingBillboard()
{
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
	device = getDevice();
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
	instanceList->terminate();
	SAFE_DELETE(instanceList);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT InstancingBillboard::initialize(LPD3DXEFFECT _effect, LPDIRECT3DTEXTURE9 _texture)
{
	//インスタンシングビルボードの頂点定義
	InstancingBillboardNS::Vertex vertex[4] = {
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
	
	//テクスチャを設定
	texture = _texture;

	//インスタンスリストの初期化
	instanceList = new InstancingBillboardNS::InstanceList();

	return S_OK;
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void InstancingBillboard::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	int instanceNum = getInstanceNum();
	if (!onRender)return;
	if (instanceNum <= 0)return;
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
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | instanceNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// 頂点宣言を通知
	device->SetVertexDeclaration(declation);
	
	//デバイスデータストリームにメッシュの各バッファをバインド
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(InstancingBillboardNS::Vertex));		//頂点バッファ
	device->SetStreamSource(1, positionBuffer, 0, sizeof(D3DXVECTOR3));						//位置バッファ
	device->SetStreamSource(2, colorBuffer, 0, sizeof(D3DXCOLOR));							//カラーバッファ

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
//【カラーバッファをセットする】
//===================================================================================================================================
void InstancingBillboard::updateColor()
{
	int instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;
	if (device == NULL)				return;

	//カラー配列の更新
	for (int i = 0; i < instanceNum; i++)
	{
		color[i] = instanceList->getValue(i)->color;
	}

	//カラーバッファの解放
	if (this->colorBuffer != NULL)	SAFE_RELEASE(this->colorBuffer);

	//カラーバッファの作成
	device->CreateVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, 0, 0, D3DPOOL_MANAGED, &colorBuffer, 0);
	copyVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, color, colorBuffer);

}

//===================================================================================================================================
//【描画数の位置情報バッファをセットする】
//===================================================================================================================================
void InstancingBillboard::updatePosition()
{
	int instanceNum = getInstanceNum();
	if (device == NULL)	return;
	if (getInstanceNum() <= 0) return;
	
	for (int i = 0; i < instanceNum; i++)
	{
		position[i] = instanceList->getValue(i)->position;
	}

	//位置バッファの解放
	if (positionBuffer != NULL)SAFE_RELEASE(positionBuffer);

	//位置情報バッファの作成
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	copyVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, position, positionBuffer);

}

//===================================================================================================================================
//【インスタンスの生成】
//===================================================================================================================================
void InstancingBillboard::generateInstance(InstancingBillboardNS::Instance instance)
{
	int instanceNum = getInstanceNum();

	instanceList->insertFront();
	instanceList->listUpdate();

	//位置バッファ
	if(this->position != NULL)SAFE_DELETE_ARRAY(this->position);
	position = new D3DXVECTOR3[instanceNum];

	//カラーバッファの作成
	if (this->color != NULL)	SAFE_DELETE_ARRAY(this->color);
	color = new D3DXCOLOR[instanceNum];

	*instanceList->getValue(0) = instance;

}

//===================================================================================================================================
//【インスタンスの削除】
//===================================================================================================================================
void InstancingBillboard::deleteInstance(int i)
{
	if (instanceList->getValue(i)->existenceTimer >= 0)return;
	instanceList->remove(instanceList->getNode(i));
	instanceList->listUpdate();
}

//===================================================================================================================================
//【描画をしない】
//===================================================================================================================================
void InstancingBillboard::offRender()
{
	onRender = false;
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void InstancingBillboard::update(float frameTime)
{
	if (getInput()->isKeyDown('G'))
	{
		srand((unsigned int)time(NULL));
		InstancingBillboardNS::Instance instance;
		instance.position = D3DXVECTOR3((float)(rand()%100-50),(float)(rand()%100-50),(float)(rand()%100-50));
		instance.color = D3DCOLOR_RGBA(rand()%255,rand()%255,rand()%255,255);
		//instance.texture = *textureNS::reference(textureNS::LIGHT_001);
		instance.existenceTimer = (float)(rand()%120);
		generateInstance(instance);
	}

	for (int i = 0; i < instanceList->nodeNum; i++)
	{
		instanceList->getValue(i)->existenceTimer -= frameTime;
		deleteInstance(i);
	}

}

//===================================================================================================================================
//【getter】
//===================================================================================================================================
InstancingBillboardNS::InstanceList InstancingBillboard::getList() { return *instanceList; }
int InstancingBillboard::getInstanceNum() { return instanceList->nodeNum; }
