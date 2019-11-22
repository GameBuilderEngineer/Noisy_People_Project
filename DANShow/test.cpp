// test.cpp
#include "test.h"

TestPic::TestPic()
{
	LoadTexture(GetDevice(), &testpic, "wood.jpg");

	testpic.pos = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	testpic.size = D3DXVECTOR2(50, 50);

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