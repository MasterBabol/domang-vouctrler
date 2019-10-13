#pragma once

#include "DInput.h"
#include "DXMgr.h"
#include "BGMMgr.h"

#include <algorithm>
#include <vector>
#include <list>
#include <stdarg.h>
#include <time.h>
#include <d3dx9.h>

enum form_t
{
	FORM_PLAY,
	FORM_SCORE,
	FORM_NUMBER,
	FORM_NULL
};

#define MESSAGE_BOX_BUFFER_LENGTH	256
#define MESSAGE_BOX_LENGTH			MESSAGE_BOX_BUFFER_LENGTH - 1

class opzBasicForm
{
public:
	opzBasicForm(void);
	virtual ~opzBasicForm(void);

	void OnSet();
	void OnReset();

	void OnFrameMove();
	void OnFrameDraw();
	
	bool CheckEnd(){return _bEnd; }
	int GetNextForm(){return _nextForm; }

	void SetNextForm(form_t nextForm){_nextForm = nextForm; }
	
protected:
	HWND _hWnd;
	LPDirectDevice _pd3dDevice;
	int _wndWidth;
	int _wndHeight;

	virtual void Set() = 0;
	virtual void Reset() = 0;
	
	virtual void FrameMove() = 0;
	virtual void FrameDraw() = 0;

	ID3DXFont *GetNewFont(LPCWSTR strName, int nSize);

	void MsgBox(LPCWSTR str, ...);

	void Exit(){_bEnd = true;}

private:
	//Æû ÀüÈ¯ °ü·Ã
	bool _bEnd;
	form_t _nextForm;
};

