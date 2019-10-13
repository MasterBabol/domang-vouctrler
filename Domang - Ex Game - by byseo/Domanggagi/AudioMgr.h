#pragma once

#include "audiere.h"

using namespace audiere;

class opzAudioMgr
{
public:
	static void S_Create();
	static void S_Delete();

	static AudioDevicePtr S_GetDevice(){return _instance->_device; }	

private:
	static opzAudioMgr *_instance;

	AudioDevicePtr _device;

	opzAudioMgr(void);
	~opzAudioMgr(void);
};

