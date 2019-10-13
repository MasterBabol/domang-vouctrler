#pragma once

#include "DXMgr.h"

#include <d3dx9.h>
#include <vector>
#include <map>

typedef IDirect3DTexture9* LPTexture;

struct texture_t
{
	static const int TEXTURE_NAME_BUFFER_LENGTH = 64;
	LPTexture _texture;
	WCHAR _name[TEXTURE_NAME_BUFFER_LENGTH];
	int _width;
	int _height;
	float _fWidth;
	float _fHeight;
	texture_t(LPTexture texture, LPCWSTR name,
		int width, int height) : _texture(texture),
		_width(width), _height(height), 
		_fWidth((float)width), _fHeight((float)height)
	{
		wcscpy_s(_name, TEXTURE_NAME_BUFFER_LENGTH, name);
	}

	void SetWidth(int width)
	{
		_width = width;
		_fWidth = static_cast<float>(width);
	}

	void SetHeight(int height)
	{
		_height = height;
		_fHeight = static_cast<float>(height);
	}
};

typedef std::vector<texture_t *> textures_t;

typedef std::map<std::wstring, int *> countersMap_t;

class opzTextureMgr
{
public:
	static void S_Create();
	static void S_Delete();

	static const texture_t *S_Bisr(LPCWSTR str);
	static const texture_t *S_GetTextureFromTexture(LPTexture texture, const RECT &rect, LPCWSTR name);
	static void				S_SortTextures(){_instance->SortTextures(); }

private:
	static opzTextureMgr *_instance;

	LPDirectDevice _pd3dDevice;

	textures_t		_textures;
	countersMap_t	_counters;

	opzTextureMgr(void);
	~opzTextureMgr(void);

	texture_t		*bisr(LPCWSTR str);
	const texture_t *constBisr(LPCWSTR str);
	const texture_t *GetTextureFromTexture(LPTexture texture, const RECT &rect, LPCWSTR name);

	void LoadTexture(LPCWSTR name);
	void SortTextures();
	void LoadPngs();
	void GetInfo(FILE *in, texture_t *texture);
	void LoadTxts();
	void LoadTextures();
};
