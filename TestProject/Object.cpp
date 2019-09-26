//===================================================================================================================================
//【Object.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/09/23
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Object.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace objectNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Object::Object()
{
	ZeroMemory(this, sizeof(Object));			//メンバ変数を0で初期化
	alpha = 1.0f;								//α値の設定
	fillMode = objectNS::FILLMODE::SOLID;		//描画方法
	onLighting = true;							//ライティングフラグ
	onTransparent = false;						//透過フラグ
	operationAlpha = false;						//α値操作フラグ
	D3DXMatrixIdentity(&matrixRotation);
	quaternion = D3DXQUATERNION(0, 0, 0, 1);
	axisX.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(1, 0, 0));
	axisY.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(0, 1, 0));
	axisZ.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(0, 0, 1));
	reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(-1, 0, 0));
	reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(0, -1, 0));
	reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(0, 0, -1));
	renderNum = 1;
#ifdef _DEBUG
	axisX.color			= D3DXCOLOR(255, 0, 0, 255);
	axisY.color			= D3DXCOLOR(0, 255, 0, 255);
	axisZ.color			= D3DXCOLOR(0, 0, 255, 255);
	reverseAxisX.color	= D3DXCOLOR(255, 0, 0, 255);
	reverseAxisY.color	= D3DXCOLOR(0, 255, 0, 255);
	reverseAxisZ.color	= D3DXCOLOR(0, 0, 255, 255);
#endif // _DEBUG
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Object::~Object()
{
	
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
HRESULT Object::initialize(D3DXVECTOR3* _position)
{
	//初期位置
	memcpy(position, _position, sizeof(D3DXVECTOR3));
	//事前更新
	update();
	return S_OK;
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Object::update()
{
	if (onActive == false)return;
	//ワールドトランスフォーム（ローカル座標→ワールド座標への変換）	
	D3DXMatrixTranslation(&matrixPosition, position.x, position.y, position.z);

	//クォータニオンを回転量パラメーターに使用する
	D3DXMatrixRotationQuaternion(&matrixRotation, &quaternion);
	D3DXMatrixMultiply(&matrixWorld, &matrixRotation, &matrixPosition);

	//ワールド座標から自身の軸レイを更新する
	axisX.update(position, D3DXVECTOR3(matrixWorld._11,matrixWorld._12,matrixWorld._13));
	axisY.update(position, D3DXVECTOR3(matrixWorld._21,matrixWorld._22,matrixWorld._23));
	axisZ.update(position, D3DXVECTOR3(matrixWorld._31,matrixWorld._32,matrixWorld._33));
	reverseAxisX.update(position, -D3DXVECTOR3(matrixWorld._11, matrixWorld._12, matrixWorld._13));
	reverseAxisY.update(position, -D3DXVECTOR3(matrixWorld._21, matrixWorld._22, matrixWorld._23));
	reverseAxisZ.update(position, -D3DXVECTOR3(matrixWorld._31, matrixWorld._32, matrixWorld._33));
}

//===================================================================================================================================
//【インスタンシング描画設定】
//===================================================================================================================================
//void Object::setInstancing(LPDIRECT3DDEVICE9 device,int num, D3DXVECTOR3* _positionList)
//{
//	renderNum = num;
//	//位置情報バッファの作成
//	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*num, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
//	copyVertexBuffer(sizeof(D3DXVECTOR3)*num, _positionList, positionBuffer);
//
//	//頂点宣言
//	D3DVERTEXELEMENT9 vertexElement[65];
//	int i = 0;
//	while(staticMesh->vertexElement[i].Type != D3DDECLTYPE_UNUSED)
//	{
//		vertexElement[i] = staticMesh->vertexElement[i];
//		i++;
//	}
//	vertexElement[i] = { 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 };
//	i++;
//	vertexElement[i] = D3DDECL_END();
//	i++;
//	device->CreateVertexDeclaration(vertexElement, &declaration);
//}

//===================================================================================================================================
//【インスタンシング描画】
//===================================================================================================================================
//void Object::instancingRender(LPDIRECT3DDEVICE9 device, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon,LPD3DXEFFECT effect)
//{
//	if (onRender == false)return;
//	//device->SetRenderState(D3DRS_LIGHTING, true);
//
//	D3DMATERIAL9 matDef;
//	device->GetMaterial(&matDef);
//
//	//インスタンス宣言
//	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | renderNum);
//	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
//
//	//頂点宣言を追加
//	device->SetVertexDeclaration(declaration);
//
//	//ストリームにメッシュの頂点バッファをバインド
//	device->SetStreamSource(0, staticMesh->vertexBuffer, 0, staticMesh->numBytesPerVertex);
//	device->SetStreamSource(1, positionBuffer, 0, sizeof(D3DXVECTOR3));
//
//	//インデックスバッファをセット
//	device->SetIndices(staticMesh->indexBuffer);
//
//	effect->SetTechnique("mainTechnique");
//	effect->SetMatrix("matrixProjection", &projection);
//	effect->SetMatrix("matrixView", &view);
//
//	effect->Begin(0, 0);
//	effect->BeginPass(0);
//
//	// レンダリング
//	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
//	{
//		//device->SetMaterial(&staticMesh->materials[i]);
//		effect->SetValue("diffuse", &staticMesh->materials[i].Diffuse,16);
//		effect->SetTexture("textureDecal", staticMesh->textures[i]);
//
//		device->DrawIndexedPrimitive(
//			D3DPT_TRIANGLELIST,									//D3DPRIMITIVETYPE Type
//			0,													//INT BaseVertexIndex
//			staticMesh->attributeTable[i].VertexStart,			//UINT MinIndex
//			staticMesh->attributeTable[i].VertexCount,			//UINT NumVertices
//			staticMesh->attributeTable[i].FaceStart * 3,		//UINT StartIndex
//			staticMesh->attributeTable[i].FaceCount);			//UINT PrimitiveCount
//	}
//
//	effect->EndPass();
//	effect->End();
//
//	// マテリアルをデフォルトに戻す
//	device->SetMaterial(&matDef);
//	// テクスチャの設定をNULLにする
//	device->SetTexture(0, NULL);
//
//	//後始末
//	device->SetStreamSourceFreq(0, 1);
//	device->SetStreamSourceFreq(1, 1);
//
//}

//===================================================================================================================================
//【デバッグ用描画】
//===================================================================================================================================
void Object::debugRender()
{
#ifdef _DEBUG
	axisX.render(10.0f);
	axisY.render(10.0f);
	axisZ.render(10.0f);
	reverseAxisX.render(10.0f);
	reverseAxisY.render(10.0f);
	reverseAxisZ.render(10.0f);
#endif // _DEBUG
}

//===================================================================================================================================
//【トゥーンレンダー描画】
//===================================================================================================================================
//void Object::toonRender(LPDIRECT3DDEVICE9 device,D3DXMATRIX view,D3DXMATRIX projection,D3DXVECTOR3 cameraPositon,
//	LPD3DXEFFECT effect, LPDIRECT3DTEXTURE9 textureShade, LPDIRECT3DTEXTURE9 textureLine)
//{
//	device->SetTransform(D3DTS_WORLD, &matrixWorld);
//
//	effect->SetTechnique("ToonShading");
//	effect->SetMatrix("mProj", &projection);
//	effect->SetMatrix("mView", &view);
//	effect->SetMatrix("mWorld", &matrixWorld);
//	effect->SetTexture("ShadeTexture", textureShade);
//	effect->SetTexture("LineTexture", textureLine);
//
//	effect->SetVector("LightPos", &D3DXVECTOR4(0, 0, 5, 1));
//	effect->SetVector("EyePos", (D3DXVECTOR4*)&cameraPositon);
//	effect->Begin(NULL, 0);
//
//	// レンダリング			
//	for (DWORD i = 0; i <staticMesh->numMaterial; i++)
//	{
//		effect->BeginPass(0);
//		effect->SetFloatArray("Diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
//		//effect->SetTexture("DecalTexture", meshTextures[i]);
//		//device->SetMaterial(&meshMaterials[i]);
//		//device->SetTexture(0, meshTextures[i]);
//		staticMesh->mesh->DrawSubset(i);
//		effect->EndPass();
//	}
//	effect->End();
//
//#ifdef _DEBUG
//	axisX.render(10.0f);
//	axisY.render(10.0f);
//	axisZ.render(10.0f);
//	reverseAxisX.render(10.0f);
//	reverseAxisY.render(10.0f);
//	reverseAxisZ.render(10.0f);
//#endif // _DEBUG
//
//}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void Object::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("ObjectInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop		= 1000;
		float limitBottom	= -1000;

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//位置
		ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
		ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);						//スケール
		ImGui::SliderFloat("radius", &radius, 0, limitTop);								//半径
		ImGui::SliderFloat("alpha", &alpha, 0, 255);									//透過値
		ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//速度
		ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//加速度
		ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);					//重力

		ImGui::Checkbox("onGravity", &onGravity);										//重力有効化フラグ
		ImGui::Checkbox("onActive", &onActive);											//アクティブ化フラグ
		ImGui::Checkbox("onRender", &onRender);											//描画有効化フラグ
		ImGui::Checkbox("onLighting", &onLighting);										//光源処理フラグ
		ImGui::Checkbox("onTransparent", &onTransparent);								//透過フラグ
		ImGui::Checkbox("operationAlpha", &operationAlpha);								//透過値の操作有効フラグ
		
		ImGui::SliderInt("renderNum", &renderNum,1,(int)limitTop);						//透過値の操作有効フラグ
	}
#endif // _DEBUG
}

void Object::setGravity(D3DXVECTOR3 source, float power)
{
	D3DXVec3Normalize(&gravity, &source);
	//gravity *= min(power, reverseAxisY.distance);
	gravity *= power;
	
	if (onGravity)acceleration += gravity;
}

void Object::activation()
{
	onRender = true;
	onActive = true;
}

void Object::inActivation()
{
	onRender = false;
	onActive = false;
}

void Object::setAlpha(float value)
{
	alpha = value;
}

void Object::switchTransparent(bool flag)
{
	onTransparent = flag;
}

void Object::switchOperationAlpha(bool flag)
{
	operationAlpha = flag;
}

void Object::setRenderFlag(bool flag)
{
	onRender = flag;
}