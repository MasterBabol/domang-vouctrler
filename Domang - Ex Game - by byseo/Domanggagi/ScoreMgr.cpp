#include "ScoreMgr.h"
#include "Macros.h"

#define NULL 0

/*********************
		public
*********************/
void opzScoreMgr::S_Create()
{
	if(_instance == NULL)
	{
		new opzScoreMgr();
	}
}

void opzScoreMgr::S_Delete()
{
	SafeDelete(_instance);
}

/*********************
		private
*********************/
opzScoreMgr *opzScoreMgr::_instance = NULL;

opzScoreMgr::opzScoreMgr(void) :
	_score(0)
{
	_instance = this;
}


opzScoreMgr::~opzScoreMgr(void)
{
}
