#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mmsystem.h>

class opzTimer
{
public:
	static void S_Create();
	static void S_Delete();

	static unsigned int S_GetAbsoluteTime();	//opzTimer�� ������� �ĺ��� ���� �ð��� ��ȯ�մϴ�.

private:
	static opzTimer *_instance;

	LARGE_INTEGER _timerFreq;
	LARGE_INTEGER _counterAtStart;

	opzTimer();
	~opzTimer();

	unsigned int GetAbsoluteTime();
};