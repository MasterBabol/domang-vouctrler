#include "BasicForm.h"

#include "WndMgr.h"
#include "DXMgr.h"
#include <stdarg.h>

opzBasicForm::opzBasicForm(void) :
	_bEnd(false), _nextForm(FORM_NULL)
{
	_hWnd = opzWndMgr::S_GetHandle();
	_wndWidth = opzWndMgr::S_GetWidth();
	_wndHeight = opzWndMgr::S_GetHeight();
	_pd3dDevice = opzDXMgr::S_GetDevice();
}

opzBasicForm::~opzBasicForm(void)
{
}

void opzBasicForm::OnSet()
{
	Set();
}

void opzBasicForm::OnReset()
{
	_bEnd = false;
	_nextForm = FORM_NULL;

	Reset();
}

void opzBasicForm::OnFrameMove()
{
	opzDInput::S_Update();

	FrameMove();
}

void opzBasicForm::OnFrameDraw()
{
	_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	_pd3dDevice->BeginScene();
	
	FrameDraw();
	
	_pd3dDevice->EndScene();
	_pd3dDevice->Present(0, 0, 0, 0);
	return;
}

void opzBasicForm::MsgBox(LPCWSTR str, ...)
{
	//Buffer에 보낼 데이터를 저장한다.
	va_list args;
	static WCHAR buffer[MESSAGE_BOX_BUFFER_LENGTH];

	va_start( args, str );
	
	_vsnwprintf_s(buffer, MESSAGE_BOX_LENGTH, str, args );

	va_end(args);

	MessageBox(_hWnd, buffer, L"돼지가 보낸 메시지", 0);
}
