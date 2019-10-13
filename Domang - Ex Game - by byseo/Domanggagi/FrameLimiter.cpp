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
	timeBeginPeriod(1);	//Sleep�� ��Ȯ���� �����ݴϴ�.
}

opzFrameLimiter::~opzFrameLimiter(void)
{
	timeEndPeriod(1);	//timeBeginPeriod�� ȣ�������� �ݵ�� timeEndPeriod�� ȣ���Ͽ��� �մϴ�.
}

void opzFrameLimiter::End()
{
	unsigned int endTime = opzTimer::S_GetAbsoluteTime();

	unsigned int period = endTime - _periodBeginTime;

	if(period > 1000U)
	{	//1�ʸ��� �����ӷ� ���
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
		//�� ���� Process�� oneFrameTime�ȿ� �۾��� �� ���� ����Դϴ�.
		//�׷��� Sleep�� ���� �ʽ��ϴ�.
		_idt = endTime - _beginTime;
		_dt = _idt / 1000.0f;
	}
	else
	{	//�� ���� �ð��� ���� ����Դϴ�.
		//�� ���� oneFrameTime - endTime + beginTime�Դϴ�.
		Sleep(oneFrameTime - endTime + _beginTime - _k);
		_k = opzTimer::S_GetAbsoluteTime() - endTime -
			(oneFrameTime - endTime + _beginTime) + _k;
		_remainder = (1000 + _remainder) % _FPS;

		_idt = opzTimer::S_GetAbsoluteTime() - _beginTime;
		_dt = _idt / 1000.0f;
	}


	++_periodFrameCount;
}