//===================================================================================================================================
//【StaticMeshObject.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/08
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "StaticMeshObject.h"
#include "Direct3D9.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
StaticMeshObject::StaticMeshObject(StaticMesh* _staticMesh)
{
	this->staticMesh = _staticMesh;

	//デバイスの取得
	LPDIRECT3DDEVICE9 device = getDevice();

	//頂点宣言
	D3DVERTEXELEMENT9 vertexElement[65];
	int i = 0;
	while(staticMesh->vertexElement[i].Type != D3DDECLTYPE_UNUSED)
	{
		vertexElement[i] = staticMesh->vertexElement[i];
		i++;
	}
	vertexElement[i] = { 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 };
	i++;
	vertexElement[i] = D3DDECL_END();
	i++;
	device->CreateVertexDeclaration(vertexElement, &declaration);

	//位置バッファの作成
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3) * 1/*instanceNum*/, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	copyVertexBuffer(sizeof(D3DXVECTOR3)*1/*instanceNum*/, position, positionBuffer);

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
StaticMeshObject::~StaticMeshObject()
{

}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void StaticMeshObject::update()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 device = getDevice();

	Object::update();

	updatePosition();


}

//===================================================================================================================================
//【バッファのリソース更新】
//===================================================================================================================================
void StaticMeshObject::updateBuffer()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 device = getDevice();

	//位置情報バッファの再作成
	SAFE_RELEASE(positionBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*1/*instanceNum*/, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
}

//===================================================================================================================================
//【位置バッファを更新する】
//===================================================================================================================================
void StaticMeshObject::updatePosition()
{
	copyVertexBuffer(sizeof(D3DXVECTOR3)*1, position, positionBuffer);
}


//===================================================================================================================================
//【描画】
//===================================================================================================================================
void StaticMeshObject::render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	//描画が有効でない場合終了
	if (onRender == false)return;

	//デバイスの取得
	LPDIRECT3DDEVICE9 device = getDevice();

	//ワールドマトリックスの設定
	device->SetTransform(D3DTS_WORLD, &matrixWorld);
	
	//レンダーステートの設定
	device->SetRenderState(D3DRS_LIGHTING, onLighting);
	device->SetRenderState(D3DRS_FILLMODE, fillMode);

	//透過処理
	if (onTransparent)
	{//有効
		//αブレンディングを設定する
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);	// ポリゴンのαと
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);	// テクスチャのαを
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// 混ぜる
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);				// 加算合成を行う
	}
	else
	{//無効
		//αブレンディングを設定しない
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	//インスタンス宣言
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | renderNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//頂点宣言を追加
	device->SetVertexDeclaration(declaration);

	//ストリームにメッシュの頂点バッファをバインド
	device->SetStreamSource(0, staticMesh->vertexBuffer, 0, staticMesh->numBytesPerVertex);
	device->SetStreamSource(1, positionBuffer, 0, sizeof(D3DXVECTOR3));

	//インデックスバッファをセット
	device->SetIndices(staticMesh->indexBuffer);

	effect->SetTechnique("mainTechnique");
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);

	// レンダリング			
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		if (onTransparent) {
			D3DMATERIAL9 material = staticMesh->materials[i];
			material.Diffuse.a = alpha;
			effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		}
		else {
			effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		}

		effect->SetTexture("textureDecal", staticMesh->textures[i]);

		effect->Begin(0, 0);
		effect->BeginPass(0);

		//Drawコール
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,									//D3DPRIMITIVETYPE Type
			0,													//INT BaseVertexIndex
			staticMesh->attributeTable[i].VertexStart,			//UINT MinIndex
			staticMesh->attributeTable[i].VertexCount,			//UINT NumVertices
			staticMesh->attributeTable[i].FaceStart * 3,		//UINT StartIndex
			staticMesh->attributeTable[i].FaceCount);			//UINT PrimitiveCount

		effect->EndPass();
		effect->End();
	}



	// テクスチャの設定をNULLにする
	//device->SetTexture(0, NULL);

	//後始末
	device->SetStreamSourceFreq(0, 1);
	device->SetStreamSourceFreq(1, 1);



	//デバッグ用情報オブジェクトの描画
	debugRender();
}

//===================================================================================================================================
//【スタティックメッシュの取得】
//===================================================================================================================================
StaticMesh* StaticMeshObject::getStaticMesh()
{

	return staticMesh;
}