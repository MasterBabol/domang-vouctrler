#pragma once

#include "Macros.h"

#include <d3dx9.h>
#include <string.h>

#define LABEL_BUFFER_LENGTH		1024
#define LABEL_LENGTH			LABEL_BUFFER_LENGTH - 1

class opzLabel
{
public:
	opzLabel(LPCWSTR _str, POINT _c, int dirX, int dirY, ID3DXFont *_font);
	opzLabel(const opzLabel &other);
	opzLabel(LPCWSTR _str, int sx, int sy, int dirX, int dirY, ID3DXFont *_font);
	virtual ~opzLabel(void);
	
	virtual int Draw(ID3DXSprite *sprite, DWORD format);

	void CalcRect();

	void SetC(POINT c, int dirX, int dirY){SetC(c.x, c.y, dirX, dirY); }
	void SetC(int x, int y, int dirX, int dirY);
	void SetRect(RECT rt){_rt = rt; }
	void SetRect(int left, int top, int right, int bottom){_rt.left = left; _rt.top = top; _rt.right = right; _rt.bottom = bottom; }
	void SetStr(LPCWSTR str, ...);
	virtual void SetColor(int r, int g, int b){_color = D3DCOLOR_XRGB(r, g, b); }

	LPCWSTR GetStr() const{return _str; }
	const RECT &GetRect() const{return _rt; }
	const int GetX()const{return _x;}
	const int GetY()const{return _y;}
	
protected:
	int _x;
	int _y;
	int _dirX;
	int _dirY;
	D3DCOLOR _color;

	WCHAR _str[LABEL_BUFFER_LENGTH];
	RECT _rt;
	ID3DXFont *_font;
	int m_len;
};
