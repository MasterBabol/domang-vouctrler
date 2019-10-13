#pragma once

#include "DXMgr.h"
#include "Car.h"
#include "DrawObject.h"
#include <list>

struct boom_t
{
	opz2DVector		_pos;
	unsigned int	_beginTime;

	boom_t(const opz2DVector &pos, unsigned int beginTime) :
		_pos(pos), _beginTime(beginTime)
	{
	}
};

class opzCarMgr
{
public:
	static void S_Create();
	static void S_Delete();

	static void S_Reset(){_instance->Reset(); }

	static void S_FrameMove(){_instance->FrameMove(); }
	static void S_Draw(){_instance->Draw(); }
	
	static std::list<opzCar *>::const_iterator S_GetListBegin(){return _instance->_list.begin(); }
	static std::list<opzCar *>::const_iterator S_GetListEnd(){return _instance->_list.end(); }

private:
	static opzCarMgr *_instance;

	ID3DXSprite*	_sprite;
	opzDrawObject	*_unitDO;
	opzDrawObject	*_unitBoomDO;

	std::list<opzCar *>	_list;
	std::list<boom_t>	_boomList;

	unsigned int _startTime;
	unsigned int _lastNewCarTime;

	opzCarMgr(void);
	void ClearList();
	~opzCarMgr(void);

	void Reset();

	void NewCar();
	void CarCarColProcess();
	void FrameMove();
	void Draw();
};

