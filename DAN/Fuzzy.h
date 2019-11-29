//-----------------------------------------------------------------------------
// ファジー理論 [Fuzzy.h]
// Author：GP12A332 32 中込和輝
// 作成日：2019/6/23
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
struct FuzzyDistance
{
	float	nearSet;
	float	normalSet;
	float	farSet;
};

class Fuzzy
{
public:
	// メンバーシップ関数「上昇斜辺」／
	float grade(float value, float x0, float x1);

	// メンバーシップ関数「下降斜辺」＼
	float reverseGrade(float value, float x0, float x1);

	// メンバーシップ関数「三角形」＿／＼＿
	float triangle(float value, float x0, float x1, float x2);

	// メンバーシップ関数「台形」＿／￣＼＿
	float trapenoid(float value, float x0, float x1, float x2, float x3);

	// ヘッジ関数「とても強く」
	float hedgeVery(float a);

	// ヘッジ関数「とても弱く」
	float hedgeNotVery(float a);

	// ファジーAND演算
	float AND(float a, float b);

	// ファジーOR演算
	float OR(float a, float b);

	// ファジーNOT演算
	float NOT(float a);
};
