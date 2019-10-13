#include "BGMMgr.h"
#include "Macros.h"
#include "AudioMgr.h"
#include <assert.h>

class opzStopCallback : public StopCallback
{
public:
	opzStopCallback() : StopCallback(),
		_refCount(0)
	{
	}

	ADR_METHOD(void) ref()
	{
		_refCount++;
	}

	ADR_METHOD(void) unref()
	{
		_refCount--;
		if(_refCount == 0)delete(this);
	}

	ADR_METHOD(void) call(Event* event)
	{
		opzBGMMgr::S_Stoped();
	}

	ADR_METHOD(void) streamStopped(StopEvent* event)
	{
		opzBGMMgr::S_Stoped();
	}

private:
	int _refCount;
};


/********************
		public
********************/
void opzBGMMgr::S_Create()
{
	if(_instance == NULL)
	{
		new opzBGMMgr();
	}
}

void opzBGMMgr::S_Delete()
{
	SafeDelete(_instance);
}

void opzBGMMgr::S_SetBGM(bgm_t bgm)
{
	assert(_instance != NULL);

	_instance->_currentBGM = bgm;

	switch(bgm)
	{
	case BGM_MAIN:
		_instance->_stream = OpenSound(_instance->_device, "data\\bgm\\cheeta.mp3", true);
		break;

	case BGM_SCORE:
		_instance->_stream = OpenSound(_instance->_device, "data\\bgm\\score.wav", true);
		break;
	}

	_instance->_stream->play();
}

void opzBGMMgr::S_Stoped()
{
	assert(_instance != NULL);
	switch(_instance->_currentBGM)
	{
	case BGM_MAIN:
		_instance->_stream->play();	//그냥 무한 반복 ㄷㄷ
		break;
	}
}

/********************
		private
********************/
opzBGMMgr *opzBGMMgr::_instance = NULL;

opzBGMMgr::opzBGMMgr(void)
	: _currentBGM(BGM_NULL)
{
	_instance = this;

	_device = OpenDevice();
	opzStopCallback *scb = new opzStopCallback;
	_device->registerCallback(scb);
}

opzBGMMgr::~opzBGMMgr(void)
{
}