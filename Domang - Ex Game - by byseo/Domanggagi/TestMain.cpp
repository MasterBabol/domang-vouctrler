#include <stdio.h>
#include <stdlib.h>

//for bluetooth
#include "BluetoothDeviceSearch.h"
#include "BluetoothSocket.h"
#include "VOUCtrlerLibrary.h"
#include "VOUCtrlerClass.h"

//
#include "Timer.h"
#include "FrameLimiter.h"
#include "WndMgr.h"
#include "AudioMgr.h"
#include "DXMgr.h"
#include "FontMgr.h"
#include "TextureMgr.h"
#include "DInput.h"
#include "ScoreMgr.h"
#include "BasicForm.h"
#include "PlayForm.h"
#include "ScoreForm.h"

int ConnectToVOUCtrlerDevice()
{
	WSADATA wd; WSAStartup(0x0202, &wd);

	HostBluetoothDeviceFindHandle hostHandle = FindHostDeviceBegin();
	if (hostHandle)
	do
	{
		RemoteBluetoothDeviceFindHandle remoteHandle = FindRemoteDeviceBegin(hostHandle, 5);
		if (remoteHandle)
		do
		{
			if (wcscmp(L"VOUCtrler", remoteHandle->deviceInfo.szName) == 0)
			{
				int sock = BluetoothSocketConnectTo(&remoteHandle->deviceInfo.Address);
				return sock;
			}
		} while(FindRemoteDeviceNext(remoteHandle));
	} while(FindHostDeviceNext(hostHandle));
}

int APIENTRY WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	/**************************
				초기화
	**************************/
	opzTimer::S_Create();
	
	//
	opzFrameLimiter::S_Create(60U);
	
	//
	opzWndMgr::S_Create(hInstance);

	//
	opzAudioMgr::S_Create();

	//
	opzDXMgr::S_Create();

	//
	opzFontMgr::S_Create();

	//
	opzTextureMgr::S_Create();
	
	SOCKET btSock = ConnectToVOUCtrlerDevice();
	VOUCtrlerClass::InitInstance(btSock);
	//
	opzDInput::S_Create();

	opzScoreMgr::S_Create();

	opzBasicForm *form[FORM_NUMBER];
	form[FORM_PLAY] = new opzPlayForm();
	form[FORM_SCORE] = new opzScoreForm();

	opzBasicForm *currentForm = form[FORM_PLAY];
	
	for(;;)
	{
		currentForm->OnSet();
		for(;;)
		{
			opzFrameLimiter::S_Begin();
			
			if(opzWndMgr::S_WinMsgProc() == false)
			{
				currentForm->SetNextForm(FORM_NULL);
				break;	//윈도우 메시지 처리
			}
			currentForm->OnFrameMove();

			if(currentForm->CheckEnd() == true)break;

			currentForm->OnFrameDraw();

			opzFrameLimiter::S_End();

		}
		{
			int nextForm = currentForm->GetNextForm();
			if(nextForm == FORM_NULL)
				break;
			else
			{
				currentForm = form[nextForm];
				currentForm->OnReset();
			}
		}
	}

	opzScoreMgr::S_Delete();
	opzDInput::S_Delete();
	opzTextureMgr::S_Delete();
	opzFontMgr::S_Delete();
	opzDXMgr::S_Delete();
	opzAudioMgr::S_Delete();
	opzWndMgr::S_Delete();
	opzFrameLimiter::S_Delete();
	opzTimer::S_Delete();

	_CrtDumpMemoryLeaks();

	return 0;
}
/*
int main()
{
	WSADATA wd; WSAStartup(0x0202, &wd);

	HostBluetoothDeviceFindHandle hostHandle = FindHostDeviceBegin();
	if (hostHandle)
	do
	{
		wprintf(L"Host Bluetooth Device\nInstance Name: %s\r\n", hostHandle->radioInfo.szName);
        wprintf(L"Address: %02X:%02X:%02X:%02X:%02X:%02X\r\n", hostHandle->radioInfo.address.rgBytes[5],
                    hostHandle->radioInfo.address.rgBytes[4], hostHandle->radioInfo.address.rgBytes[3], hostHandle->radioInfo.address.rgBytes[2],
                        hostHandle->radioInfo.address.rgBytes[1], hostHandle->radioInfo.address.rgBytes[0]);
        wprintf(L"Class: 0x%08x\r\n", hostHandle->radioInfo.ulClassofDevice);
        wprintf(L"Manufacturer: 0x%04x\r\n", hostHandle->radioInfo.manufacturer);

		RemoteBluetoothDeviceFindHandle remoteHandle = FindRemoteDeviceBegin(hostHandle, 5);
		if (remoteHandle)
		do
		{
			wprintf(L"\tRemote Bluetooth Device\n\tInstance Name: %s\r\n", remoteHandle->deviceInfo.szName);
			wprintf(L"\tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", remoteHandle->deviceInfo.Address.rgBytes[5],
						remoteHandle->deviceInfo.Address.rgBytes[4], remoteHandle->deviceInfo.Address.rgBytes[3], remoteHandle->deviceInfo.Address.rgBytes[2],
						remoteHandle->deviceInfo.Address.rgBytes[1], remoteHandle->deviceInfo.Address.rgBytes[0]);
			wprintf(L"\tClass: 0x%08x\r\n", remoteHandle->deviceInfo.ulClassofDevice);
			wprintf(L"\tConnected: %s\r\n", remoteHandle->deviceInfo.fConnected ? L"true" : L"false");
			wprintf(L"\tAuthenticated: %s\r\n", remoteHandle->deviceInfo.fAuthenticated ? L"true" : L"false");
			wprintf(L"\tRemembered: %s\r\n", remoteHandle->deviceInfo.fRemembered ? L"true" : L"false");
			
			if (wcscmp(L"VOUCtrler", remoteHandle->deviceInfo.szName) == 0)
			{
				printf("\nFound VOUCtrler ! \n\n");
				int sock = BluetoothSocketConnectTo(&remoteHandle->deviceInfo.Address);
				printf("socket() returned %d.\n", sock);
				
				char buf[256];
				int result = sock;
				HANDLE stdouthandle = GetStdHandle(STD_OUTPUT_HANDLE);
				COORD pos = {0,16};
				VOUCtrlerInputs input;
				VOUCtrlerQueryInputStatus status;
				int timeouts = 0;
				DWORD nowTime = GetTickCount();
				
				send(sock, "ACCMB", 5, 0);
				for(;result != -1;)
				{
					status = VOUCtrlerQueryInput(sock, &input, 0);
					if (status == VOUCtrlerQueryInputStatus::Timeout)
						timeouts++;
					else if (status == VOUCtrlerQueryInputStatus::ConnectionError)
					{
						printf("\nConnection lost.\n");
						break;
					}
					else
					{
						SetConsoleCursorPosition(stdouthandle, pos);
						printf("X v: %3d    \nY v: %3d    \nZ v: %3d    \nButtons: %08s\nTimeouts: %5d\nTotal Running Time: %5d", 
							input.accelX, input.accelY, input.accelZ, itoa(input.buttons, buf+128, 2), timeouts, GetTickCount() - nowTime);
					}
				}
				return 0;
			}
		} while(FindRemoteDeviceNext(remoteHandle));
	} while(FindHostDeviceNext(hostHandle));
	return 0;
}
*/