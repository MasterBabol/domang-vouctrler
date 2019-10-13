#include "AudioMgr.h"
#include "Macros.h"
#include "BGMMgr.h"
#include "SFXMgr.h"

/********************
		public
********************/
void opzAudioMgr::S_Create()
{
	if(_instance == NULL)
	{
		new opzAudioMgr();
	}
}

void opzAudioMgr::S_Delete()
{
	SafeDelete(_instance);
}

/********************
		private
********************/
opzAudioMgr *opzAudioMgr::_instance = NULL;

opzAudioMgr::opzAudioMgr(void) :
	_device()
{
	_instance = this;

	_device = OpenDevice();
	
	opzBGMMgr::S_Create();
	opzSFXMgr::S_Create();
}


opzAudioMgr::~opzAudioMgr(void)
{
	opzSFXMgr::S_Delete();
	opzBGMMgr::S_Delete();
}
