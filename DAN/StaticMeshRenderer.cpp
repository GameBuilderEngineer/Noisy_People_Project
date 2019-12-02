//===================================================================================================================================
//【StaticMeshRenderer.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/11/11
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "StaticMeshRenderer.h"
#include "Direct3D9.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace staticMeshRendererNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
StaticMeshRenderer::StaticMeshRenderer(StaticMesh* _staticMesh)
{
	device					= getDevice();		//デバイスの取得
	this->staticMesh		= _staticMesh;
	onRender				= true;
	onTransparent			= false;
	onForeground			= false;
	onLight					= true;
	didRegister				= false;
	didUnRegister			= false;
	objectNum				= 0;
	fillMode				= staticMeshRendererNS::FILLMODE::SOLID;
	matrixBuffer			= NULL;
	worldMatrix				= NULL;
	declaration				= NULL;
	renderPass				= LAMBERT_PASS;
	alpha					= 1.0f;

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

	objectList = new staticMeshRendererNS::ObjectList();

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
StaticMeshRenderer::~StaticMeshRenderer()
{
	SAFE_RELEASE(matrixBuffer);
	SAFE_RELEASE(colorBuffer);
	SAFE_RELEASE(uvBuffer);

	SAFE_DELETE_ARRAY(worldMatrix);
	SAFE_DELETE_ARRAY(color);

	objectList->terminate();
	SAFE_DELETE(objectList);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void StaticMeshRenderer::update()
{
	if (didRegister || didUnRegister)
	{
		objectList->listUpdate();
		objectNum = objectList->nodeNum;
	}

	for (int i = 0; i < objectNum; i++)
	{
		unRegisterObject(i);							//解除処理
	}

	if (didRegister|| didUnRegister)
	{
		objectList->listUpdate();
		updateBuffer();
		updateArray();
		didRegister = false;
		didUnRegister = false;
	}

	//値の更新
	updateWorldMatrix();
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void StaticMeshRenderer::render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//描画が有効でない場合終了
	if (onRender == false)return;
	if (objectNum <= 0)return;
	
	//レンダーステートの設定
	device->SetRenderState(D3DRS_FILLMODE, fillMode);

	//インスタンス宣言
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA		| objectNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA		| 1);

	//頂点宣言を追加
	device->SetVertexDeclaration(declaration);

	//ストリームにメッシュの頂点バッファをバインド
	device->SetStreamSource(0, staticMesh->vertexBuffer,	0, staticMesh->numBytesPerVertex);
	device->SetStreamSource(1, matrixBuffer,				0, sizeof(D3DXMATRIX));

	//インデックスバッファをセット
	device->SetIndices(staticMesh->indexBuffer);


	//シェーダへ値をセット
	effect->SetTechnique("mainTechnique");
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);	
	effect->SetFloat("alphaValue", alpha);

	// レンダリング
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		effect->SetTexture("textureDecal", staticMesh->textures[i]);

		//シェーダー更新
		effect->CommitChanges();
		effect->Begin(0, 0);

		//描画パスの切替
		switch (renderPass)
		{
		case LAMBERT_PASS:		effect->BeginPass(0);	break;
		case TEXEL_PASS:		effect->BeginPass(1);	break;
		case TRANSPARENT_PASS:	effect->BeginPass(2);	break;
		case FOREGROUND_PASS:	effect->BeginPass(3);	break;
		}

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
void StaticMeshRenderer::updateWorldMatrix()
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
void StaticMeshRenderer::updateColor()
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
//【描画オブジェクトの登録】
//===================================================================================================================================
void StaticMeshRenderer::registerObject(Object* object)
{
	objectList->insertFront(object);			//オブジェクトポインタを作成
	didRegister = true;
}

//===================================================================================================================================
//【描画オブジェクトの全解除】
//===================================================================================================================================
void StaticMeshRenderer::allUnRegister()
{
	objectList->allClear();
	objectNum = objectList->nodeNum;
}

//===================================================================================================================================
//【IDを使用して描画オブジェクトの解除】
//===================================================================================================================================
void StaticMeshRenderer::unRegisterObjectByID(int id)
{
	//総当たり検索
	for (int i = 0; i < objectNum; i++)
	{	
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//ここで止まるバグあり(2019/11/24)
		//記：菅野
		//おそらくバレットのサイクルに問題あり(←11/25書き換えたので問題なさそう？)
		//他のクラスなどでここで停止するバグがあった場合は菅野へ報告してください。
		//書き換えを行っているので、影響が出ている恐れがあります。
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if ((*objectList->getValue(i))->id == id)
		{
			//unRegisterObject(i);
			//存在時間を0にして削除可能状態にする
			//(*objectList->getValue(i))->existenceTimer = 0.0f;
			objectList->remove(objectList->getNode(i));
			didUnRegister = true;
			//検索終了
			return;	
		}
	}
}

//===================================================================================================================================
//【描画オブジェクトの解除】
//===================================================================================================================================
void StaticMeshRenderer::unRegisterObject(int i)
{
	//if (*objectList->getValue(i)) 
	//{
	//有効値ならタイマーチェック後解除処理
	if ((*objectList->getValue(i))->existenceTimer > 0)return;	//タイマーチェック
	objectList->remove(objectList->getNode(i));					//リスト内のオブジェクトポインタを削除
	didUnRegister = true;
	//}
	//else
	//{
	//	//有効値でなければ自動的に解除
	//	objectList->remove(objectList->getNode(i));					//リスト内のオブジェクトポインタを削除
	//	didUnRegister = true;
	//}
}

//===================================================================================================================================
//【ランダムアクセス用配列の更新】
//===================================================================================================================================
void StaticMeshRenderer::updateAccessList()
{
	objectList->listUpdate();
	objectNum = objectList->nodeNum;
}

//===================================================================================================================================
//【バッファのリソース更新】
//===================================================================================================================================
void StaticMeshRenderer::updateBuffer()
{
	objectNum = objectList->nodeNum;				//オブジェクトの数の取得

	//ワールドマトリックス情報バッファの再作成
	SAFE_RELEASE(matrixBuffer);
	device->CreateVertexBuffer(sizeof(D3DXMATRIX)*objectNum, 0, 0, D3DPOOL_MANAGED, &matrixBuffer, 0);
}

//===================================================================================================================================
//【copy用配列のメモリ更新】
//===================================================================================================================================
void StaticMeshRenderer::updateArray()
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
void StaticMeshRenderer::outputGUI()
{

}
#endif // _DEBUG

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void StaticMeshRenderer::enableLight()			{ onLight = true; }
void StaticMeshRenderer::disableLight()			{ onLight = false; }
void StaticMeshRenderer::enableTransparent()	{ onTransparent = true; }
void StaticMeshRenderer::disableTransparent()	{ onTransparent = false; }
void StaticMeshRenderer::enableForeground()		{ onForeground = true; }
void StaticMeshRenderer::disableForeground()	{ onForeground = false; }
void StaticMeshRenderer::setFillMode(int mode)	{ fillMode = mode; }
void StaticMeshRenderer::setRenderPass(int pass){ renderPass = pass; }
void StaticMeshRenderer::setAlpha(float value)	{ alpha = value; }

//===================================================================================================================================
//【getter】
//===================================================================================================================================
StaticMesh* StaticMeshRenderer::getStaticMesh(){return staticMesh;}
int			StaticMeshRenderer::getObjectNum() { return objectNum; }
float*		StaticMeshRenderer::getAlpha() { return &alpha; }
