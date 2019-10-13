#pragma once

#include "2DVector.h"

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h> 

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

enum Key_State
{
	KEY_DOWN,
	KEY_HOLD,
	KEY_UP	
};
enum Mouse_State
{
	MOUSE_LEFT_DOWN,
	MOUSE_LEFT_HOLD,
	MOUSE_LEFT_UP,
	MOUSE_RIGHT_DOWN,
	MOUSE_RIGHT_HOLD,
	MOUSE_RIGHT_UP,
};

class opzDInput
{
public:
	static void S_Create();
	static void S_Delete();

	static void S_Update();

	static bool S_GetKeyState(int key_,	Key_State state_){return _instance->GetKeyState(key_, state_); }
	static bool S_GetMouseState(Mouse_State state_){return _instance->GetMouseState(state_); }
	static float S_GetPointerX(){return (float)_instance->_pointerPos.x;};
	static float S_GetPointerY(){return (float)_instance->_pointerPos.y;};
	static opz2DVector S_GetPointerPos(){return opz2DVector(S_GetPointerX(), S_GetPointerY());};
	static LONG S_GetDX(){return _instance->_mouseState.lX; }
	static LONG S_GetDY(){return _instance->_mouseState.lY; }

	static void S_SetPointerX(int x){_instance->_pointerPos.x = x;}
	static void S_SetPointerY(int y){_instance->_pointerPos.y = y;}
	static void S_SetPointerPos(POINT pos){_instance->_pointerPos = pos; }
	static void S_SetPointerPos(int x, int y){S_SetPointerX(x); S_SetPointerY(y); }

	static void S_ChangePointerPos(int dx, int dy){S_SetPointerX(_instance->_pointerPos.x + dx); S_SetPointerY(_instance->_pointerPos.y + dy);}

	static void S_AcquireKeyboard();		//���̷�Ʈ ��ǲ�� Ű���带 ����ϴ�.
	static void S_UnacquireKeyboard();	//���̷�Ʈ ��ǲ�� Ű���忡 �޼������� ���� ���ϴ�.

private:
	static opzDInput *_instance;

	HWND _hWnd;
	int _wndWidth;
	int _wndHeight;

	LPDIRECTINPUT8 _pDI; // DirectInput ��ü�� �������Դϴ�.
	LPDIRECTINPUTDEVICE8 _keyboard; // Keyboard ��ü�Դϴ�.
	LPDIRECTINPUTDEVICE8 _mouse; // Keyboard ��ü�Դϴ�.

	char _keyboardState[256];
	char _prevKeyboardState[256];
	DIMOUSESTATE _mouseState;
	DIMOUSESTATE _prevMouseState;
	POINT _pointerPos;

	POINT _defaultPos;
	
	bool	_keyboardAcquire;

	opzDInput(void);
	~opzDInput(void);

	void Update();
	bool GetKeyState(int key_, Key_State state_);
	bool GetMouseState(Mouse_State state_);
};

