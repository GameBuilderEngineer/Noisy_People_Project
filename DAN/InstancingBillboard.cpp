//===================================================================================================================================
//【InstancingBillboard.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/27
// [更新日]2019/10/23
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
	didGenerate			= false;
	didDelete			= false;
	vertexBuffer		= NULL;
	indexBuffer			= NULL;
	positionBuffer		= NULL;
	colorBuffer			= NULL;
	uvBuffer			= NULL;
	declation			= NULL;
	effect				= NULL;
	position			= NULL;
	color				= NULL;
	uv					= NULL;
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
	SAFE_RELEASE(rotationBuffer);
	SAFE_RELEASE(scaleBuffer);
	SAFE_RELEASE(colorBuffer);
	SAFE_RELEASE(uvBuffer);
	SAFE_RELEASE(declation);

	//SAFE_DELETE
	SAFE_DELETE_ARRAY(position);
	SAFE_DELETE_ARRAY(rotation);
	SAFE_DELETE_ARRAY(scale);
	SAFE_DELETE_ARRAY(color);
	SAFE_DELETE_ARRAY(uv);

	for (int i = 0; i < instanceNum; i++)
	{
		SAFE_DELETE(*instanceList->getValue(i));			//リスト内の実体を削除	
	}
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
		{ 0, sizeof(D3DXVECTOR2),				D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0 },	//UV
		{ 1, 0,									D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1 },	//位置
		{ 2, 0,									D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,			0 },	//カラー
		{ 3, 0,									D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2 },	//相対UV
		{ 4, 0,									D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3 },	//回転
		{ 5, 0,									D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4 },	//スケール
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
//【更新】
//===================================================================================================================================
void InstancingBillboard::update(float frameTime)
{

	for (int i = 0; i < instanceNum; i++)
	{
		(*instanceList->getValue(i))->update(frameTime);		//更新処理
		deleteInstance(i);										//削除処理
	}

	//削除もしくは作成を行った場合のメモリ整理
	if (didDelete || didGenerate)
	{
		updateAccessList();		//リストの更新
		updateBuffer();			//バッファの更新
		updateArray();			//コピー配列の更新
		didGenerate = false;
		didDelete = false;
	}
	
	//値の更新
	updatePosition();
	updateRotation();
	updateScale();
	updateColor();
	updateUV();

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
	device->SetRenderState(D3DRS_ZENABLE, TRUE);

	//αテスト
	//device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//device->SetRenderState(D3DRS_ALPHAREF, 0x00);

	// αブレンドを行う
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// αソースカラーの指定
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//加算合成を行う
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	// αデスティネーションカラーの指定
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//インスタンス宣言
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA		| instanceNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(4, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(5, D3DSTREAMSOURCE_INSTANCEDATA		| 1);

	// 頂点宣言を通知
	device->SetVertexDeclaration(declation);
	
	//デバイスデータストリームにメッシュの各バッファをバインド
	device->SetStreamSource(0, vertexBuffer,	0, sizeof(InstancingBillboardNS::Vertex));		//頂点バッファ
	device->SetStreamSource(1, positionBuffer,	0, sizeof(D3DXVECTOR3));								//位置バッファ
	device->SetStreamSource(2, colorBuffer,		0, sizeof(D3DXCOLOR));									//カラーバッファ
	device->SetStreamSource(3, uvBuffer,		0, sizeof(D3DXVECTOR2));								//UVバッファ
	device->SetStreamSource(4, rotationBuffer,	0, sizeof(D3DXVECTOR3));								//回転バッファ
	device->SetStreamSource(5, scaleBuffer,		0, sizeof(D3DXVECTOR2));								//スケールバッファ

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

	device->SetStreamSource(0, NULL, 0, NULL);
	device->SetStreamSource(1, NULL, 0, NULL);
	device->SetStreamSource(2, NULL, 0, NULL);
	device->SetStreamSource(3, NULL, 0, NULL);
	device->SetStreamSource(4, NULL, 0, NULL);
	device->SetStreamSource(5, NULL, 0, NULL);

	//後始末
	device->SetStreamSourceFreq(0, 0);
	device->SetStreamSourceFreq(1, 0);
	device->SetStreamSourceFreq(2, 0);
	device->SetStreamSourceFreq(3, 0);
	device->SetStreamSourceFreq(4, 0);
	device->SetStreamSourceFreq(5, 0);
	// αブレンドを切る
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//===================================================================================================================================
//【位置バッファを更新する】
//===================================================================================================================================
void InstancingBillboard::updatePosition()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		position[i] = (*instanceList->getValue(i))->position;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, position, positionBuffer);
}

//===================================================================================================================================
//【回転バッファを更新する】
//===================================================================================================================================
void InstancingBillboard::updateRotation()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		rotation[i] = (*instanceList->getValue(i))->rotation;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, rotation, rotationBuffer);
}

//===================================================================================================================================
//【スケールバッファを更新する】
//===================================================================================================================================
void InstancingBillboard::updateScale()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		scale[i] = (*instanceList->getValue(i))->scale;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, scale, scaleBuffer);
}

//===================================================================================================================================
//【UVバッファを更新する】
//===================================================================================================================================
void InstancingBillboard::updateUV()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		uv[i] = (*instanceList->getValue(i))->uv;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, uv, uvBuffer);
}

//===================================================================================================================================
//【カラーバッファを更新する】
//===================================================================================================================================
void InstancingBillboard::updateColor()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;

	for (int i = 0; i < instanceNum; i++)
	{
		color[i] = (*instanceList->getValue(i))->color;
	}
	copyVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, color, colorBuffer);
}

//===================================================================================================================================
//【インスタンスの生成】
//===================================================================================================================================
void InstancingBillboard::generateInstance(InstancingBillboardNS::Instance* instance)
{
	instanceList->insertFront(instance);			//インスタンスを作成
	didGenerate = true;
}

//===================================================================================================================================
//【インスタンスの削除】
//===================================================================================================================================
void InstancingBillboard::deleteInstance(int i)
{
	if ((*instanceList->getValue(i))->lifeTimer <= (*instanceList->getValue(i))->limitTime)return;
	SAFE_DELETE(*instanceList->getValue(i));			//リスト内の実体を削除
	instanceList->remove(instanceList->getNode(i));		//リスト内のインスタンスを削除
	didDelete = true;
}

//===================================================================================================================================
//【ランダムアクセス用配列の更新】
//===================================================================================================================================
void InstancingBillboard::updateAccessList()
{
	instanceList->listUpdate();
	instanceNum = instanceList->nodeNum;
}

//===================================================================================================================================
//【描画をしない】
//===================================================================================================================================
void InstancingBillboard::offRender()
{
	onRender = false;
}

//===================================================================================================================================
//【バッファのリソース更新】
//===================================================================================================================================
void InstancingBillboard::updateBuffer() 
{
	//インスタンス数の取得
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)
	{
		SAFE_RELEASE(positionBuffer);		//位置バッファの解放
		SAFE_RELEASE(rotationBuffer);		//回転バッファの解放
		SAFE_RELEASE(scaleBuffer);			//スケールバッファの解放
		SAFE_RELEASE(uvBuffer);				//UVバッファの解放
		SAFE_RELEASE(colorBuffer);			//カラーバッファの解放
		return;
	}

	//位置情報バッファの再作成
	SAFE_RELEASE(positionBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);

	//回転情報バッファの再作成
	SAFE_RELEASE(rotationBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, 0, 0, D3DPOOL_MANAGED, &rotationBuffer, 0);

	//スケール情報バッファの再作成
	SAFE_RELEASE(scaleBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, 0, 0, D3DPOOL_MANAGED, &scaleBuffer, 0);

	//UV情報バッファの再作成
	SAFE_RELEASE(uvBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, 0, 0, D3DPOOL_MANAGED, &uvBuffer, 0);

	//カラー情報バッファの再作成
	SAFE_RELEASE(colorBuffer);
	device->CreateVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, 0, 0, D3DPOOL_MANAGED, &colorBuffer, 0);

}

//===================================================================================================================================
//【copy用配列のメモリ更新】
//===================================================================================================================================
void InstancingBillboard::updateArray() 
{
	//インスタンス数の取得
	instanceNum = getInstanceNum();

	//位置バッファ用配列の再作成
	SAFE_DELETE_ARRAY(position);
	position = new D3DXVECTOR3[instanceNum];

	//回転バッファ用配列の再作成
	SAFE_DELETE_ARRAY(rotation);
	rotation = new D3DXVECTOR3[instanceNum];

	//スケールバッファ用配列の再作成
	SAFE_DELETE_ARRAY(scale);
	scale = new D3DXVECTOR2[instanceNum];

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

