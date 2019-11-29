// test.cpp
#include "test.h"

TestPic::TestPic(HWND hWnd)
{
	LoadTexture(GetDevice(), &testpic, "wood.jpg");

	testpic.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RECT rect = { 0 };
	GetClientRect(hWnd, &rect);

	testpic.size = D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT);

	MakeVtx4PointData(&testpic);
}

TestPic::~TestPic()
{
	testpic.texture->Release();
}

void TestPic::draw()
{
	DrawNormalObject(GetDevice(), testpic);
}