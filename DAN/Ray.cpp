//===================================================================================================================================
//【Ray.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/10/29
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Ray.h"
#ifdef _DEBUG
#include "Direct3D9.h"
#endif // _DEBUG

//===================================================================================================================================
//【プロトタイプ宣言】
//===================================================================================================================================
HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices);

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Ray::Ray()
{
	ZeroMemory(this, sizeof(Ray));
	direction = D3DXVECTOR3(0, 0, 1);//Z方向
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Ray::~Ray()
{

}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void Ray::initialize(D3DXVECTOR3 _start, D3DXVECTOR3 _direction)
{
	start = _start;
	direction = _direction;
}

//===================================================================================================================================
//【レイによる衝突判定】
// レイが相手メッシュと交差する場合は、pfDistanceに距離を入れてtrueを返す
// 交差しているポリゴンも返す
//===================================================================================================================================
bool Ray::rayIntersect(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix)
{
	BOOL hit = false;
	D3DXMATRIX Inv;
	//レイを出すメッシュの位置・回転をレイに適用
	//レイを当てるメッシュが動いていたり回転している場合でも対象のワールド行列の逆行列を用いれば正しくレイが当たる
	D3DXMatrixInverse(&Inv, NULL, &targetMatrix);
	D3DXVec3TransformCoord(&start, &start, &Inv);
	D3DXVec3Normalize(&direction, &direction);
	Inv._41 =
	Inv._42 =
	Inv._43 = 0.0f;
	D3DXVec3TransformCoord(&direction, &direction, &Inv);

	DWORD index = 0;//ポリゴンのインデックス保存変数
	//----------------------------------------------------------------------------------------------------
	//D3DXIntersectの結果
	//index:レイと衝突しているポリゴンのインデックス番号を取得する
	//hit:レイとの衝突の検知
	//distance:レイと衝突したポリゴンとの距離
	D3DXIntersect(targetMesh, &start, &direction, &hit, &index, NULL, NULL, &distance, NULL, NULL);
	//----------------------------------------------------------------------------------------------------
	if (hit)
	{
		//交差しているポリゴンの頂点を見つける
		D3DXVECTOR3 vertex[3];
		FindVerticesOnPoly(targetMesh, index, vertex);
		D3DXPLANE plane;
		//その頂点から平面方程式を得る
		D3DXPlaneFromPoints(&plane, &vertex[0], &vertex[1], &vertex[2]);
		//平面方程式の係数が法線の成分
		normal.x = plane.a;
		normal.y = plane.b;
		normal.z = plane.c;

		return true;
	}
	return false;
}

//===================================================================================================================================
//【スリップ関数】
// L:入射ベクトル（レイ） N:ポリゴンの法線
//法線と平行な入射ベクトル成分を打ち消す
//===================================================================================================================================
D3DXVECTOR3 Ray::slip(D3DXVECTOR3 L, D3DXVECTOR3 N)
{
	D3DXVECTOR3 S; //滑りベクトル（滑る方向）

	//滑りベクトル S=L-(N * L)/(|N|^2)*N
	S = L - ((D3DXVec3Dot(&N, &L)) / (pow(D3DXVec3Length(&N), 2)))*N;

	return S;
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Ray::update(D3DXVECTOR3 _start, D3DXVECTOR3 _direction)
{
	start = _start;
	direction = _direction;
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Ray::render(float length)
{
#ifdef _DEBUG
	LPDIRECT3DDEVICE9 device = getDevice();
	device->SetFVF(D3DFVF_XYZ);
	D3DXVECTOR3 vPnt[2];
	vPnt[0] = start;
	vPnt[1] = start + (direction * length);

	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	device->SetTransform(D3DTS_WORLD, &mWorld);

	// アルファ・ブレンディングを行う
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//レイのマテリアル設定
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient =
	mtrl.Diffuse = color;
	device->SetMaterial(&mtrl);
	device->SetTexture(0, NULL);
	device->SetRenderState(D3DRS_LIGHTING, true);

	//レイのレンダリング
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vPnt, sizeof(D3DXVECTOR3));


#endif // _DEBUG
}

//===================================================================================================================================
//【メッシュ上のポリゴンの頂点探査】
//HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh,DWORD dwPolyIndex,D3DXVECTOR3* pvVertices )
//そのポリゴンの頂点を見つける
//===================================================================================================================================
HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices)
{
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVertexNum = pMesh->GetNumVertices();
	DWORD dwPolyNum = pMesh->GetNumFaces();
	WORD* pwPoly = NULL;
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pwPoly);

	BYTE *pbVertices = NULL;
	FLOAT* pfVetices = NULL;
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;
	pMesh->GetVertexBuffer(&VB);
	if (SUCCEEDED(VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pvVertices[0].x = pfVetices[0];
		pvVertices[0].y = pfVetices[1];
		pvVertices[0].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pvVertices[1].x = pfVetices[0];
		pvVertices[1].y = pfVetices[1];
		pvVertices[1].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pvVertices[2].x = pfVetices[0];
		pvVertices[2].y = pfVetices[1];
		pvVertices[2].z = pfVetices[2];

		pMesh->UnlockIndexBuffer();
		VB->Unlock();
		VB->Release();
	}
	return S_OK;
}