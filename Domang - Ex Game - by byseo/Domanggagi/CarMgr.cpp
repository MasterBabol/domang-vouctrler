#include "CarMgr.h"
#include "Macros.h"
#include "Timer.h"
#include "CarMgr_config.h"
#include "config.h"
#include "Hosung.h"
#include "Hosung_config.h"
#include "22Matrix.h"
#include "2DPolygon.h"
#include "2DLine.h"
#include "Car_config.h"
#include "SFXMgr.h"

/****************************
			public
****************************/
void opzCarMgr::S_Create()
{
	if(_instance == NULL)
	{
		new opzCarMgr();
	}
}

void opzCarMgr::S_Delete()
{
	SafeDelete(_instance);
}

/****************************
			private
****************************/
opzCarMgr *opzCarMgr::_instance = NULL;

opzCarMgr::opzCarMgr(void)
	: _startTime(opzTimer::S_GetAbsoluteTime()), _lastNewCarTime(opzTimer::S_GetAbsoluteTime())
{
	_instance = this;

	D3DXCreateSprite(opzDXMgr::S_GetDevice(), &_sprite);
	_unitDO = new opzDrawObject(L"car");
	_unitDO->SetSize(CAR_F_WIDTH, CAR_F_HEIGHT);
	_unitBoomDO = new opzDrawObject(L"boom");
	_unitBoomDO->Animate(BOOM_ANIMATION_DELAY, 3);
}

void opzCarMgr::ClearList()
{
	std::list<opzCar *>::iterator i, i_end;
	for(i = _list.begin(), i_end = _list.end();
		i != i_end;)
	{
		delete *i;
		i = _list.erase(i);
	}
}

opzCarMgr::~opzCarMgr(void)
{
	ClearList();
	SafeDelete(_unitDO);
	SafeRelease(_sprite);
}

void opzCarMgr::Reset()
{
	_startTime = opzTimer::S_GetAbsoluteTime();
	_lastNewCarTime = opzTimer::S_GetAbsoluteTime();
	ClearList();
}

void opzCarMgr::NewCar()
{
	const opz2DVector &hosungPos = opzHosung::S_GetPos();
	opz2DVector sponPos;
	
	switch(rand() % 4)
	{
	case 0:
		sponPos._x = hosungPos._x - CAR_BORN_DISTANCE_FROM_HOSUNG;
		sponPos._y = hosungPos._y + rand() % SCREEN_WIDTH;
		break;
	
	case 1:
		sponPos._x = hosungPos._x + rand() % SCREEN_HEIGHT;
		sponPos._y = hosungPos._y + CAR_BORN_DISTANCE_FROM_HOSUNG;
		break;

	case 2:
		sponPos._x = hosungPos._x + CAR_BORN_DISTANCE_FROM_HOSUNG;
		sponPos._y = hosungPos._y + rand() % SCREEN_WIDTH;
		break;

	case 3:
		sponPos._x = hosungPos._x + rand() % SCREEN_HEIGHT;
		sponPos._y = hosungPos._y - CAR_BORN_DISTANCE_FROM_HOSUNG;
		break;
	}

	_list.push_back(new opzCar(sponPos));
}

void opzCarMgr::CarCarColProcess()
{
	if(_list.size() == 0) return;

	opz2DVector vectorPoolForRects[8];
	opz2DPolygon polygon1(4, vectorPoolForRects), polygon2(4, vectorPoolForRects + 4);
	
	std::list<opzCar *>::const_iterator i, j, i_end;
	opzCar *car1, *car2;

	unsigned int now = opzTimer::S_GetAbsoluteTime();

	for(i = _list.begin(), i_end = _list.end(), ++i;
		i != i_end; ++i)
	{
		car1 = *i;
		polygon1.SetPoint(0, car1->GetLeftTop());
		polygon1.SetPoint(1, car1->GetRightTop());
		polygon1.SetPoint(2, car1->GetRightBottom());
		polygon1.SetPoint(3, car1->GetLeftBottom());
		for(j = _list.begin(); j != i; ++j)
		{
			car2 = *j;
			polygon2.SetPoint(0, car2->GetLeftTop());
			polygon2.SetPoint(1, car2->GetRightTop());
			polygon2.SetPoint(2, car2->GetRightBottom());
			polygon2.SetPoint(3, car2->GetLeftBottom());
			if(polygon1.CheckCollisionWithPolygon(polygon2, FLOAT_EPSILON))
			{
				car1->Dead();
				car2->Dead();

				_boomList.push_back(boom_t(car1->GetPos(), now));
				_boomList.push_back(boom_t(car2->GetPos(), now));

				opzSFXMgr::S_PlaySFX("data\\sfx\\boom.wav");
			}
		}
	}
}

void opzCarMgr::FrameMove()
{
	if(opzTimer::S_GetAbsoluteTime() - _lastNewCarTime > CARMGR_NEW_CAR_DELAY_TIME)
	{
		_lastNewCarTime += CARMGR_NEW_CAR_DELAY_TIME;
		int i, count = 1 + (opzTimer::S_GetAbsoluteTime() - _startTime) / GAME_LEVEL_PER_TIME;
		for(i = 0; i < count; ++i)
			NewCar();
	}

	{
		std::list<opzCar *>::const_iterator i, i_end;
		for(i = _list.begin(), i_end = _list.end();
			i != i_end; ++i)
		{
			(*i)->Update();
		}

		CarCarColProcess();

		for(i = _list.begin(), i_end = _list.end();
			i != i_end;)
		{
			if((*i)->CheckDead())
			{
				delete *i;
				i = _list.erase(i);
			}
			else
			{
				++i;
			}
		}
	}

	{
		unsigned int now = opzTimer::S_GetAbsoluteTime();
		std::list<boom_t>::iterator i, i_end;
		for(i = _boomList.begin(), i_end = _boomList.end();
			i != i_end;)
		{
			if(now - i->_beginTime > BOOM_SHOW_TIME)
			{
				i = _boomList.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
}

void opzCarMgr::Draw()
{
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	opz2DVector relativePos;
	opz2DVector hosungPos = opzHosung::S_GetPos();
	float hosungDir = opzHosung::S_GetDir();
	float cosValue = cos(hosungDir);
	float sinValue = sin(hosungDir);
	opz22Matrix rotationMatrix(cosValue, sinValue, -sinValue, cosValue);

	{
		std::list<opzCar *>::const_iterator i, i_end;
		for(i = _list.begin(), i_end = _list.end();
			i != i_end; ++i)
		{
			const opz2DVector &pos = (*i)->GetPos();
			float dir = (*i)->GetDir();

			relativePos = pos - hosungPos;
			relativePos.MultipiedBy22Matrix(rotationMatrix);		

			_unitDO->SetPos(HOSUNG_SCREEN_F_X + relativePos._y, HOSUNG_SCREEN_F_Y - relativePos._x);
			_unitDO->SetAngle(dir - hosungDir);
			_unitDO->Draw(_sprite);
		}
	}
	{
		std::list<boom_t>::const_iterator i, i_end;
		for(i = _boomList.begin(), i_end = _boomList.end();
			i != i_end; ++i)
		{
			relativePos = i->_pos - hosungPos;
			relativePos.MultipiedBy22Matrix(rotationMatrix);

			_unitBoomDO->SetPos(HOSUNG_SCREEN_F_X + relativePos._y, HOSUNG_SCREEN_F_Y - relativePos._x);
			_unitBoomDO->SetAngle(- hosungDir);
			_unitBoomDO->DrawWithAnimation(_sprite);
		}
	}
	_sprite->End();
}