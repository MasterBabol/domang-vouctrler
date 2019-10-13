#include "TextureMgr.h"

#include "MiniFuncs.h"
#include "Macros.h"
#include "WndMgr.h"

#include <errno.h>
#include <algorithm>
#include <assert.h>
#include <io.h>
#include <hash_map>
#include <string>

/**********************
		public
**********************/
void opzTextureMgr::S_Create()
{
	if(_instance == NULL)
	{
		new opzTextureMgr();
	}
}

void opzTextureMgr::S_Delete()
{
	SafeDelete(_instance);
}

const texture_t *opzTextureMgr::S_Bisr(LPCWSTR str)//n은 문자열의 갯수
{
	return _instance->bisr(str);
}

const texture_t *opzTextureMgr::S_GetTextureFromTexture(
	LPTexture texture, const RECT &rect, LPCWSTR name)
{
	return _instance->GetTextureFromTexture(texture, rect, name);
}

/******************
		private
******************/
opzTextureMgr *opzTextureMgr::_instance = NULL;

opzTextureMgr::opzTextureMgr(void)
{
	_instance = this;

	_pd3dDevice = opzDXMgr::S_GetDevice();

	LoadTextures();
}


opzTextureMgr::~opzTextureMgr(void)
{
	textures_t::iterator iter, it_end;
	for(iter = _textures.begin(), it_end = _textures.end();
		iter != it_end; iter++)
	{
		delete *iter;
	}
}

void opzTextureMgr::LoadTexture(LPCWSTR name)
{
	static WCHAR strFileName[2 * texture_t::TEXTURE_NAME_BUFFER_LENGTH];
	LPWSTR temp = strFileName;

	temp = MyStrcpy(temp, TEXT("data\\graphics\\"));
	temp = MyStrcat(temp, name);

	D3DXIMAGE_INFO ImageInfo;
	D3DXGetImageInfoFromFile(strFileName, &ImageInfo);
	
	IDirect3DTexture9* crtTexture;
    D3DCOLOR colorKey = D3DCOLOR_XRGB(255, 0, 255);
	D3DXCreateTextureFromFileEx(_pd3dDevice, strFileName, ImageInfo.Width, ImageInfo.Height, 1, D3DUSAGE_AUTOGENMIPMAP,
		ImageInfo.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, colorKey, NULL, NULL, &crtTexture);

	int width, height;
	width = (int)ImageInfo.Width;
	height = (int)ImageInfo.Height;

	static WCHAR strName[texture_t::TEXTURE_NAME_BUFFER_LENGTH];
	wcsncpy_s(strName, name, wcslen(name) - 4);
	_textures.push_back(new texture_t(crtTexture, strName, width, height));
}

bool TextureSort(const texture_t *a, const texture_t *b)
{
	if(wcscmp(a->_name, b->_name) < 0)return true; return false;
}

void opzTextureMgr::SortTextures()
{
	std::sort(_textures.begin(), _textures.end(), TextureSort);
}

void opzTextureMgr::LoadPngs()
{
	_finddata_t fd;
    long handle=_findfirst("data\\graphics\\*.png", &fd);
	if(handle == -1)
	{
#ifdef _DEBUG
		MessageBox(opzWndMgr::S_GetHandle(), L"그래픽 파일 없다!", L"경고", 0);
#endif
		return;
	}
	int result = 1;
	int count = 0;
	WCHAR buffer[texture_t::TEXTURE_NAME_BUFFER_LENGTH + 4];

	while(result != -1)
	{
		swprintf_s(buffer, _countof(buffer), L"%hs", fd.name);
		LoadTexture(buffer);
		result = _findnext(handle, &fd);
	}
	_findclose(handle);

	SortTextures();
}

void opzTextureMgr::GetInfo(FILE *in, texture_t *texture)
{
	WCHAR cmd[CMD_BUFFER_LENGTH];
	
	while(fwscanf_s(in, L"%s", cmd, CMD_BUFFER_LENGTH) != EOF)
	{
		int iToken;
		STR_SWITCH_BEGIN(cmd)
		{
		CASE(L"width")
		fwscanf_s(in, L"%d", &iToken);
		texture->SetWidth(iToken);
			break;

		CASE(L"height")
		fwscanf_s(in, L"%d", &iToken);
		texture->SetHeight(iToken);
		break;
		}
		STR_SWITCH_END()

	}
}

void opzTextureMgr::LoadTxts()
{
	_finddata_t fd;
	long handle = _findfirst("data\\graphics\\*.txt", &fd);
	if(handle == -1)
		return;

	int result = 1;
	int count = 0;
	FILE *in;
	WCHAR filePath[1024];
	LPWSTR pFilePath;
	pFilePath = MyStrcpy(filePath, L"data\\graphics\\");
	WCHAR buffer[texture_t::TEXTURE_NAME_BUFFER_LENGTH + 4];
	
	while(result != -1)
	{
		swprintf_s(buffer, _countof(buffer), L"%hs", fd.name);
		*pFilePath = '\0';
		MyStrcat(pFilePath, buffer);
		if(EINVAL != _wfopen_s(&in, filePath, L"r"))
		{
			buffer[wcslen(buffer) - 4] = '\0';
			texture_t *texture = bisr(buffer);
			if(wcscmp(texture->_name, buffer))
				continue;
			
			GetInfo(in, texture);

			fclose(in);
		}
		result = _findnext(handle, &fd);
	}
	
}

void opzTextureMgr::LoadTextures()
{
	LoadPngs();

	LoadTxts();
}

texture_t *opzTextureMgr::bisr(LPCWSTR str)
{
	if(str == NULL)return (texture_t *)NULL;
	textures_t::iterator min, max, mid;
	min = _textures.begin();
	max = _textures.end();

	int condition;

	while(1)
	{
		//mid = (min + max) / 2을 stl식으로 표현
		mid = min;
		std::advance(mid, std::distance(min, max) / 2);

		condition = wcscmp((*mid)->_name, str);
		if(condition < 0)
		{
			min = ++mid;
		}
		else if(condition > 0)
		{
			max = --mid;	
		}else
		{
			break;
		}

		if(std::distance(_textures.begin(), min) >= std::distance(_textures.begin(), max)) return *mid;
	}

	return *mid;
}

const texture_t *opzTextureMgr::constBisr(LPCWSTR str)
{
	return bisr(str);
}

const texture_t *opzTextureMgr::GetTextureFromTexture(LPTexture texture, const RECT &rect, LPCWSTR name)
{
	HRESULT hr;
	LPTexture  pTempTexture = NULL ;

    hr = D3DXCreateTexture(opzDXMgr::S_GetDevice(), 
		rect.right - rect.left,
		rect.bottom - rect.top,
		1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTempTexture) ;
 
    if ( SUCCEEDED(hr) )
    {
        LPDIRECT3DSURFACE9  pTargetSurf = NULL ;
        pTempTexture->GetSurfaceLevel(0, &pTargetSurf);
 
        LPDIRECT3DSURFACE9  pSrcSurf = NULL ;
		texture->GetSurfaceLevel(0, &pSrcSurf);        
 
		RECT destRect = {0, 0, rect.right - rect.left, rect.bottom - rect.top};
        D3DXLoadSurfaceFromSurface(pTargetSurf, NULL, &destRect, pSrcSurf, NULL, &rect, D3DX_FILTER_BOX, 0);
             
        pTargetSurf->Release();
        pSrcSurf->Release();
    }

	texture_t *newOne = new texture_t(pTempTexture, name, rect.right - rect.left, rect.bottom - rect.top);
	_textures.push_back(newOne);
	return newOne;
}