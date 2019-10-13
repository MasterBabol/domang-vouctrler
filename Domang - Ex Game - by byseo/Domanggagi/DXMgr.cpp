#include "DXMgr.h"
#include "WndMgr.h"
#include "config.h"

#include <assert.h>

/********************
		public
********************/
void opzDXMgr::S_Create()
{
	if(_instance == NULL)
	{		
		new opzDXMgr();
	}
}

void opzDXMgr::S_Delete()
{
	SafeDelete(_instance);
}

/**********************
		private
**********************/
opzDXMgr *opzDXMgr::_instance = NULL;

opzDXMgr::opzDXMgr()
{
	_instance = this;

	HWND hWnd = opzWndMgr::S_GetHandle();
	
	RECT clientRect;
	clientRect.left = clientRect.top = 0;
	clientRect.right = opzWndMgr::S_GetWidth();
	clientRect.bottom = opzWndMgr::S_GetHeight();

	HRESULT hr;
	
	IDirect3D9* d3d9;
	
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;
	d3d9 -> GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	int vp = 0; 
	
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; 

	
	D3DPRESENT_PARAMETERS PresentParameters; 
	
	PresentParameters.BackBufferWidth = clientRect.right; 
	PresentParameters.BackBufferHeight = clientRect.bottom; 
	PresentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	PresentParameters.BackBufferCount = 1;
	PresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	PresentParameters.MultiSampleQuality = 0; 
	PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	PresentParameters.hDeviceWindow = hWnd; 
#ifdef FULL_SCREEN
	PresentParameters.Windowed = false;
#endif
#ifdef WINDOW_SCREEN
	PresentParameters.Windowed = true;
#endif
	PresentParameters.EnableAutoDepthStencil = true; 
	PresentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	PresentParameters.Flags = 0; 
	PresentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; 
	PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	hr = d3d9 -> CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &PresentParameters, &_pd3dDevice);

	if(FAILED(hr))
	{
		d3d9 -> Release();
		MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
			
		return;
	}
	
	d3d9 -> Release();
}

opzDXMgr::~opzDXMgr(void)
{
	SafeRelease(_pd3dDevice);
}