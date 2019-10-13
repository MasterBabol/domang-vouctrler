#include "FrameLimiter.h"
#include "Timer.h"
#include "Macros.h"
#include "WndMgr.h"

#include <assert.h>

/***************************
			public
***************************/
void opzFrameLimiter::S_Create(unsigned int FPS)
{
	assert(_instance == NULL);
	if(_instance == NULL)
		new opzFrameLimiter(FPS);
}

void opzFrameLimiter::S_Delete()
{
	assert(_instance != NULL);
	SafeDelete(_instance);
}

void opzFrameLimiter::S_Begin()
{
	_instance->_beginTime = opzTimer::S_GetAbsoluteTime();
}

void opzFrameLimiter::S_End()
{
	_instance->End();
}

/***************************
			private
***************************/
opzFrameLimiter *opzFrameLimiter::_instance = NULL;

opzFrameLimiter::opzFrameLimiter(unsigned int FPS)
	: _beginTime(0U), _periodBeginTime(0U), _periodFrameCount(0U),
	_dt(0.0f), _idt(0U),
	_remainder(0U), _FPS(FPS), _k(0U)
{
	_instance = this;
	timeBeginPeriod(1);	//Sleep의 정확도를 높여줍니다.
}

opzFrameLimiter::~opzFrameLimiter(void)
{
	timeEndPeriod(1);	//timeBeginPeriod를 호출했으면 반드시 timeEndPeriod를 호출하여야 합니다.
}

void opzFrameLimiter::End()
{
	unsigned int endTime = opzTimer::S_GetAbsoluteTime();

	unsigned int period = endTime - _periodBeginTime;

	if(period > 1000U)
	{	//1초마다 프레임률 계산
		switch(_periodFrameCount)
		{
		case 0U:
			break;
		default:
			swprintf_s(_frameString, L"FPS: %2f", 1000.0f / period * _periodFrameCount);	
			_periodBeginTime = endTime;
			_periodFrameCount = 0U;
			break;
		}
	}
	
	unsigned int oneFrameTime = (1000 + _remainder) / _FPS;
	
	if(endTime - _beginTime + _k > oneFrameTime)
	{
		//이 경우는 Process가 oneFrameTime안에 작업을 못 끝낸 경우입니다.
		//그래서 Sleep을 하지 않습니다.
		_idt = endTime - _beginTime;
		_dt = _idt / 1000.0f;
	}
	else
	{	//이 경우는 시간이 남은 경우입니다.
		//그 값은 oneFrameTime - endTime + beginTime입니다.
		Sleep(oneFrameTime - endTime + _beginTime - _k);
		_k = opzTimer::S_GetAbsoluteTime() - endTime -
			(oneFrameTime - endTime + _beginTime) + _k;
		_remainder = (1000 + _remainder) % _FPS;

		_idt = opzTimer::S_GetAbsoluteTime() - _beginTime;
		_dt = _idt / 1000.0f;
	}


	++_periodFrameCount;
}