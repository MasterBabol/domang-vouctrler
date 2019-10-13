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
	ZeroMemory(&fontDesc, sizeof(fontDesc)); //��Ʈ�� ����ü�� �ʱ�ȭ��Ŵ
	fontDesc.Height = 2 * nSize; //��Ʈ�� ����(��ü�� �ʺ��� 2������ ������)
	fontDesc.Width = nSize; //��Ʈ�� �ʺ�
	fontDesc.Weight = 600; //��Ʈ�� ���� (0(����)~ 1000(����)�� ���� ����)
	fontDesc.Italic = false; //���Ÿ�?
	fontDesc.CharSet = DEFAULT_CHARSET; //ĳ���� ����.
	wcscpy_s(fontDesc.FaceName, strName); // �۲� ��Ÿ��

	D3DXCreateFontIndirect(_pd3dDevice, &fontDesc, &font); //��Ʈ�� �����.

	_fonts.push_back(font);
	
	return font;
}