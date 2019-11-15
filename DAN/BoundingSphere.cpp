//===================================================================================================================================
//【BoundingSphere.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/24
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "BoundingSphere.h"
#include "Direct3D9.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
BoundingSphere::BoundingSphere()
{
	ZeroMemory(this, sizeof(BoundingSphere));
	scale = 1.0f;
}

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
BoundingSphere::BoundingSphere(D3DXVECTOR3* _position,float _radius)
{
	ZeroMemory(this, sizeof(BoundingSphere));
	scale = 1.0f;
	initialize(_position, _radius);
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
BoundingSphere::~BoundingSphere()
{
	SAFE_RELEASE(mesh);
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
//メッシュ情報から初期化
void BoundingSphere::initialize(D3DXVECTOR3* _position, LPD3DXMESH ownerMesh)
{
	LPDIRECT3DDEVICE9 device = getDevice();
	position = _position;

	// メッシュの頂点バッファをロックする
	LPVOID buffer;
	ownerMesh->LockVertexBuffer(D3DLOCK_READONLY, &buffer);

	// メッシュの境界球を求める
	D3DXComputeBoundingSphere(
		(D3DXVECTOR3 *)buffer,
		ownerMesh->GetNumVertices(),
		ownerMesh->GetNumBytesPerVertex(),
		&center, &radius);

	// ロックした頂点バッファをアンロック
	ownerMesh->UnlockVertexBuffer();

	//メッシュを作成
	D3DXCreateSphere(device, radius, 8, 8, &mesh, NULL);

}
//半径情報から初期化
void BoundingSphere::initialize(D3DXVECTOR3* _position,float _raidus)
{
	LPDIRECT3DDEVICE9 device = getDevice();
	position = _position;
	radius = _raidus;
	//メッシュを作成
	D3DXCreateSphere(device, radius, 8, 8, &mesh, NULL);
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void BoundingSphere::render(D3DXMATRIX owner)
{
	LPDIRECT3DDEVICE9 device = getDevice();

	// ライティングモードを設定
	device->SetRenderState(D3DRS_LIGHTING, true);
	// レンダリングモードの設定
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);//ワイヤーフレーム表示
	// アルファ・ブレンディングを行う
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	// 透過処理を行う
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 半透明処理を行う
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	D3DMATERIAL9 matDef;
	// 現在のマテリアルを取得
	device->GetMaterial(&matDef);

	//ワールドマトリックスの設定
	device->SetTransform(D3DTS_WORLD, &owner);

	// テクスチャの設定をNULLにする
	device->SetTexture(0, NULL);

	// マテリアル情報に対するポインタを取得
	D3DMATERIAL9 material;

	// マテリアル情報を緑色
	material.Diffuse = D3DCOLORVALUE(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	material.Ambient = D3DCOLORVALUE(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	material.Specular = D3DCOLORVALUE(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	material.Emissive = D3DCOLORVALUE(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	material.Power = 120.0f;

	// マテリアルの設定
	device->SetMaterial(&material);

	//描画
	mesh->DrawSubset(0);

	//マテリアルをデフォルトに戻す
	device->SetMaterial(&matDef);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//ソリッド表示
}
void BoundingSphere::render()
{
	D3DXMATRIX worldMatrix;
	D3DXMATRIX positionMatrix;
	D3DXMatrixTranslation(&positionMatrix, position->x, position->y, position->z);
	D3DXMATRIX scaleMatrix;
	D3DXMatrixScaling(&scaleMatrix, scale, scale, scale);

	D3DXMatrixIdentity(&worldMatrix);									//正規化
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleMatrix);		//*スケール行列
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &positionMatrix);	//*位置行列

	render(worldMatrix);
}

//===================================================================================================================================
//【衝突判定】
//===================================================================================================================================
bool BoundingSphere::collide(D3DXVECTOR3 targetCenter, float targetRadius, D3DXMATRIX ownerMatrix, D3DXMATRIX targetMatrix)
{
	D3DXVECTOR3 ownerCenter;
	D3DXVECTOR3 _targetCenter;
	D3DXVec3TransformCoord(&ownerCenter, &center, &ownerMatrix);
	D3DXVec3TransformCoord(&_targetCenter, &targetCenter, &targetMatrix);

	//２つの物体の中心間の距離を求める
	float length = D3DXVec3Length(&(_targetCenter - ownerCenter));
	// その距離が、２つの物体の半径を足したものより小さい場合、境界球同士が重なっている（衝突している）ということ
	if (length <= radius + targetRadius)
	{
		return true;
	}
	return false;
}

//===================================================================================================================================
//【setter】
//===================================================================================================================================
void BoundingSphere::setScale(float value)
{
	scale = value;
}

//===================================================================================================================================
//【getter】
//===================================================================================================================================
float BoundingSphere::getRadius() { return radius*scale; }
D3DXVECTOR3 BoundingSphere::getCenter() { return center; }
LPD3DXMESH BoundingSphere::getMesh() { return mesh; }
