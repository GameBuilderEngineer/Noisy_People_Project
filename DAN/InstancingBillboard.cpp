//===================================================================================================================================
//【InstancingBillboard.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/27
// [更新日]2019/10/08
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
	onRender			= false;
	didGenerate		= false;
	didDelete			= false;
	vertexBuffer		= NULL;
	indexBuffer		= NULL;
	positionBuffer		= NULL;
	colorBuffer			= NULL;
	uvBuffer			= NULL;
	declation			= NULL;
	effect					= NULL;
	position				= NULL;
	color					= NULL;
	uv						= NULL;
	texture				= NULL;
	device				= getDevice();
	srand((unsigned int)time(NULL));
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
InstancingBillboard::~InstancingBillboard()
{
	//SAFE_RELEASE
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(positionBuffer);
	SAFE_RELEASE(colorBuffer);
	SAFE_RELEASE(uvBuffer);
	SAFE_RELEASE(declation);

	//SAFE_DELETE
	SAFE_DELETE_ARRAY(position);
	SAFE_DELETE_ARRAY(color);
	SAFE_DELETE_ARRAY(uv);

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
		{D3DXVECTOR2(-1.0f,  1.0f),	D3DXVECTOR2(0.0f,0.0f)},
		{D3DXVECTOR2(1.0f,  1.0f),	D3DXVECTOR2(1.0f,0.0f)},
		{D3DXVECTOR2(-1.0f, -1.0f),	D3DXVECTOR2(0.0f,1.0f)},
		{D3DXVECTOR2(1.0f, -1.0f),	D3DXVECTOR2(1.0f,1.0f)},
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
		{ 0, 0,									D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },	//頂点座標
		{ 0, sizeof(D3DXVECTOR2),	D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0 },	//UV
		{ 1, 0,									D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1 },	//位置
		{ 2, 0,									D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,			0 },	//カラー
		{ 3, 0,									D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2 },	//相対UV
		D3DDECL_END()
	};
	device->CreateVertexDeclaration(vertexElement, &declation);

	//シェーダーを設定
	effect = _effect;
	
	//テクスチャを設定
	texture = _texture;

	//インスタンスリストの初期化
	instanceList = new InstancingBillboardNS::InstanceList();

	//描画をオンにする
	onRender = true;

	return S_OK;
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void InstancingBillboard::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (!onRender)return;
	int instanceNum = getInstanceNum();
	if (instanceNum <= 0)return;

	//Z深度バッファ
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//αテスト
	//device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	//加算合成を行う
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

	// αブレンドを行う
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// αソースカラーの指定
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// αデスティネーションカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//インスタンス宣言
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA		| instanceNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA		| 1);

	// 頂点宣言を通知
	device->SetVertexDeclaration(declation);
	
	//デバイスデータストリームにメッシュの各バッファをバインド
	device->SetStreamSource(0, vertexBuffer,	0, sizeof(InstancingBillboardNS::Vertex));		//頂点バッファ
	device->SetStreamSource(1, positionBuffer,	0, sizeof(D3DXVECTOR3));								//位置バッファ
	device->SetStreamSource(2, colorBuffer,		0, sizeof(D3DXCOLOR));									//カラーバッファ
	device->SetStreamSource(3, uvBuffer,		0, sizeof(D3DXVECTOR2));								//UVバッファ

	//インデックスバッファをセット
	device->SetIndices(indexBuffer);

	effect->SetTechnique("mainTechnique");
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
//【位置バッファを更新する】
//===================================================================================================================================
void InstancingBillboard::updatePosition()
{
	int instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		position[i] = instanceList->getValue(i)->position;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, position, positionBuffer);
}

//===================================================================================================================================
//【UVバッファを更新する】
//===================================================================================================================================
void InstancingBillboard::updateUV()
{
	int instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		uv[i] = instanceList->getValue(i)->uv;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, uv, uvBuffer);
}

//===================================================================================================================================
//【カラーバッファを更新する】
//===================================================================================================================================
void InstancingBillboard::updateColor()
{
	int instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;

	for (int i = 0; i < instanceNum; i++)
	{
		color[i] = instanceList->getValue(i)->color;
	}
	copyVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, color, colorBuffer);
}


//===================================================================================================================================
//【インスタンスの生成】
//===================================================================================================================================
void InstancingBillboard::generateInstance(InstancingBillboardNS::Instance instance)
{
	instanceList->insertFront(instance);			//インスタンスを作成
	didGenerate = true;
}

//===================================================================================================================================
//【インスタンスの削除】
//===================================================================================================================================
void InstancingBillboard::deleteInstance(int i)
{
	if (instanceList->getValue(i)->existenceTimer >= 0)return;
	instanceList->remove(instanceList->getNode(i));		//リスト内のインスタンスを削除
	didDelete = true;
}

//===================================================================================================================================
//【ランダムアクセス用配列の更新】
//===================================================================================================================================
void InstancingBillboard::updateAccessList()
{
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
	didGenerate = false;
	didDelete = false;

	//削除処理
	for (int i = 0; i < instanceList->nodeNum; i++)
	{
		instanceList->getValue(i)->update(frameTime);
		deleteInstance(i);
	}

	//生成処理（サンプル）
	InstancingBillboardNS::Instance instance;
	instance.position			= D3DXVECTOR3((float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500));
	instance.rotation			= D3DXVECTOR3((float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500));
	instance.scale				= D3DXVECTOR3((float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500));
	int pattern = rand()%6;
	switch (pattern)
	{
	case 0:instance.speed		= D3DXVECTOR3(0,0,-30.0f);			break;
	case 1:instance.speed		= D3DXVECTOR3(0,0,30.0f);			break;
	case 2:instance.speed		= D3DXVECTOR3(0,-30.0f,0.0f);		break;
	case 3:instance.speed		= D3DXVECTOR3(0,30.0f,0.0f);		break;
	case 4:instance.speed		= D3DXVECTOR3(-30,0,0.0f);			break;
	case 5:instance.speed		= D3DXVECTOR3(30,0,-0.0f);			break;
	}
	instance.color					= D3DCOLOR_RGBA(rand()%255,rand()%255,rand()%255,255);
	instance.uv					= D3DXVECTOR2(0.0f, 0.0f);
	instance.existenceTimer	= (float)(rand() % 1000);
	generateInstance(instance);
	

	//削除もしくは作成を行った場合のメモリ整理
	if (didDelete || didGenerate)
	{
		updateAccessList();	//リストの更新
		updateBuffer();			//バッファの更新
		updateArray();			//コピー配列の更新
	}
	
	//値の更新
	updatePosition();
	updateColor();
	updateUV();
}

//===================================================================================================================================
//【バッファのリソース更新】
//===================================================================================================================================
void InstancingBillboard::updateBuffer() 
{
	//インスタンス数の取得
	int instanceNum = getInstanceNum();
	if (instanceNum <= 0)
	{
		SAFE_RELEASE(positionBuffer);		//位置バッファの解放
		SAFE_RELEASE(uvBuffer);				//UVバッファの解放
		SAFE_RELEASE(colorBuffer);			//カラーバッファの解放
	}

	//位置情報バッファの再作成
	SAFE_RELEASE(positionBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);

	//UV情報バッファの再作成
	SAFE_RELEASE(uvBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, 0, 0, D3DPOOL_MANAGED, &uvBuffer, 0);

	//カラーバッファの再作成
	SAFE_RELEASE(colorBuffer);
	device->CreateVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, 0, 0, D3DPOOL_MANAGED, &colorBuffer, 0);

}

//===================================================================================================================================
//【copy用配列のメモリ更新】
//===================================================================================================================================
void InstancingBillboard::updateArray() 
{
	//インスタンス数の取得
	int instanceNum = getInstanceNum();

	//位置バッファ用配列の再作成
	SAFE_DELETE_ARRAY(position);
	position = new D3DXVECTOR3[instanceNum];

	//UVバッファ用配列の再作成
	SAFE_DELETE_ARRAY(uv);
	uv = new D3DXVECTOR2[instanceNum];

	//カラーバッファ用配列の再作成
	SAFE_DELETE_ARRAY(color);
	color = new D3DXCOLOR[instanceNum];

}


//===================================================================================================================================
//【getter】
//===================================================================================================================================
InstancingBillboardNS::InstanceList InstancingBillboard::getList() { return *instanceList; }
int InstancingBillboard::getInstanceNum() { return instanceList->nodeNum; }
