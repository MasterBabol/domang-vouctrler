#pragma once

#include "VOUCtrlerLibrary.h"

enum VOUCtrlerButtons
{
	VOUCButton_LUp =1,
	VOUCButton_LLeft =2,
	VOUCButton_LRight =4,
	VOUCButton_LDown =8,
	VOUCButton_RUp =16,
	VOUCButton_RLeft =32,
	VOUCButton_RRight =64,
	VOUCButton_RDown =128
};

enum VOUCtrlerButtonsState
{
	KEYHOLD, KEYDOWN, KEYUP
};

class VOUCtrlerClass
{
public:
	VOUCtrlerClass(SOCKET btSocket)
	{
		this->btSocket = btSocket;
		memset(histories, 0, sizeof(VOUCtrlerInputs)*16);
		memset(&zeroPoint, 0, sizeof(VOUCtrlerInputs));
		while(VOUCtrlerQueryInput(btSocket, &zeroPoint, 25) == VOUCtrlerQueryInputStatus::Timeout);
	}

	static void InitInstance(SOCKET btSocket) { if (!instance) instance = new VOUCtrlerClass(btSocket); }
	static VOUCtrlerInputs* GetZeroPoint() { return &instance->zeroPoint; }
	static VOUCtrlerInputs* GetInput()
	{
		int accelXSum=0;
		int accelYSum=0;
		int accelZSum=0;
		for(int i=0; i < 15; i++)
		{
			instance->histories[i+1] = instance->histories[i];
			accelXSum += instance->histories[i].accelX;
			accelYSum += instance->histories[i].accelY;
			accelZSum += instance->histories[i].accelZ;
		}
		if (VOUCtrlerQueryInput(instance->btSocket, &instance->histories[0], 25) == VOUCtrlerQueryInputStatus::ConnectionError)
		{
			return 0;
		}
		instance->histories[0].accelX = (instance->histories[0].accelX + accelXSum) / 16;
		instance->histories[0].accelY = (instance->histories[0].accelY + accelYSum) / 16;
		instance->histories[0].accelZ = (instance->histories[0].accelZ + accelZSum) / 16;
		return &instance->histories[0];
	}

	static bool GetButtonState(VOUCtrlerButtons button, VOUCtrlerButtonsState state)
	{
		switch(state)
		{
		case VOUCtrlerButtonsState::KEYHOLD:
			return (button & instance->histories[0].buttons) != 0;
		case VOUCtrlerButtonsState::KEYDOWN:
			return ((button & instance->histories[1].buttons) == 0) && ((button & instance->histories[0].buttons) != 0);
		case VOUCtrlerButtonsState::KEYUP:
			return ((button & instance->histories[1].buttons) != 0) && ((button & instance->histories[0].buttons) == 0);
		}
		return false;
	}

private:
	static VOUCtrlerClass* instance;
	SOCKET btSocket;
	VOUCtrlerInputs histories[16];
	VOUCtrlerInputs zeroPoint;
};
