//===================================================================================================================================
//【Ocean.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/07
// [更新日]2019/11/07
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Ocean.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Ocean::Ocean() {
	initialize();
};

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Ocean::~Ocean() {
	SAFE_DELETE(object);
};

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Ocean::initialize() {
	object = new Object();
	object->initialize(&D3DXVECTOR3(0, 0, 0));
	object->scale = D3DXVECTOR3(20000, 1, 20000);

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
};

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Ocean::update()
{
	object->update();
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
	device->SetStreamSource(0, staticMesh->vertexBuffer, 0, staticMesh->numBytesPerVertex);

	//インデックスバッファをセット
	device->SetIndices(staticMesh->indexBuffer);


	//シェーダへ値をセット
	effect->SetTechnique("mainTechnique");
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);

	// レンダリング
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		effect->SetTexture("textureDecal", staticMesh->textures[i]);

		//シェーダー更新
		effect->CommitChanges();
		effect->Begin(0, 0);

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
		effect->End();
	}

	device->SetStreamSource(0, NULL, 0, NULL);
	device->SetStreamSource(1, NULL, 0, NULL);

	//後始末
	device->SetStreamSourceFreq(0, 0);
	device->SetStreamSourceFreq(1, 0);
}
