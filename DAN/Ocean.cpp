//===================================================================================================================================
//【Ocean.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/07
// [更新日]2020/01/15
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Ocean.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Ocean::Ocean() {
	bumpTexture = NULL;
	waveMove	= D3DXVECTOR2(0,0);
	height		= 1.0f;
	deltaHeight = 0.01f;
	initialize();
};

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Ocean::~Ocean() {
	SAFE_DELETE(object);
	SAFE_RELEASE(bumpTexture);
};

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Ocean::initialize() {
	object = new Object();
	object->initialize(&D3DXVECTOR3(0, 0, 0));
	object->scale = D3DXVECTOR3(1, 1, 1);

	device = getDevice();
	effect = *shaderNS::reference(shaderNS::OCEAN);
	staticMesh = staticMeshNS::reference(staticMeshNS::OCEAN);
	
	//頂点宣言
	D3DVERTEXELEMENT9 vertexElement[65];
	int i = 0;
	while (staticMesh->vertexElement[i].Type != D3DDECLTYPE_UNUSED)
	{
		vertexElement[i] = staticMesh->vertexElement[i];
		i++;
	}
	vertexElement[i] = { 1, 0,					D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 };
	i++;
	vertexElement[i] = { 1, sizeof(float) * 4,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 2 };
	i++;
	vertexElement[i] = { 1, sizeof(float) * 4 * 2,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 3 };
	i++;
	vertexElement[i] = { 1, sizeof(float) * 4 * 3,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 4 };
	i++;
	vertexElement[i] = D3DDECL_END();
	i++;
	device->CreateVertexDeclaration(vertexElement, &declaration);
	needUpdate = true;

	//事前更新
	object->update();

	//ワールドマトリックスバッファ用配列の再作成
	SAFE_DELETE_ARRAY(worldMatrix);
	worldMatrix = new D3DXMATRIX[1];

	//ワールドマトリックス情報バッファの再作成
	SAFE_RELEASE(matrixBuffer);
	device->CreateVertexBuffer(sizeof(D3DXMATRIX)*1, 0, 0, D3DPOOL_MANAGED, &matrixBuffer, 0);

	worldMatrix[0] = object->matrixWorld;
	copyVertexBuffer(sizeof(D3DXMATRIX)*1, worldMatrix, matrixBuffer);

	//バンプマッピング用のテクスチャイメージの作成
	LPDIRECT3DTEXTURE9 heightTexture = *textureNS::reference(textureNS::OCEAN_BUMP);//ハイトマップテクスチャ
	D3DSURFACE_DESC desc;

	heightTexture->GetLevelDesc(0, &desc);
	if (FAILED(
		D3DXCreateTexture(
			device, 
			desc.Width, 
			desc.Height, 
			0,
			0, 
			D3DFMT_X8R8G8B8,
			D3DPOOL_MANAGED, 
			&bumpTexture)))
	{
		MSG("バンプマッピング用のテクスチャイメージの作成に失敗しました。");
	}
	if (FAILED(
		D3DXComputeNormalMap(
			bumpTexture, 
			heightTexture, 
			NULL, 
			0,
			D3DX_CHANNEL_LUMINANCE,
			//D3DX_CHANNEL_RED,
			20.0f)))
			//1.0f)))
	{
		MSG("バンプマッピング用テクスチャの法線の算出に失敗しました。");
	}

};

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Ocean::update()
{

	object->update();

	//波の移動
	waveMove.x += 0.0001f;

	//波の高さ増減
	height += deltaHeight;
	
	//波の高さ増減の切替
	if (height <= 0.8)
	{
		deltaHeight = 0.002;
	}
	else if (height > 1.2)
	{
		deltaHeight = -0.002;
	}

	//ワールド行列の逆転置行列
	//D3DXMatrixInverse(&worldInverseTranspose, NULL, &object->matrixWorld);	//逆行列
	//D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);	//転置行列

}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Ocean::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	//レンダーステートの設定
	device->SetRenderState(D3DRS_FILLMODE, staticMeshRendererNS::SOLID);

	//頂点宣言を追加
	device->SetVertexDeclaration(declaration);

	//ストリームにメッシュの頂点バッファをバインド
	device->SetStreamSource(0, staticMesh->vertexBuffer,	0, staticMesh->numBytesPerVertex);
	device->SetStreamSource(1, matrixBuffer,				0, sizeof(D3DXMATRIX));

	//インデックスバッファをセット
	device->SetIndices(staticMesh->indexBuffer);

	//シェーダへ値をセット
	effect->SetTechnique("tecOcean");
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);
	//effect->SetMatrixTranspose("WIT", &worldInverseTranspose);	//ワールド逆転置行列

	//ライトの方向ベクトルのセット
	D3DXMATRIX m;
	D3DXVECTOR4 v;
	D3DXVECTOR4 lightPosition = D3DXVECTOR4(-10, 100.1, -0.01, 0);
	D3DXMatrixInverse(&m, NULL, &object->matrixWorld);
	D3DXVec4Transform(&v, &lightPosition, &m);
	D3DXVec3Normalize((D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v);
	effect->SetVector("lightDirection", &v);

	//ビューの逆行列
	//D3DXMATRIX inverseView;
	//D3DXMatrixInverse(&inverseView, NULL, &view);
	//effect->SetMatrix("inverseView", &inverseView);

	//カメラの位置
	//考察：引数cameraPositionでいけそう　↓はビュー行列の逆行列で求めている・オブジェクトのワールド空間も使用しているのでダメかも
	D3DXMATRIX worldView = object->matrixWorld * view;
	D3DXMatrixInverse(&worldView, NULL, &worldView);//逆行列
	D3DXVECTOR4 eyePosition = D3DXVECTOR4(0, 0, 0, 1);
	D3DXVec4Transform(&eyePosition, &eyePosition, &worldView);
	effect->SetVector("eyePosition", &eyePosition);

	//波の値
	effect->SetValue("waveMove", &waveMove, sizeof(waveMove));	//移動値
	effect->SetValue("height", &height, sizeof(FLOAT));	//高さ
	
	// レンダリング
	effect->Begin(0, 0);
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		//effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		effect->SetTexture("textureDecal", *textureNS::reference(textureNS::OCEAN));
		effect->SetTexture("normalMap", bumpTexture);

		//シェーダー更新
		effect->CommitChanges();

		effect->BeginPass(0);
		//Drawコール
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,									//D3DPRIMITIVETYPE Type				:描画タイプ
			0,													//INT BaseVertexIndex				:オフセット
			staticMesh->attributeTable[i].VertexStart,			//UINT MinIndex						:その属性グループの頂点で最小のインデックス
			staticMesh->attributeTable[i].VertexCount,			//UINT NumVertices					:頂点数（その属性グループの頂点数）
			staticMesh->attributeTable[i].FaceStart * 3,		//UINT StartIndex					:インデックスのインデックス（インデックスバッファ内のインデックス）
			staticMesh->attributeTable[i].FaceCount);			//UINT PrimitiveCount				:ポリゴン数

		effect->EndPass();
	}
	effect->End();

	device->SetStreamSource(0, NULL, 0, NULL);
	device->SetStreamSource(1, NULL, 0, NULL);

	//後始末
	device->SetStreamSourceFreq(0, 0);
	device->SetStreamSourceFreq(1, 0);
}
