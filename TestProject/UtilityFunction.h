//===================================================================================================================================
//yUtiltyFunction.hz
// [ì¬Ò]HALGP12A332 11 ì ÷
// [ì¬ú]2019/09/20
// [XVú]2019/09/22
//===================================================================================================================================
#pragma once
//===================================================================================================================================
//yÛèz
//ÉÀWÌ©¼µ
//===================================================================================================================================

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
//yKvÈCut@CÌ[hz
//===================================================================================================================================
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")

//===================================================================================================================================
//yxñ\¦h~z
//===================================================================================================================================
#pragma warning(disable:4305)
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#pragma warning(disable:4111)

//===================================================================================================================================
//y¼OóÔz
//===================================================================================================================================
namespace utility
{
	struct Polar3D
	{
		float radius;	//®a			[è`æ] 0<= raduys <= 
		float theta;	//Ép(V¸p)  [è`æ] 0<= theta  <= Î
		float phi;		//ûÊp(Îp)  [è`æ] 0<= phi    <  2Î
	};
}

//===================================================================================================================================
//yÄpÖNXz
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
	static T clamp(T x, T low, T high);

	//yÏ·zÉÀW->¼ðÀW
	//Polar Coordinates Transformation
	static D3DXVECTOR3 fromTransformationPolar3D(float radius, float theta, float phi);
	//yÏ·z¼ðÀW->ÉÀW
	static utility::Polar3D fromRectangular3D(D3DXVECTOR3 coordinate);
	//1âÔÖ
	static float lerp(float start, float end, float time);
};