#pragma once

#pragma comment(lib, "Winmm.lib")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <time.h>
#include <stdio.h>

#define FRAME_STRING_BUFFER_LENGTH 16

class opzFrameLimiter
{
public:
	static void S_Create(unsigned int FPS);
	static void S_Delete();

	static void S_Begin();
	static void S_End();

	static float S_GetDT(){return _instance->_dt;}
	static const float *S_GetDTPtr(){return &_instance->_dt;}

	static unsigned int S_GetIDT(){return _instance->_idt;}
	static const unsigned int *S_GetIDTPtr(){return &_instance->_idt;}

	static LPCWSTR S_GetString(){return _instance->_frameString; }

private:
	static opzFrameLimiter *_instance;
	
	unsigned int _beginTime;
	unsigned int _periodBeginTime;
	unsigned int _periodFrameCount;
	float _dt;
	unsigned int _idt;
	unsigned int _remainder;
	unsigned int _FPS;
	unsigned int _k;

	WCHAR _frameString[FRAME_STRING_BUFFER_LENGTH];

	opzFrameLimiter(unsigned int FPS);
	~opzFrameLimiter(void);

	void End();
};

