#include "BluetoothDeviceSearch.h"
#include <stdlib.h>

HostBluetoothDeviceFindHandle FindHostDeviceBegin()
{
	HostBluetoothDeviceFindHandle newHandle = (HostBluetoothDeviceFindHandle)malloc(sizeof(_HostBluetoothDeviceFindHandle));
	memset(newHandle, 0, sizeof(_HostBluetoothDeviceFindHandle));

	newHandle->findRadioParams.dwSize = sizeof(BLUETOOTH_FIND_RADIO_PARAMS);
	newHandle->radioInfo.dwSize = sizeof(BLUETOOTH_RADIO_INFO);

	newHandle->radioFindHandle = BluetoothFindFirstRadio(&newHandle->findRadioParams, &newHandle->radioHandle);
	if (newHandle->radioFindHandle == NULL)
		goto cleanup;

	if (BluetoothGetRadioInfo(newHandle->radioHandle, &newHandle->radioInfo) != ERROR_SUCCESS)
		goto cleanup;

	return newHandle;
cleanup:
	free(newHandle);
	return NULL;
}

BOOL FindHostDeviceNext(HostBluetoothDeviceFindHandle handle)
{
	if (handle == NULL)
		return FALSE;
	if (BluetoothFindNextRadio(handle->radioFindHandle, &handle->radioHandle))
	{
		if (BluetoothGetRadioInfo(handle->radioHandle, &handle->radioInfo) != ERROR_SUCCESS)
			goto cleanup;
		return TRUE;
	}
cleanup:
	BluetoothFindRadioClose(handle->radioFindHandle);
	free(handle);
	return FALSE;
}

RemoteBluetoothDeviceFindHandle FindRemoteDeviceBegin(HostBluetoothDeviceFindHandle hostDeviceFindHandle, int timeOutMultiplier)
{
	RemoteBluetoothDeviceFindHandle newHandle = (RemoteBluetoothDeviceFindHandle)malloc(sizeof(_RemoteBluetoothDeviceFindHandle));
	memset(newHandle, 0, sizeof(_RemoteBluetoothDeviceFindHandle));
	newHandle->hostDeviceFindHandle = hostDeviceFindHandle;
	newHandle->deviceSearchParams.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
	newHandle->deviceSearchParams.fReturnUnknown = TRUE;
	newHandle->deviceSearchParams.fIssueInquiry = TRUE;
	newHandle->deviceSearchParams.fReturnConnected = TRUE;
	newHandle->deviceSearchParams.fReturnAuthenticated = TRUE;
	newHandle->timeOutMultiplier = timeOutMultiplier;
	newHandle->deviceSearchParams.cTimeoutMultiplier = timeOutMultiplier;
	newHandle->deviceInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);

	newHandle->deviceFindHandle = BluetoothFindFirstDevice(&newHandle->deviceSearchParams, &newHandle->deviceInfo);
	if (newHandle->deviceFindHandle == NULL)
		goto cleanup;

	return newHandle;

cleanup:
	free(newHandle);
	return NULL;
}

BOOL FindRemoteDeviceNext(RemoteBluetoothDeviceFindHandle handle)
{
	if (handle == NULL)
		return FALSE;
	if (BluetoothFindNextDevice(handle->deviceFindHandle, &handle->deviceInfo))
	{
		return TRUE;
	}
	BluetoothFindDeviceClose(handle->deviceFindHandle);
	free(handle);
	return FALSE;
}
