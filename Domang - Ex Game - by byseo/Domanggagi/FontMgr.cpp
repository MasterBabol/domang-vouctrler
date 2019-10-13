#include "FontMgr.h"

#include <assert.h>

/***********************
		public
***********************/
void opzFontMgr::S_Create()
{
	if(_instance == NULL)
	{
		LPDirectDevice _pd3dDevice = opzDXMgr::S_GetDevice();
		assert(_pd3dDevice != NULL);
		new opzFontMgr(_pd3dDevice);
	}
}

void opzFontMgr::S_Delete()
{
	SafeDelete(_instance);
}

ID3DXFont *opzFontMgr::S_GetNewFont(LPCWSTR strName, int nSize)
{
	assert(_instance != NULL);
	return _instance->InGetNewFont(strName, nSize);
}

/**********************
		private
**********************/
opzFontMgr *opzFontMgr::_instance = NULL;

opzFontMgr::opzFontMgr(LPDirectDevice pd3dDevice) :
	_pd3dDevice(pd3dDevice)
{
	_instance = this;
}

opzFontMgr::~opzFontMgr(void)
{
	ID3DXFont *font;
	std::vector<ID3DXFont *>::iterator iter, it_end;
	for(iter = _fonts.begin(), it_end = _fonts.end();
		iter != it_end; iter++)
	{
		font = *iter;
		font->Release();
	}
}

ID3DXFont *opzFontMgr::InGetNewFont(LPCWSTR strName, int nSize)
{
	ID3DXFont *font = NULL;

	D3DXFONT_DESC fontDesc;
	ZeroMemory(&fontDesc, sizeof(fontDesc)); //폰트의 구조체를 초기화시킴
	fontDesc.Height = 2 * nSize; //폰트의 높이(대체로 너비의 2배쯤이 적당함)
	fontDesc.Width = nSize; //폰트의 너비
	fontDesc.Weight = 600; //폰트의 굵기 (0(얉음)~ 1000(굵음)의 값을 정함)
	fontDesc.Italic = false; //이탤릭?
	fontDesc.CharSet = DEFAULT_CHARSET; //캐릭터 셋임.
	wcscpy_s(fontDesc.FaceName, strName); // 글꼴 스타일

	D3DXCreateFontIndirect(_pd3dDevice, &fontDesc, &font); //폰트를 만든다.

	_fonts.push_back(font);
	
	return font;
}