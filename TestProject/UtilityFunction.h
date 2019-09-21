//===================================================================================================================================
//【UtiltyFunction.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/08/07
// [更新日]2019/08/07
//===================================================================================================================================
#pragma once
#include <stdio.h>
#include <crtdbg.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>
#include <cmath>
#include <iostream>
//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
//警告非表示
#pragma warning(disable:4305)
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#pragma warning(disable:4111)

namespace utility
{
	struct Polar3D
	{
		float radius;	//動径			[定義域] 0<= raduys <= ∞
		float theta;	//極角(天頂角)  [定義域] 0<= theta  <= π
		float phi;		//方位角(偏角)  [定義域] 0<= phi    <  2π
	};
}

class UtilityFunction
{
public:
	UtilityFunction();
	~UtilityFunction();

	static int wrap(int x, int low, int high);
	static float wrap(float x, float low, float high);
	template<typename T>
	static T clamp(T x, T low, T high);

	//【変換】極座標->直交座標
	//Polar Coordinates Transformation
	static D3DXVECTOR3 fromTransformationPolar3D(float radius, float theta, float phi);
	//【変換】直交座標->極座標
	static utility::Polar3D fromRectangular3D(D3DXVECTOR3 coordinate);
	//1次補間関数
	static float lerp(float start, float end, float time);
};