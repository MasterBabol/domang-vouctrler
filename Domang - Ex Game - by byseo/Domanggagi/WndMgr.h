#pragma once

#include "Macros.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mmsystem.h>

class opzWndMgr
{
public:
	static void S_Create(HINSTANCE hInstance);
	static void S_Delete();

	static bool S_WinMsgProc();
	static HWND S_GetHandle(){return _instance->_hWnd; }

	static int S_GetWidth(){return _instance->_width;}
	static int S_GetHeight(){return _instance->_height; }

private:
	static opzWndMgr *_instance;

	HWND _hWnd;
	int _width;
	int _height;

	opzWndMgr(HINSTANCE hInstance);
	~opzWndMgr(void);
};

