//show.cpp
#include "main.h"
#include "input.h"
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
	hr = graph->QueryInterface(IID_IMediaEventEx, (void **)&eventEx);
	hr = graph->QueryInterface(IID_IMediaPosition, (void**)&mediaPos);
	hr = graph->QueryInterface(IID_IVideoWindow, (void **)&videoWindow);

	// Load File
	hr = graph->RenderFile(L"test.wmv", NULL);

	// Video Window
	hr = videoWindow->put_Owner((OAHWND)hWnd);
	hr = videoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	RECT rect = { 0 };
	GetClientRect(hWnd, &rect);
	//hr = videoWindow->SetWindowPosition(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	//hr = videoWindow->SetWindowPosition(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	//hr = videoWindow->SetWindowPosition(0, 0, rect.right, rect.bottom);
	hr = videoWindow->SetWindowPosition(0, 0, rect.right, rect.bottom);

	//“§–¾
	alphaEndTime = clock() + ToMSec(3);
	SetLayeredWindowAttributes(hWnd, (COLORREF)0x000000ff, (clock() / alphaEndTime) * 255, LWA_COLORKEY | LWA_ALPHA);

	// EventEx
	hr = eventEx->SetNotifyWindow((OAHWND)hWnd, WM_USER, 0);
	eventEx->SetNotifyFlags(0);

	// Show
	hr = videoWindow->put_Visible(OATRUE);
	
	// Input
	hr = videoWindow->put_MessageDrain((OAHWND)hWnd);
}

void SHOW::update(HWND hWnd)
{
	// Fade
	if (clock() < alphaEndTime)
	{
		int alpha = ((float)clock() / (float)alphaEndTime) * 255.0f;
		SetLayeredWindowAttributes(hWnd, (COLORREF)0x000000ff, alpha, LWA_COLORKEY | LWA_ALPHA);
	}

	long lEventCode = 0;
	LONG_PTR lEvParam1 = 0;
	LONG_PTR lEvParam2 = 0;

	eventEx->GetEvent(&lEventCode, &lEvParam1, &lEvParam2, 0);


	// Play/Pause
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		OAFilterState filterState;
		control->GetState(NULL, &filterState);

		if (filterState == State_Running)
		{
			setVideoOnOff(false);
			setWindowOnOff(false);
		}
		else
		{
			setVideoOnOff(true);
			setWindowOnOff(true);
		}
	}
	// Loop
	if ((int)lEventCode == EC_COMPLETE)
	{
		mediaPos->put_CurrentPosition(0);
	}
}

void SHOW::setWindowOnOff(bool onOff)
{
	long visble = 0;
	videoWindow->get_Visible(&visble);
	if (onOff)
	{
		videoWindow->put_Visible(OATRUE);
	}
	else
	{
		videoWindow->put_Visible(OAFALSE);
	}
}

void SHOW::setVideoOnOff(bool onOff)
{
	OAFilterState filterState;
	control->GetState(NULL, &filterState);

	if (onOff)
	{
		// Play
		control->Run();
	}
	else
	{
		// Pause
		control->Pause();
	}

}

SHOW::~SHOW()
{
	pauseShow();

	control->Stop();
	videoWindow->put_Visible(OAFALSE);
	videoWindow->put_Owner(NULL);

	graph->Release();
	control->Release();
	eventEx->Release();
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
