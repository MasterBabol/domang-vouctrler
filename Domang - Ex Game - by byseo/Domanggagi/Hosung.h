#pragma once

#include "2DVector.h"
#include "DXMgr.h"
#include "DrawObject.h"

enum heightState_t
{
	HS_GROUND,
	HS_AIR,
	HS_ABOVE_GROUND
};

enum speedState_t
{
	NORMAL_SPEED,
	FAST_SPEED
};

class opzHosung
{
public:
	static void S_Create();
	static void S_Delete();

	static void S_Reset(){_instance->Reset(); }

	static void S_FrameMove(){_instance->FrameMove(); }
	static void S_FrameDraw(){_instance->FrameDraw();}

	static void S_InputProcess(){_instance->InputProcess(); }

	static bool S_CheckDead() {return _instance->_dead;}
	static const opz2DVector &S_GetPos(){return _instance->_pos; }
	static const float		S_GetDir(){return _instance->_dir; }

private:
	static opzHosung *_instance;

	const float		*_dt;

	ID3DXSprite*	_sprite;
	opzDrawObject	*_do;
	opzDrawObject	*_healthBlocks[10];
	opzDrawObject	*_healthBackground;

	bool		_dead;

	heightState_t	_heightState;
	opz2DVector		_pos;
	float			_dir;
	speedState_t	_speedState;
	unsigned int	_speedingBeginTime;
	float			_height;
	float			_jumpSpeed;
	float			_hp;

	opzHosung(void);
	~opzHosung(void);

	void Reset();

	void InputProcess();
	bool CheckColWithCar();
	void FrameMove();

	void FrameDraw();
};

