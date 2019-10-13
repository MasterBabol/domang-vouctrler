#include "SFXMgr.h"
#include "Macros.h"
#include "AudioMgr.h"

/*************************
			public
*************************/
void opzSFXMgr::S_Create()
{
	if(_instance == NULL)
	{
		new opzSFXMgr();
	}
}

void opzSFXMgr::S_Delete()
{
	SafeDelete(_instance);
}

void opzSFXMgr::S_PlaySFX(LPCSTR filePath)
{
	_instance->_sfx = OpenSoundEffect(_instance->_device, filePath, SINGLE);
	_instance->_sfx->play();
}

void opzSFXMgr::S_PlayUnderSFX(LPCSTR filePath)
{
	_instance->_underSfx = OpenSoundEffect(_instance->_device, filePath, SINGLE);
	_instance->_underSfx->play();
}

/*************************
			private
*************************/
opzSFXMgr *opzSFXMgr::_instance = NULL;

void opzSFXMgr::LoadFiles()
{
	
}

opzSFXMgr::opzSFXMgr(void) :
	_device(NULL)
{
	_instance = this;

	_device = opzAudioMgr::S_GetDevice();
}

opzSFXMgr::~opzSFXMgr(void)
{
}
