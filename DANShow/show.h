//show.h
#pragma once

#include <dshow.h>
#pragma comment(lib,"strmiids.lib")

class SHOW
{
public:
	SHOW(HWND hWnd);
	~SHOW();

	void playShow(void);
	void pauseShow(void);

private:
	IGraphBuilder	*graph;
	IMediaControl	*control;
	IMediaEvent		*event;
	IVideoWindow		*videoWindow;
};