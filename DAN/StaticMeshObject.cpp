//===================================================================================================================================
//【StaticMeshObject.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/23
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
	onTransparent			= false;
	onLight					= true;
	didDelete				= false;
	didGenerate				= false;
	objectNum				= 0;
	fillMode				= staticMeshObjectNS::FILLMODE::SOLID;
	matrixBuffer			= NULL;
	worldMatrix				= NULL;
	declaration				= NULL;

	//頂点宣言
	D3DVERTEXELEMENT9 vertexElement[65];
	int i = 0;
	while(staticMesh->vertexElement[i].Type != D3DDECLTYPE_UNUSED)
	{
		vertexElement[i] = staticMesh->vertexElement[i];
		i++;
	}
	vertexElement[i] = { 1, 0,					D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 };
	i++;
	vertexElement[i] = { 1, sizeof(float)*4,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 2 };
	i++;
	vertexElement[i] = { 1, sizeof(float)*4*2,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 3 };
	i++;
	vertexElement[i] = { 1, sizeof(float)*4*3,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 4 };
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
	SAFE_RELEASE(matrixBuffer);
	SAFE_RELEASE(colorBuffer);
	SAFE_RELEASE(uvBuffer);

	SAFE_DELETE_ARRAY(worldMatrix);
	SAFE_DELETE_ARRAY(color);

	for (int i = 0; i < objectNum; i++)
	{
		SAFE_DELETE(*objectList->getValue(i));
	}
	objectList->terminate();
	SAFE_DELETE(objectList);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void StaticMeshObject::update()
{
	for (int i = 0; i < objectNum; i++)
	{
		(*objectList->getValue(i))->update();			//更新処理
		deleteObject(i);								//削除処理
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
	updateWorldMatrix();
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
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA		| objectNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA		| 1);

	//頂点宣言を追加
	device->SetVertexDeclaration(declaration);

	//ストリームにメッシュの頂点バッファをバインド
	device->SetStreamSource(0, staticMesh->vertexBuffer,	0, staticMesh->numBytesPerVertex);
	device->SetStreamSource(1, matrixBuffer,						0, sizeof(D3DXMATRIX));

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
		if(onLight)		effect->BeginPass(0);
		else			effect->BeginPass(1);

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

//===================================================================================================================================
//【ワールドマトリックスバッファを更新する】
//===================================================================================================================================
void StaticMeshObject::updateWorldMatrix()
{
	objectNum = objectList->nodeNum;
	if (objectNum <= 0)return;
	Object* obj;

	for (int i = 0; i < objectNum; i++)
	{
		obj = *objectList->getValue(i);
		worldMatrix[i] = obj->matrixWorld;
	}

	copyVertexBuffer(sizeof(D3DXMATRIX)*objectNum, worldMatrix, matrixBuffer);

}

//===================================================================================================================================
//【カラーバッファを更新する】
//===================================================================================================================================
void StaticMeshObject::updateColor()
{
	objectNum = objectList->nodeNum;
	if (objectNum <= 0)return;

	for (int i = 0; i < objectNum; i++)
	{
		//color[i] = (*objectList->getValue(i))->color;
	}
	copyVertexBuffer(sizeof(D3DXCOLOR)*objectNum, color, colorBuffer);
}

//===================================================================================================================================
//【オブジェクトの生成】
//===================================================================================================================================
void StaticMeshObject::generateObject(Object* object)
{
	objectList->insertFront(object);			//オブジェクトを作成
	didGenerate = true;
}

//===================================================================================================================================
//【オブジェクトの全削除】
//===================================================================================================================================
void StaticMeshObject::allDelete()
{
	for (int i = 0; i < objectNum; i++)
	{
		SAFE_DELETE(*objectList->getValue(i));
	}
	objectList->allClear();
	objectNum = objectList->nodeNum;
}

//===================================================================================================================================
//【IDを使用してオブジェクトの削除】
//===================================================================================================================================
void StaticMeshObject::deleteObjectByID(int id)
{
	for (int i = 0; i < objectNum; i++)
	{	
		if ((*objectList->getValue(i))->id == id)
		{
			(*objectList->getValue(i))->existenceTimer = 0.0f;//存在時間を0にして削除可能状態にする
			return;		//検索終了
		}
	}
}

//===================================================================================================================================
//【オブジェクトの削除】
//===================================================================================================================================
void StaticMeshObject::deleteObject(int i)
{
	if ((*objectList->getValue(i))->existenceTimer > 0)return;
	SAFE_DELETE(*objectList->getValue(i));
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

//===================================================================================================================================
//【バッファのリソース更新】
//===================================================================================================================================
void StaticMeshObject::updateBuffer()
{
	objectNum = objectList->nodeNum;				//オブジェクトの数の取得

	//ワールドマトリックス情報バッファの再作成
	SAFE_RELEASE(matrixBuffer);
	device->CreateVertexBuffer(sizeof(D3DXMATRIX)*objectNum, 0, 0, D3DPOOL_MANAGED, &matrixBuffer, 0);
}

//===================================================================================================================================
//【copy用配列のメモリ更新】
//===================================================================================================================================
void StaticMeshObject::updateArray()
{
	//インスタンス数の取得
	objectNum = objectList->nodeNum;

	//ワールドマトリックスバッファ用配列の再作成
	SAFE_DELETE_ARRAY(worldMatrix);
	worldMatrix = new D3DXMATRIX[objectNum];

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
//【setter】
//===================================================================================================================================
void StaticMeshObject::enableLight()	{ onLight = true; }
void StaticMeshObject::disableLight()	{ onLight = false; }

//===================================================================================================================================
//【スタティックメッシュの取得】
//===================================================================================================================================
StaticMesh* StaticMeshObject::getStaticMesh()
{

	return staticMesh;
}
