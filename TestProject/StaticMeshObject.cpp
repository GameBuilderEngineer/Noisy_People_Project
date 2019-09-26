//===================================================================================================================================
//【StaticMeshObject.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/09/23
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
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
StaticMeshObject::~StaticMeshObject()
{

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
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			//
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		//
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);	//
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	//
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);				//加算合成を行う
	}
	else
	{//無効
		//αブレンディングを設定しない
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	//頂点宣言を追加
	device->SetVertexDeclaration(staticMesh->declaration);

	//ストリームにメッシュの頂点バッファをバインド
	device->SetStreamSource(0, staticMesh->vertexBuffer, 0, staticMesh->numBytesPerVertex);

	//インデックスバッファをセット
	device->SetIndices(staticMesh->indexBuffer);

	// レンダリング			
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		if (operationAlpha) {
			D3DMATERIAL9 material = staticMesh->materials[i];
			material.Diffuse.a = alpha;
			device->SetMaterial(&material);
		}
		else {
			device->SetMaterial(&staticMesh->materials[i]);
		}
		device->SetTexture(0, staticMesh->textures[i]);

		//Drawコール
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,									//D3DPRIMITIVETYPE Type
			0,													//INT BaseVertexIndex
			staticMesh->attributeTable[i].VertexStart,			//UINT MinIndex
			staticMesh->attributeTable[i].VertexCount,			//UINT NumVertices
			staticMesh->attributeTable[i].FaceStart * 3,		//UINT StartIndex
			staticMesh->attributeTable[i].FaceCount);			//UINT PrimitiveCount
	}

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