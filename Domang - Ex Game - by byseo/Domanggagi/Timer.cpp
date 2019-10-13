#include "Timer.h"

#include <stdio.h>
#include <windows.h>
#include <assert.h>

/*********************
		public
*********************/
void opzTimer::S_Create()
{
	if(_instance == NULL)
	{
		new opzTimer();
	}
}

void opzTimer::S_Delete()
{
	if(_instance != NULL)
	{
		delete _instance;
	}
}

unsigned int opzTimer::S_GetAbsoluteTime()
{
	return _instance->GetAbsoluteTime();
}

/*********************
		private
*********************/
opzTimer *opzTimer::_instance = NULL;

opzTimer::opzTimer()
: _timerFreq(), _counterAtStart()
{
	_instance = this;

	QueryPerformanceFrequency(&_timerFreq); 
	QueryPerformanceCounter(&_counterAtStart);

	TIMECAPS ptc;
	UINT cbtc = 8;
	MMRESULT result = timeGetDevCaps(&ptc, cbtc);

	assert(result == TIMERR_NOERROR);
}

opzTimer::~opzTimer()
{
}

unsigned int opzTimer::GetAbsoluteTime()
{
	if(_timerFreq.QuadPart == 0)
	{
		return 0U;
	}
	else
	{
		LARGE_INTEGER c;
		QueryPerformanceCounter(&c);

		return static_cast<unsigned int>(
			(c.QuadPart - _counterAtStart.QuadPart) * 1000 / _timerFreq.QuadPart
			);
	}
}