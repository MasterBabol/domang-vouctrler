#include "BluetoothSocket.h"

SOCKET BluetoothSocketConnectTo(BLUETOOTH_ADDRESS_STRUCT* remoteBluetoothDeviceAddress)
{
	SOCKET connSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (connSocket == INVALID_SOCKET)
		return INVALID_SOCKET;

	SOCKADDR_BTH bthAddr = {0};
	bthAddr.btAddr = remoteBluetoothDeviceAddress->ullLong;
	bthAddr.port = 1;
	bthAddr.addressFamily = AF_BTH;

	if (connect(connSocket, (sockaddr*)&bthAddr, sizeof(bthAddr)) == SOCKET_ERROR)
	{
		closesocket(connSocket);
		return INVALID_SOCKET;
	}

	return connSocket;
}
