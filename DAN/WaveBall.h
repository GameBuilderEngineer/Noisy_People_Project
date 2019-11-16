//===================================================================================================================================
//yWaveBall.hz
// [์ฌา]HALGP12A332 16 ไ๏ F
// [์ฌ๚]2019/11/15
// [XV๚]2019/11/15
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"

//===================================================================================================================================
//y่z
//===================================================================================================================================
#define LINE_NUM_VERTEX (2)
// Qc|Sธ_tH[}bg( ธ_ภW[2D] / ฝห๕)
#define	FVF_VERTEX_LINE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

//===================================================================================================================================
//y\ขฬz
//===================================================================================================================================
struct VertexLine
{
	D3DXVECTOR3		vtx;		// ธ_ภW
	D3DCOLOR			diffuse;	// ฝห๕
};

//===================================================================================================================================
//g`{[NX
//===================================================================================================================================
class WaveBall
{
public:
	WaveBall();
	~WaveBall();

	void draw(void);

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
	LPD3DXLINE pLine;
	VertexLine vertexWk[LINE_NUM_VERTEX]; // ธ_๎๑i[[N
};