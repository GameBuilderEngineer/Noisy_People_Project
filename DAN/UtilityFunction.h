//===================================================================================================================================
//yUtiltyFunction.hz
// [์ฌา]HALGP12A332 11 ์ ๗
// [์ฌ๚]2019/09/20
// [XV๚]2019/10/25
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include <stdio.h>
#include <crtdbg.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>
#include <cmath>
#include <iostream>

//===================================================================================================================================
//yKvศCut@Cฬ[hz
//===================================================================================================================================
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")

//===================================================================================================================================
//yx๑\ฆh~z
//===================================================================================================================================
#pragma warning(disable:4305)
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#pragma warning(disable:4111)

//===================================================================================================================================
//yผO๓ิz
//===================================================================================================================================
namespace utility
{
	struct Polar3D
	{
		float radius;	//ฎa			[่`ๆ] 0<= raduys <= 
		float theta;	//ษp(Vธp)  [่`ๆ] 0<= theta  <= ฮ
		float phi;		//๛สp(ฮp)  [่`ๆ] 0<= phi    <  2ฮ
	};
}

//===================================================================================================================================
//yฤpึNXz
//===================================================================================================================================
class UtilityFunction
{
public:
	//Method
	UtilityFunction();
	~UtilityFunction();
	static int wrap(int x, int low, int high);
	static float wrap(float x, float low, float high);
	template<typename T>
	static T clamp(T x, T low, T high)
	{
		assert(low <= high);
		T result = max(x, low);
		result = min(result, high);
		return result;
	}
	//ไJOF์เฬ๊true;
	static bool culling(D3DXVECTOR3 center, float radius, D3DXMATRIX view, float angle, float  nearClip, float farClip, float aspect);

	//yฯทzษภW->ผ๐ภW
	//Polar Coordinates Transformation
	static D3DXVECTOR3 fromTransformationPolar3D(float radius, float theta, float phi);
	//yฯทzผ๐ภW->ษภW
	static utility::Polar3D fromRectangular3D(D3DXVECTOR3 coordinate);
	//1โิึ
	static float lerp(float start, float end, float time);

	//(float^)ฮึFCำฬ๊๐w่
	static float logarithm(float e, float x);
};