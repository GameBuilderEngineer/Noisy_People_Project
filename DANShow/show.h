//show.h
#pragma once

#include <dshow.h>
#pragma comment (lib, "strmiids.lib")

class SHOW
{
public:
	SHOW(HWND hWnd);
	~SHOW();

	void playShow(void);
	void pauseShow(void);
	void update(void);

	void setWindowOnOff(bool onOff);
	void setVideoOnOff(bool onOff);

private:
	IGraphBuilder	*graph;
	IMediaControl	*control;
	IMediaEventEx	*eventEx;
	IMediaPosition	*mediaPos;
	IVideoWindow	*videoWindow;
};