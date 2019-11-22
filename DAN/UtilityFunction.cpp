//===================================================================================================================================
//【UtiltyFunction.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/22
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "UtilityFunction.h"

//===================================================================================================================================
//using宣言
//===================================================================================================================================
using namespace utility;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
UtilityFunction::UtilityFunction()
{

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
UtilityFunction::~UtilityFunction()
{

}

//===================================================================================================================================
//【極座標から直交座標の３次元座標を求める】
//[引数]
//float radius	：動径			[定義域] 0<= radius <= ∞
//float theta	：極角(天頂角)  [定義域] 0<= theta  <= π
//float phi		：方位角(偏角)  [定義域] 0<= phi    <  2π
//[戻値]直交座標の3次元座標
//===================================================================================================================================
D3DXVECTOR3 UtilityFunction::fromTransformationPolar3D(float _radius,float _theta,float _phi)
{
	float radius = max(0, _radius);
	float theta = _theta;//wrap(_theta,0.0f,(float)(2.0f*D3DX_PI));
	float phi = (theta>D3DX_PI? -1 : 1) *  wrap(_phi,0.0f,(float)(2.0f*D3DX_PI));
	D3DXVECTOR3 result;
	result.x = 
		radius * sinf(theta) * cosf(phi);
	result.y = 
		radius * cosf(theta);
	result.z = 
		radius * sinf(theta) * sinf(phi);
	return result;
}

//===================================================================================================================================
//【直交座標から極座標の３値を求める】
//[引数]
//float radius	：動径			[定義域] 0<= radius <= ∞
//float theta	：極角(天頂角)  [定義域] 0<= theta  <= π
//float phi		：方位角(偏角)  [定義域] 0<= phi    <  2π
//[戻値]直交座標の3次元座標
//===================================================================================================================================
Polar3D UtilityFunction::fromRectangular3D(D3DXVECTOR3 coordinate)
{
	float x = coordinate.x;
	float y = coordinate.y;
	float z = coordinate.z;
	Polar3D result;
	result.radius = sqrtf(x*x+y*y+z*z);
	result.theta = (result.radius != 0) ? acosf(y/result.radius) : 0;
	result.phi = (x != 0) ? atanf(z/x) : 0;



	if (x > 0 && z > 0) {}						//++
	else if(x<0&&z>0)result.phi += D3DX_PI;		//-+
	else if(x<0&&z<=0)result.phi += D3DX_PI;	//--
	else if(x>0&&z<=0)result.phi += 2*D3DX_PI;	//+-

	return result;
}

//===================================================================================================================================
//【1次補間関数】
//===================================================================================================================================
float UtilityFunction::lerp(float start,float end, float rate)
{
	float result;
	clamp(rate, 0.0f, 1.0f);
	result = start * (1 - rate) + end * rate;
	return result;
}

//===================================================================================================================================
//【(int型)ラップ関数】
//===================================================================================================================================
int UtilityFunction::wrap(int x, int low, int high)
{
	assert(low < high);
	const int n = (x - low) % (high - low);
	return (n >= 0) ? (n + low) : (n + high);
}

//===================================================================================================================================
//【(float型)ラップ関数】
//===================================================================================================================================
float UtilityFunction::wrap(float x, float low, float high)
{
	assert(low < high);
	const float n = std::fmod(x - low,high - low);
	return (n >= 0) ? (n + low) : (n + high);
}

//===================================================================================================================================
//【視錐台カリング】
//===================================================================================================================================
bool UtilityFunction::culling(D3DXVECTOR3 center, float radius, D3DXMATRIX view, float angle, float  nearClip, float farClip, float aspect)
{
	D3DXPLANE VFLeftPlane, VFRightPlane, VFTopPlane, VFBottomPlane;
	//まず、ジオメトリの位置ベクトルをワールドからビュー空間に変換
	D3DXVECTOR3 position;
	D3DXVec3TransformCoord(&position, &center, &view);

	//左右、上下の平面を計算
	{
		D3DXVECTOR3 p1, p2, p3;
		//左面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -farClip * ((FLOAT)tan(angle / 2)*aspect);
		p2.y = -farClip * (FLOAT)tan(angle / 2);
		p2.z = farClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFLeftPlane, &p1, &p2, &p3);
		//右面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = farClip * ((FLOAT)tan(angle / 2)*aspect);
		p2.y = farClip * (FLOAT)tan(angle / 2);
		p2.z = farClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFRightPlane, &p1, &p2, &p3);
		//上面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -farClip * ((FLOAT)tan(angle / 2)*aspect);
		p2.y = farClip * (FLOAT)tan(angle / 2);
		p2.z = farClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFTopPlane, &p1, &p2, &p3);
		//下面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = farClip * ((FLOAT)tan(angle / 2)*aspect);
		p2.y = -farClip * (FLOAT)tan(angle / 2);
		p2.z = farClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFBottomPlane, &p1, &p2, &p3);
	}

	//6つの平面とジオメトリ境界球をチェック
	{
		//ニアクリップ面について
		if ((position.z + radius) < nearClip)
		{
			return false;
		}
		//ファークリップ面について
		if ((position.z - radius) > farClip)
		{
			return false;
		}
		//左クリップ面について
		FLOAT Distance = (position.x * VFLeftPlane.a) + (position.z * VFLeftPlane.c);
		if (Distance > radius)
		{
			return false;
		}
		//右クリップ面について
		Distance = (position.x * VFRightPlane.a) + (position.z * VFRightPlane.c);
		if (Distance > radius)
		{
			return false;
		}
		//上クリップ面について
		Distance = (position.y * VFTopPlane.b) + (position.z * VFTopPlane.c);
		if (Distance > radius)
		{
			return false;
		}
		//下クリップ面について
		Distance = (position.y * VFBottomPlane.b) + (position.z * VFBottomPlane.c);
		if (Distance > radius)
		{
			return false;
		}
	}

	return true;

}
