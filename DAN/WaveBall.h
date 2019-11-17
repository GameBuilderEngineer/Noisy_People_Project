//===================================================================================================================================
//yWaveBall.hz
// [ì¬ŽÒ]HAL“Œ‹žGP12A332 16 äï —F„
// [ì¬“ú]2019/11/15
// [XV“ú]2019/11/15
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yƒCƒ“ƒNƒ‹[ƒhz
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"
#include "Sound.h"

//===================================================================================================================================
//y’è”z
//===================================================================================================================================
#define LINE_NUM_VERTEX (2)
// ‚Q‚cƒ|ƒŠƒSƒ“’¸“_ƒtƒH[ƒ}ƒbƒg( ’¸“_À•W[2D] / ”½ŽËŒõ)
#define	FVF_VERTEX_LINE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

#define ON_COL_IN			D3DCOLOR(D3DCOLOR_RGBA(255,0,0,255))
#define OFF_COL_IN			D3DCOLOR(D3DCOLOR_RGBA(255,0,0,50))
#define MOUSE_IN_COL_IN		D3DCOLOR(D3DCOLOR_RGBA(255,0,0,255))
#define MOUSE_OUT_COL_IN		D3DCOLOR(D3DCOLOR_RGBA(255,0,0,50))

//===================================================================================================================================
//”gŒ`ƒ{[ƒ‹ƒNƒ‰ƒX
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
		SoundInterface::SE->setEndPointVoiceVolume(volume);
		SoundInterface::BGM->setEndPointVoiceVolume(volume);
		SoundInterface::S3D->setEndPointVoiceVolume(volume);
		if (inVolume == NULL) { setOnCol(false); }
		else setOnCol(true);
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
	D3DCOLOR	 diffuse;		// ”½ŽËŒõ
};