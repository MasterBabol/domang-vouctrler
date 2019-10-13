#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Winsock2.h>
#include <Ws2bth.h>
#include <BluetoothAPIs.h>
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "irprops.lib")

struct _HostBluetoothDeviceFindHandle
{
	BLUETOOTH_RADIO_INFO radioInfo;
	BLUETOOTH_FIND_RADIO_PARAMS findRadioParams;
	HANDLE radioHandle;
	HBLUETOOTH_RADIO_FIND radioFindHandle;
};
typedef _HostBluetoothDeviceFindHandle* HostBluetoothDeviceFindHandle;

struct _RemoteBluetoothDeviceFindHandle
{
	BLUETOOTH_DEVICE_INFO deviceInfo;
	HostBluetoothDeviceFindHandle hostDeviceFindHandle;
	BLUETOOTH_DEVICE_SEARCH_PARAMS deviceSearchParams;
	HBLUETOOTH_DEVICE_FIND deviceFindHandle;
	int timeOutMultiplier;
};
typedef _RemoteBluetoothDeviceFindHandle* RemoteBluetoothDeviceFindHandle;

HostBluetoothDeviceFindHandle FindHostDeviceBegin();
BOOL FindHostDeviceNext(HostBluetoothDeviceFindHandle handle);

RemoteBluetoothDeviceFindHandle FindRemoteDeviceBegin(HostBluetoothDeviceFindHandle hostDeviceFindHandle, int timeOutMultiplier);
BOOL FindRemoteDeviceNext(RemoteBluetoothDeviceFindHandle handle);
