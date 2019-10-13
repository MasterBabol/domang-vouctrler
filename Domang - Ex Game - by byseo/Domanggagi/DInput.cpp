#include "DInput.h"
#include "MiniFuncs.h"
#include "WndMgr.h"

#include <assert.h>
#include <math.h>

/********************
		public
*******************/
void opzDInput::S_Create()
{
	if(_instance == NULL)
	{
		new opzDInput();
	}
}

void opzDInput::S_Delete()
{
	SafeDelete(_instance);
}

void opzDInput::S_Update()
{
	_instance->Update();
}

void opzDInput::S_AcquireKeyboard()
{
	_instance->_keyboard->Acquire();
	_instance->_keyboardAcquire = true;
}

void opzDInput::S_UnacquireKeyboard()
{
	_instance->_keyboard->Unacquire();
	_instance->_keyboardAcquire = false;
}

/******************
	private
******************/
opzDInput *opzDInput::_instance = NULL;

opzDInput::opzDInput(void) : _hWnd(NULL), _wndWidth(0),
	_wndHeight(0), _keyboardAcquire(true)
{
	_instance = this;

	_hWnd = opzWndMgr::S_GetHandle();
	_wndWidth = opzWndMgr::S_GetWidth();
	_wndHeight = opzWndMgr::S_GetHeight();

	DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&_pDI, NULL);	//DirectInput 객체를 만드는 함수에요.
	assert(_pDI != NULL);
	
	_pDI->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);	//Device 객체를 만드는 함수에요. 이제 fDIKeyboard가 정상적인 Device를 가리키겠죠.
	assert(_keyboard != NULL);

	_keyboard->SetDataFormat(&c_dfDIKeyboard);		//Device를 키보드 포맷으로 맞춰요.
	_keyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE); //이건 몰겠음.
	_keyboard->Acquire();  //이건 키보드를 개방하는 의미에요. 키보드Device에 대한 권한을 얻는 의미에요.
	
	_pDI->CreateDevice(GUID_SysMouse, &_mouse, NULL);
	assert(_mouse != NULL);

	_mouse -> SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	_mouse -> SetDataFormat(&c_dfDIMouse);
	_mouse -> Acquire();
	
	_defaultPos.x = _wndWidth / 2;
	_defaultPos.y = _wndHeight / 2;

	SetCursorPos(_defaultPos.x, _defaultPos.y);
	_pointerPos.x = 0;
	_pointerPos.y = 0;
}


opzDInput::~opzDInput(void)
{
	_keyboard->Unacquire();
	_keyboard->Release();
	_mouse->Unacquire();
	_mouse->Release();
	SafeRelease(_pDI);
}

void opzDInput::Update()
{
	if(GetFocus() != _hWnd) return;

	SetCursor(NULL);
	
	HRESULT res;

	if(_keyboardAcquire)
	{
		CopyMemory(_prevKeyboardState, _keyboardState, sizeof(_keyboardState));
		
		res = _keyboard->GetDeviceState(sizeof(char) * 256, &_keyboardState);
		if(FAILED(res))
		{
			_keyboard->Acquire();
			res = _keyboard->GetDeviceState(sizeof(char) * 256, &_keyboardState);
		}
	}
	else
	{
		memset(_prevKeyboardState, 0x00, sizeof(_keyboardState));
	}

	_prevMouseState = _mouseState;
	res = _mouse -> GetDeviceState(sizeof(DIMOUSESTATE), &_mouseState);
	if(FAILED(res))
	{
		_mouse->Acquire();
		res = _mouse -> GetDeviceState(sizeof(DIMOUSESTATE), &_mouseState);
	}

	static POINT newMousePos;
	static POINT tp;

	LONG dx = _mouseState.lX;
	LONG dy = -_mouseState.lY;

	if(dx != 0 || dy != 0)
	{	
		_pointerPos.x += dx;
		_pointerPos.y += dy;

		tp = _defaultPos;
		ClientToScreen(_hWnd, &tp);
		SetCursorPos(tp.x, tp.y);
	}
}

bool opzDInput::GetKeyState(int key_, Key_State state_)
{
	switch(state_)
	{
	case KEY_DOWN:
		return (_prevKeyboardState[key_] == 0x00 && _keyboardState[key_] == -0x80);
		break;
	case KEY_HOLD:
		return (_keyboardState[key_] == -0x80);
		break;
	case KEY_UP:
		return (_prevKeyboardState[key_] == -0x80 && _keyboardState[key_] == 0x00);
		break;
	}
	return false;
}

bool opzDInput::GetMouseState(Mouse_State state_)
{
	switch(state_)
	{
	case MOUSE_LEFT_DOWN:
		return (_prevMouseState.rgbButtons[0] == 0x00 && _mouseState.rgbButtons[0] == 0x80);
		break;
	case MOUSE_LEFT_HOLD:
		return (_mouseState.rgbButtons[0] == 0x80);
		break;
	case MOUSE_LEFT_UP:
		return (_prevMouseState.rgbButtons[0] == 0x80 && _mouseState.rgbButtons[0] == 0x00);
		break;
	case MOUSE_RIGHT_DOWN:
		return (_prevMouseState.rgbButtons[1] == 0x00 && _mouseState.rgbButtons[1] == 0x80);
		break;
	case MOUSE_RIGHT_HOLD:
		return (_mouseState.rgbButtons[1] == 0x80);
		break;
	case MOUSE_RIGHT_UP:
		return (_prevMouseState.rgbButtons[1] == 0x80 && _mouseState.rgbButtons[1] == 0x00);
		break;
	}
	return false;
}