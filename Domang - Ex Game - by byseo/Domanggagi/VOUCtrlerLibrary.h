#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Winsock2.h>

typedef struct
{
	unsigned char accelX, accelY, accelZ;
	unsigned char buttons;
} VOUCtrlerInputs;

enum VOUCtrlerQueryInputStatus
{
	Successful, ConnectionError, Timeout
};
VOUCtrlerQueryInputStatus VOUCtrlerQueryInput(SOCKET ctrlerSocket, VOUCtrlerInputs* out, unsigned int maxWaitableTimeoutMS);
