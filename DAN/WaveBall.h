//===================================================================================================================================
//【WaveBall.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/11/15
// [更新日]2019/11/15
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"

//===================================================================================================================================
//【定数】
//===================================================================================================================================
#define LINE_NUM_VERTEX (2)
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光)
#define	FVF_VERTEX_LINE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

#define ON_COL_IN			D3DCOLOR(D3DCOLOR_RGBA(255,0,0,255))
#define OFF_COL_IN			D3DCOLOR(D3DCOLOR_RGBA(255,0,0,50))
#define MOUSE_IN_COL_IN		D3DCOLOR(D3DCOLOR_RGBA(255,0,0,255))
#define MOUSE_OUT_COL_IN		D3DCOLOR(D3DCOLOR_RGBA(255,0,0,50))

//===================================================================================================================================
//波形ボールクラス
//===================================================================================================================================
class WaveBall
{
public:
	WaveBall();
	~WaveBall();

	void draw(void);
	void setMouseInCol(bool flag) 
	{
		if ((flag) && (!switchFlag))diffuse = MOUSE_IN_COL_IN;
		else diffuse = MOUSE_OUT_COL_IN;
	}
	void setOnCol(bool flag)
	{
		if (flag)diffuse = ON_COL_IN;
		else diffuse = OFF_COL_IN;
		switchFlag = flag;
	}
	void setVolume(float inVolume)
	{
		volume = inVolume;
	}

	static int dataMax;
	static float *fData;

	static void SetWaveData(int inDataMax, float *inFdata)
	{
		SAFE_DELETE_ARRAY(fData);
		dataMax = inDataMax - 1;
		fData = new float[dataMax];
		memset(fData, 0, dataMax * sizeof(float));
		memcpy(fData, inFdata, dataMax * sizeof(float));
	};

private:

	bool switchFlag;
	float volume;
	LPD3DXLINE pLine;
	D3DCOLOR	 diffuse;		// 反射光
};