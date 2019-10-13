#pragma once


#include "Macros.h"

//DirectX¿ë
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <d3dx9.h>

typedef IDirect3DDevice9* LPDirectDevice;

class opzDXMgr
{
public:
	static void S_Create();
	static void S_Delete();

	static LPDirectDevice S_GetDevice(){return _instance->_pd3dDevice; }

private:
	static opzDXMgr *_instance;

	LPDirectDevice _pd3dDevice;

	opzDXMgr(void);
	~opzDXMgr(void);
};

