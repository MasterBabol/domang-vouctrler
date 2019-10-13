#pragma once

#include "DXMgr.h"
#include "Macros.h"

#include <d3dx9.h>
#include <vector>

class opzFontMgr
{
public:
	static void S_Create();
	static void S_Delete();

	static ID3DXFont *S_GetNewFont(LPCWSTR strName, int nSize);

private:
	static opzFontMgr *_instance;

	LPDirectDevice _pd3dDevice;

	std::vector<ID3DXFont *> _fonts;

	opzFontMgr(LPDirectDevice pd3dDevice);
	~opzFontMgr(void);

	ID3DXFont *InGetNewFont(LPCWSTR strName, int nSize);
};

