// test.h
#pragma once

#include "main.h"
#include "basic.h"
#include "fastDraw2D.h"

class TestPic
{
public:
	TestPic(HWND hWnd);
	~TestPic();

	void update() {};
	void draw();

private:
	TEXTURE testpic;
};

