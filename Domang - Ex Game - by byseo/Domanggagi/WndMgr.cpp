#include "WndMgr.h"
#include "config.h"

#include <assert.h>

/******************
	Callback
******************/
LRESULT CALLBACK WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWindow, message, wParam, lParam);
	}

	return 0;
}

/*******************
		public
*******************/
void opzWndMgr::S_Create(HINSTANCE hInstance)
{
	if(_instance == NULL)
	{
		new opzWndMgr(hInstance);
	}
}

void opzWndMgr::S_Delete()
{
	SafeDelete(_instance);
}

bool opzWndMgr::S_WinMsgProc()
{
	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

/********************
		private
********************/
opzWndMgr *opzWndMgr::_instance = NULL;

opzWndMgr::opzWndMgr(HINSTANCE hInstance)
{
	_instance = this;

	WNDCLASS wnd = {0};
	wnd.hInstance = hInstance;
	wnd.lpszClassName = WND_CLASS_NAME;
	wnd.lpfnWndProc = (WNDPROC)WndProc;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
//	wnd.hCursor = LoadCursor(NULL, NULL);
	RegisterClass(&wnd);

	_width = SCREEN_WIDTH;
	_height = SCREEN_HEIGHT;
	RECT rc = {0, 0, _width, _height};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
#ifdef WINDOW_SCREEN
	_hWnd = CreateWindow(WND_CLASS_NAME, WND_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right, rc.bottom, 0, 0, hInstance, 0);
#endif
#ifdef FULL_SCREEN
	_hWnd = CreateWindow(WINDOW_CLASS_NAME, TITLE, WS_EX_TOPMOST | WS_POPUP, 0, 0,
		_width, _height, 0, 0, hInstance, 0);
#endif
	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);

//	SetCursor(NULL);
}

opzWndMgr::~opzWndMgr(void)
{
	PostQuitMessage(0);
}
