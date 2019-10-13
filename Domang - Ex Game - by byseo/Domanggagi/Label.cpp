#include "Label.h"

#include <stdio.h>

void opzLabel::CalcRect()
{
	//사각영역을 갱신해주어야 함...
	if(_font == NULL) return;
	RECT rect;
	rect.left = 0; rect.right = 0; rect.top = 0; rect.bottom = 0;
	if(_str != NULL)
		_font->DrawTextW(NULL, _str, -1, &rect, DT_CALCRECT, NULL);

	switch(_dirY)
	{
	case -1:
		_rt.top = _y - rect.bottom;
		_rt.bottom = _y;
		break;

	case 0:
		_rt.top = _y - rect.bottom / 2 - 1;
		_rt.bottom = _y + rect.bottom / 2 + 1;
		break;

	case 1:
		_rt.top = _y;
		_rt.bottom = _y + rect.bottom;
		break;
	}

	switch(_dirX)
	{
	case -1:
		_rt.left = _x - rect.right;
		_rt.right = _x;
		break;

	case 0:
		_rt.left = _x - rect.right / 2 - 1;
		_rt.right = _x + rect.right / 2  + 1;
		break;
		
	case 1:
		_rt.left = _x;
		_rt.right = _x + rect.right;
		break;
	}
}

void opzLabel::SetC(int x, int y, int dirX, int dirY)
{
	_x = x;	_y = y;
	_dirX = dirX;	_dirY = dirY;

	CalcRect();
}

void opzLabel::SetStr(LPCWSTR str, ...)
{
	m_len = 0;
	if(str == NULL) return;
	
	//Buffer에 보낼 데이터를 저장한다.
	va_list args;
	static WCHAR buffer[LABEL_BUFFER_LENGTH];

	va_start( args, str );
	
	_vsnwprintf_s(buffer, LABEL_LENGTH, str, args );
	
	va_end(args);

	wcscpy_s(_str,LABEL_BUFFER_LENGTH, buffer);
	m_len = wcslen(_str);

	CalcRect();
}

int opzLabel::Draw(ID3DXSprite *sprite, DWORD format)
{
	if(_font != NULL && _str != NULL)
		return _font->DrawTextW(sprite, _str, -1, &_rt, format, _color);
	return 0;
}

opzLabel::opzLabel(LPCWSTR _str, POINT _c, int dirX, int dirY, ID3DXFont *font)
	:_font(font), _color(D3DCOLOR_XRGB(0, 0, 0))
{
	SetC(_c, dirX, dirY);
	SetStr(_str);
}

opzLabel::opzLabel(LPCWSTR _str, int sx, int sy, int dirX, int dirY, ID3DXFont *font)
	:_font(font), _color(D3DCOLOR_XRGB(0, 0, 0))
{
	SetC(sx, sy, dirX, dirY);
	SetStr(_str);
}

opzLabel::opzLabel(const opzLabel &other)
	:_font(other._font), _color(D3DCOLOR_XRGB(0, 0, 0))
{
	SetRect(other._rt);
	SetC(other.GetX(), other.GetY(), other._dirX, other._dirY);
	SetStr(other.GetStr());
}

opzLabel::~opzLabel(void)
{
}