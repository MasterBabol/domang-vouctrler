#include "VOUCtrlerLibrary.h"

static int VOUCtrlerAccelModeTable[2][2] = 
{
	800, 600, 300, 200
};

VOUCtrlerQueryInputStatus VOUCtrlerQueryInput(SOCKET ctrlerSocket, VOUCtrlerInputs* out, unsigned int maxWaitableTimeoutMS)
{
	char buf[128]; unsigned char checkSum;
	int ret;
	DWORD nowTime = GetTickCount();

	for(;;)
	{
		if (send(ctrlerSocket, "INFO", 4, 0) != 4)
			return VOUCtrlerQueryInputStatus::ConnectionError;

		int received = 0;
		do
		{
			ret = ioctlsocket(ctrlerSocket, FIONREAD, (DWORD*)&received);
			if (ret < 0)
				return VOUCtrlerQueryInputStatus::ConnectionError;
			if (received >= 8)
				break;
			if (GetTickCount() - nowTime > maxWaitableTimeoutMS)
			{
				if (received > 0)
					recv(ctrlerSocket, buf, received, 0);
				return VOUCtrlerQueryInputStatus::Timeout;
			}
		} while(received < 8);
		recv(ctrlerSocket, buf, 8, 0);

		if (buf[0] == '@')
		{
			if (buf[7] == '#')
			{
				checkSum = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
				if ((checkSum & 0xFF) == 0)
				{
					out->accelX = buf[1];
					out->accelY = buf[2];
					out->accelZ = buf[3];
					out->buttons = buf[4];

					return VOUCtrlerQueryInputStatus::Successful;
				}
			}
		}
	}
}
