#pragma once

#include "audiere.h"


#define LEAN_AND_MEAN
#include <Windows.h>

using namespace audiere;

class opzSFXMgr
{
public:
	static void S_Create();
	static void S_Delete();

	static void S_PlaySFX(LPCSTR filePath);
	static void S_PlayUnderSFX(LPCSTR filePath);

private:
	static opzSFXMgr	*_instance;

	AudioDevicePtr		_device;
	SoundEffectPtr		_sfx;
	SoundEffectPtr		_underSfx;


	void LoadFiles();

	opzSFXMgr(void);
	~opzSFXMgr(void);
};

