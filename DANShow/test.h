// test.h
#pragma once

#include "main.h"
#include "basic.h"
#include "fastDraw2D.h"

class TestPic
{
public:
	TestPic();
	~TestPic();

	void update() {};
	void draw();

private:
	TEXTURE testpic;
};

