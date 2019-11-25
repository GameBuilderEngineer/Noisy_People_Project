//show.cpp
#include "main.h"
#include "show.h"

SHOW::SHOW(HWND hWnd)
{
	// COM Lib Init
	HRESULT hr = CoInitialize(NULL);

	// Create Filter
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (void **)&graph);

	// Functions
	hr = graph->QueryInterface(IID_IMediaControl, (void **)&control);
	hr = graph->QueryInterface(IID_IMediaEvent, (void **)&event);
	hr = graph->QueryInterface(IID_IVideoWindow, (LPVOID *)&videoWindow);

	// Load File
	hr = graph->RenderFile(L"test.wmv", NULL);

	// Video Window
	hr = videoWindow->put_Owner((OAHWND)hWnd);
	hr = videoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	RECT rect = { 0 };
	//GetClientRect(hWnd, &rect);
	hr = videoWindow->SetWindowPosition(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	hr = videoWindow->SetWindowPosition(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	//hr = videoWindow->SetWindowForeground(OATRUE);
	//hr = videoWindow->put_AutoShow(OATRUE);
	
	// Show
	hr = videoWindow->put_Visible(OATRUE);
	
	// Input
	hr = videoWindow->put_MessageDrain((OAHWND)hWnd);
}

SHOW::~SHOW()
{
	pauseShow();

	control->Stop();
	videoWindow->put_Visible(OAFALSE);
	videoWindow->put_Owner(NULL);

	graph->Release();
	control->Release();
	event->Release();
	videoWindow->Release();

	CoUninitialize();
}

void SHOW::playShow()
{
	OAFilterState filterState;
	control->GetState(NULL, &filterState);

	if (filterState == State_Stopped || State_Paused)
	{
		// Play
		control->Run();
	}
}
void SHOW::pauseShow()
{
	OAFilterState filterState;
	control->GetState(NULL, &filterState);

	if (filterState == State_Running)
	{
		// Pause
		control->Pause();
	}
}
