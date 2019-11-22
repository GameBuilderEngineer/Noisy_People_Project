//===================================================================================================================================
//ÅyWaveBall.hÅz
// [çÏê¨é“]HALìåãûGP12A332 16 ‰Ô óFçÑ
// [çÏê¨ì˙]2019/11/15
// [çXêVì˙]2019/11/15
//===================================================================================================================================
#include "WaveBall.h"
#include "Direct3D9.h"

int WaveBall::dataMax = 0;
float *WaveBall::fData = 0;
//===================================================================================================================================
//ÅyÉRÉìÉXÉgÉâÉNÉ^Åz
//===================================================================================================================================
WaveBall::WaveBall()
{
	LPDIRECT3DDEVICE9 device = getDevice();
	D3DXCreateLine(device, &pLine);

	switchFlag = true;
	volume = 1.0f;
	diffuse = ON_COL_IN;
}

//===================================================================================================================================
//ÅyÉfÉXÉgÉâÉNÉ^Åz
//===================================================================================================================================
WaveBall::~WaveBall()
{
	SAFE_DELETE_ARRAY(fData);
	SAFE_RELEASE(pLine);
}

//===================================================================================================================================
//Åyï`âÊÅz
//===================================================================================================================================
void WaveBall::draw(void)
{
	setOnCol(switchFlag);

	const float inRadius = 50;
	const float amplitude = inRadius / 2;
	const float inX = inRadius + amplitude;
	const float inY = inRadius + amplitude;
	const int inSides = dataMax;
	const D3DCOLOR inColor = diffuse;

	const float outX = inX;
	const float outY = inY;
	const int outSides = 10;
	float outRadius = 0;
	float baseRadius = inRadius + (amplitude / 2);
	D3DCOLOR outColor = 0;

	D3DXVECTOR2* inLine;
	D3DXVECTOR2* outLine;
	outLine = new D3DXVECTOR2[outSides + 1];
	inLine = new D3DXVECTOR2[inSides + 1];

	float Theta = NULL;
	float WedgeAngle = NULL;

	WedgeAngle = (float)((2 * D3DX_PI) / inSides);
	for (int i = 0; i <= inSides; i++)
	{
		Theta = i * WedgeAngle;
		float size = 0;
 		if (fData[i] > 0)
		{
			size = amplitude * ((float)(unsigned short)fData[i] / (float)USHRT_MAX);
		}
		inLine[i].x = (inX + (inRadius + size) * cos(Theta));
		inLine[i].y = (inY - (inRadius + size) * sin(Theta));

		if (outRadius < size)
		{
			outRadius = size;
		}
	}

	WedgeAngle = (float)((2 * D3DX_PI) / outSides);
	for (int i = 0; i <= outSides; i++)
	{
		Theta = i * WedgeAngle;
		outLine[i].x = outX + (baseRadius + outRadius)*volume * cos(Theta);
		outLine[i].y = outY - (baseRadius + outRadius)*volume * sin(Theta);
	}

	int a = 155 * (outRadius / amplitude);
	outColor = D3DCOLOR_RGBA(0, 255, 0, a);

	pLine->SetWidth(2);
	pLine->SetAntialias(false);
	pLine->SetGLLines(false);
	pLine->Begin();
	pLine->Draw(inLine, (inSides + 1), inColor);
	if (switchFlag)
	{
		pLine->Draw(outLine, (outSides + 1), outColor);
	}
	pLine->End();
	SAFE_DELETE_ARRAY(inLine);
	SAFE_DELETE_ARRAY(outLine);
}
