#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Winsock2.h>
#include <Ws2bth.h>
#include <BluetoothAPIs.h>
#include <bthdef.h>
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "irprops.lib")

SOCKET BluetoothSocketConnectTo(BLUETOOTH_ADDRESS_STRUCT* remoteBluetoothDeviceAddress);

