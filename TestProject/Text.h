//===================================================================================================================================
//yText.hz
// [ì¬Ò]HAL“Œ‹GP12A332 11 ›–ì ÷
// [ì¬“ú]2019/05/16
// [XV“ú]2019/08/07
//===================================================================================================================================
#pragma once
#include "Base.h"
#include <string>



class Text : public Base
{
	D3DXCOLOR color;
	LPD3DXFONT font;
public:

	Text();
	~Text();
	void initialize(LPDIRECT3DDEVICE9 device, int height, int width, D3DXCOLOR _color);
	void initialize(LPDIRECT3DDEVICE9 device, int height, int width, D3DXCOLOR _color,LPCSTR fontName);
	void print(float x, float y, const char* string, ...);
};

