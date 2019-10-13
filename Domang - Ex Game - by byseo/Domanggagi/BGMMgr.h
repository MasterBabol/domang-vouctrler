#pragma once

#include "audiere.h"

using namespace audiere;

enum bgm_t
{
	BGM_MAIN,
	BGM_SCORE,
	BGM_NULL
};

class opzBGMMgr
{
public:
	static void S_Create();
	static void S_Delete();

	static void S_SetBGM(bgm_t bgm);

	static void S_Stoped();

private:
	static opzBGMMgr *_instance;

	AudioDevicePtr _device;
	OutputStreamPtr _stream;

	bgm_t _currentBGM;

	opzBGMMgr(void);
	~opzBGMMgr(void);
};

