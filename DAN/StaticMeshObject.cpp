//===================================================================================================================================
//【StaticMeshObject.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/16
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "StaticMeshObject.h"
#include "Direct3D9.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
StaticMeshObject::StaticMeshObject(StaticMesh* _staticMesh)
{
	device					= getDevice();		//デバイスの取得
	this->staticMesh		= _staticMesh;
	onRender				= true;
	didDelete				= false;
	didGenerate			= false;
	onTransparent		= false;
	objectNum				= 0;
	fillMode					= staticMeshObjectNS::FILLMODE::SOLID;
	positionBuffer			= NULL;
	declaration				= NULL;
	position					= NULL;

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

	objectList = new staticMeshObjectNS::ObjectList();

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
StaticMeshObject::~StaticMeshObject()
{
	objectList->terminate();
	SAFE_DELETE(objectList);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void StaticMeshObject::update()
{
	Object* obj;

	for (int i = 0; i < objectNum; i++)
	{
		obj = *objectList->getValue(i);
		obj->update();
	}


	if (didDelete || didGenerate)
	{
		objectList->listUpdate();
		updateBuffer();
		updateArray();
		didGenerate = false;
		didDelete = false;
	}

	//値の更新
	updatePosition();

}

//===================================================================================================================================
//【バッファのリソース更新】
//===================================================================================================================================
void StaticMeshObject::updateBuffer()
{
	objectNum = objectList->nodeNum;				//オブジェクトの数の取得

	//位置情報バッファの再作成
	SAFE_RELEASE(positionBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*objectNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
}

//===================================================================================================================================
//【copy用配列のメモリ更新】
//===================================================================================================================================
void StaticMeshObject::updateArray()
{
	//インスタンス数の取得
	objectNum = objectList->nodeNum;

	//位置バッファ用配列の再作成
	SAFE_DELETE_ARRAY(position);
	position = new D3DXVECTOR3[objectNum];

}

//===================================================================================================================================
//【位置バッファを更新する】
//===================================================================================================================================
void StaticMeshObject::updatePosition()
{
	objectNum = objectList->nodeNum;
	if (objectNum <= 0)return;
	Object* obj;

	for (int i = 0; i < objectNum; i++)
	{
		obj = *objectList->getValue(i);
		position[i] = obj->position;
	}

	copyVertexBuffer(sizeof(D3DXVECTOR3)*objectNum, position, positionBuffer);

}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void StaticMeshObject::render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	//描画が有効でない場合終了
	if (onRender == false)return;
	if (objectNum <= 0)return;
	
	//レンダーステートの設定
	device->SetRenderState(D3DRS_FILLMODE, fillMode);

	//透過処理
	if (onTransparent)
	{//有効
		//αブレンディングを設定する
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);						//
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			//
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);	//
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	//
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);					//加算合成を行う
	}
	else
	{//無効
		//αブレンディングを設定しない
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	//インスタンス宣言
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA		| objectNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA		| 1);

	//頂点宣言を追加
	device->SetVertexDeclaration(declaration);

	//ストリームにメッシュの頂点バッファをバインド
	device->SetStreamSource(0, staticMesh->vertexBuffer,	0, staticMesh->numBytesPerVertex);
	device->SetStreamSource(1, positionBuffer,						0, sizeof(D3DXVECTOR3));

	//インデックスバッファをセット
	device->SetIndices(staticMesh->indexBuffer);

	effect->SetTechnique("mainTechnique");

	//シェーダへ値をセット
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);	
	//effect->SetValue("positionList", position,objectNum*sizeof(D3DXVECTOR3));


	// レンダリング
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		effect->SetTexture("textureDecal", staticMesh->textures[i]);
		
		//シェーダー更新
		//effect->CommitChanges();
		effect->Begin(0, 0);
		effect->BeginPass(0);

		//Drawコール
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,										//D3DPRIMITIVETYPE Type			:描画タイプ
			0,																		//INT BaseVertexIndex				:オフセット
			staticMesh->attributeTable[i].VertexStart,			//UINT MinIndex						:その属性グループの頂点で最小のインデックス
			staticMesh->attributeTable[i].VertexCount,			//UINT NumVertices					:頂点数（その属性グループの頂点数）
			staticMesh->attributeTable[i].FaceStart * 3,			//UINT StartIndex						:インデックスのインデックス（インデックスバッファ内のインデックス）
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

//===================================================================================================================================
//【スタティックメッシュの取得】
//===================================================================================================================================
StaticMesh* StaticMeshObject::getStaticMesh()
{
	return staticMesh;
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
#ifdef _DEBUG
void StaticMeshObject::outputGUI()
{
	Object* obj;
	for (int i = 0; i < objectNum; i++)
	{
		obj = *objectList->getValue(i);
		obj->outputGUI();
	}
}
#endif // _DEBUG

//===================================================================================================================================
//【オブジェクトの生成】
//===================================================================================================================================
void StaticMeshObject::generateObject(Object* object)
{
	objectList->insertFront(object);			//オブジェクトを作成
	didGenerate = true;
}

//===================================================================================================================================
//【オブジェクトの削除】
//===================================================================================================================================
void StaticMeshObject::deleteObject(int i)
{
	Object* obj = *objectList->getValue(i);
	if (obj->existenceTimer >= 0)return;
	objectList->remove(objectList->getNode(i));		//リスト内のオブジェクトを削除
	didDelete = true;
}

//===================================================================================================================================
//【ランダムアクセス用配列の更新】
//===================================================================================================================================
void StaticMeshObject::updateAccessList()
{
	objectList->listUpdate();
}
