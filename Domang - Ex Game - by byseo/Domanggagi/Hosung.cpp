#include "VOUCtrlerClass.h"
#include "config.h"
#include "Hosung.h"
#include "Hosung_config.h"
#include "DInput.h"
#include "Macros.h"
#include "FrameLimiter.h"
#include "Hosung.h"
#include "2DLine.h"
#include "2DPolygon.h"
#include "CarMgr.h"
#include "Timer.h"
#include "SFXMgr.h"

/***************************
			public
***************************/
void opzHosung::S_Create()
{
	if(_instance == NULL)
	{
		new opzHosung();
	}
}

void opzHosung::S_Delete()
{
	SafeDelete(_instance);
}

/***************************
			private
***************************/
opzHosung *opzHosung::_instance = NULL;

opzHosung::opzHosung(void) : 
	_dt(NULL),
	_sprite(NULL), _do(NULL),
	_dead(false),
	_heightState(HS_GROUND),
	_pos(), _dir(0.0f), _speedState(NORMAL_SPEED), _speedingBeginTime(opzTimer::S_GetAbsoluteTime()),
	_height(0.0f), _jumpSpeed(0.0f),
	_hp(HOSUNG_MAX_HP)
{
	_instance = this;

	_dt = opzFrameLimiter::S_GetDTPtr();

	D3DXCreateSprite(opzDXMgr::S_GetDevice(), &_sprite);
	_do = new opzDrawObject(L"hosung", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
//	_do->SetHeightWithAdjustWidthRatio(HOSUNG_LENGTH);
	_do->Animate(HOSUNG_NORMAL_ANIMATION_DELAY, 2);

	int i;
	for(i = 0; i < 10; ++i)
	{
		_healthBlocks[i] = new opzDrawObject(L"health_block", HOSUNG_SCREEN_F_X + (i - 5) * HEALTH_BLOCK_WIDTH + HEALTH_BLOCK_WIDTH / 2, HOSUNG_SCREEN_F_Y + 150.0f);
	}
	_healthBackground = new opzDrawObject(L"health_background", HOSUNG_SCREEN_F_X, HOSUNG_SCREEN_F_Y + 150.0f);

}

opzHosung::~opzHosung(void)
{
	SafeDelete(_healthBackground);
	int i;
	for(i = 0; i < 10; ++i)
		delete _healthBlocks[i];

	SafeDelete(_do);
	SafeRelease(_sprite);
}

void opzHosung::Reset()
{
	_dead = false;
	_heightState = HS_GROUND;
	_pos.Reset();
	_dir = 0.0f;
	_speedState = NORMAL_SPEED;
	_do->Animate(HOSUNG_NORMAL_ANIMATION_DELAY, 2);
	_speedingBeginTime = opzTimer::S_GetAbsoluteTime();
	_height = 0.0f;
	_jumpSpeed = 0.0f;
	_hp = HOSUNG_MAX_HP;
}

void opzHosung::InputProcess()
{
	VOUCtrlerInputs* joyInput = VOUCtrlerClass::GetInput();
	switch(_heightState)
	{
	case HS_GROUND:
		if (joyInput)
		{
			float amp = (joyInput->accelY / 256.0f) - (VOUCtrlerClass::GetZeroPoint()->accelY / 256.0f);
			if (amp > 0)
				amp *= 2*amp;
			else
				amp *= 2*amp * -1;
			if (amp > 1.0f)
				amp = 1.0f;
			if (amp < -1.0f)
				amp = -1.0f;
			_dir += amp;
		}
		if(opzDInput::S_GetKeyState(DIK_LEFT, KEY_HOLD))
		{
			_dir -= HOSUNG_ANGLE_SPEED * *_dt;
		}
		else if(opzDInput::S_GetKeyState(DIK_RIGHT, KEY_HOLD))
		{
			_dir += HOSUNG_ANGLE_SPEED * *_dt;
		}

		if((opzDInput::S_GetKeyState(DIK_Q, KEY_DOWN)
			|| VOUCtrlerClass::GetButtonState(VOUCtrlerButtons::VOUCButton_RRight, VOUCtrlerButtonsState::KEYDOWN)) && _hp >= HOSUNG_JUMP_SPENDING_HP)
		{
			_hp -= HOSUNG_JUMP_SPENDING_HP;
			_jumpSpeed = HOSUNG_JUMP_SPEED;
			_heightState = HS_AIR;
			opzSFXMgr::S_PlaySFX("data\\sfx\\shout.wav");
			if(_hp < HOSUNG_MAX_HP * 0.2f)
				opzSFXMgr::S_PlayUnderSFX("data\\sfx\\heartbeat.wav");
		}

		if((opzDInput::S_GetKeyState(DIK_W, KEY_DOWN)
			 || VOUCtrlerClass::GetButtonState(VOUCtrlerButtons::VOUCButton_LLeft, VOUCtrlerButtonsState::KEYDOWN))  && _hp >= HOSUNG_SPEEDING_SPENDING_HP)
		{
			_hp -= HOSUNG_SPEEDING_SPENDING_HP;
			_speedState = FAST_SPEED;
			_speedingBeginTime = opzTimer::S_GetAbsoluteTime();
			_do->Animate(HOSUNG_FAST_ANIMATION_DELAY, 2);
			opzSFXMgr::S_PlaySFX("data\\sfx\\speed.wav");
	
			if(_hp < HOSUNG_MAX_HP * 0.2f)
				opzSFXMgr::S_PlayUnderSFX("data\\sfx\\heartbeat.wav");
		}
		break;
	}
}

bool opzHosung::CheckColWithCar()
{
	float cosValue = cos(_dir),
		sinValue = sin(_dir);
	opz22Matrix rotationMatrix(cosValue, -sinValue, sinValue, cosValue);
	opz2DVector hosungUpper(HOSUNG_HALF_LENGTH, 0.0f);
	opz2DVector hosungDowner(-HOSUNG_HALF_LENGTH, 0.0f);
	hosungUpper.MultipiedBy22Matrix(rotationMatrix);
	hosungDowner.MultipiedBy22Matrix(rotationMatrix);
	hosungUpper += _pos;
	hosungDowner += _pos;
	opz2DLine hosungLine(hosungUpper, hosungDowner);

	std::list<opzCar *>::const_iterator i, i_end;
	opz2DVector vectorPoolForRect[4];
	opz2DPolygon polygon(4, vectorPoolForRect);
	opzCar *car;
	for(i = opzCarMgr::S_GetListBegin(), i_end = opzCarMgr::S_GetListEnd();
		i != i_end; ++i)
	{
		car = *i;
		polygon.SetPoint(0, car->GetLeftTop());
		polygon.SetPoint(1, car->GetRightTop());
		polygon.SetPoint(2, car->GetRightBottom());
		polygon.SetPoint(3, car->GetLeftBottom());
		if(polygon.CheckCollisionWithLine(hosungLine, 0.1f))
		{
			return true;
		}
	}
	return false;
}

void opzHosung::FrameMove()
{
	InputProcess();

	float speed;
	switch(_speedState)
	{
	case FAST_SPEED:
		if(opzTimer::S_GetAbsoluteTime() - _speedingBeginTime > HOSUNG_SPEEDING_TIME)
		{
			_do->Animate(HOSUNG_NORMAL_ANIMATION_DELAY, 2);
			_speedState = NORMAL_SPEED;
		}
		else
			speed = HOSUNG_FAST_SPEED * *_dt;
		break;
	}

	switch(_speedState)
	{
	case NORMAL_SPEED:
		speed = HOSUNG_NORMAL_SPEED * *_dt;
		break;
	}

	opz2DVector dPos(
		speed * cos(_dir),
		speed * sin(_dir)
		);

	_pos += dPos;

	switch(_heightState)
	{
	case HS_GROUND:
		if(CheckColWithCar())
		{
			_dead = true;
			opzSFXMgr::S_PlaySFX("data\\sfx\\die.wav");
		}
		break;

	case HS_AIR:
		_jumpSpeed -= GRAVITY;
		_height += _jumpSpeed;
		if(_height <= HOSUNG_GROUND_ABOVE_HEIGHT)
		{
			_heightState = HS_ABOVE_GROUND;
		}
		break;
	}

	switch(_heightState)
	{
	case HS_ABOVE_GROUND:
		if(CheckColWithCar())
		{
			_height = HOSUNG_GROUND_ABOVE_HEIGHT;
			_jumpSpeed = 0.0f;
		}
		else
		{
			_jumpSpeed -= GRAVITY;
			_height += _jumpSpeed;

			if(_height <= 0.0f)
			{
				_heightState = HS_GROUND;
				_height = 0.0f;
			}
		}
		break;
	}

	_hp += HOSUNG_HEALTH_INCREASING_SPEED * *_dt;
	if(_hp > HOSUNG_MAX_HP)
		_hp = HOSUNG_MAX_HP;
}

void opzHosung::FrameDraw()
{
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	_healthBackground->Draw(_sprite);
	int i, hpCount = static_cast<int>(10.0f * _hp / HOSUNG_MAX_HP);
	if(hpCount > 10)hpCount = 10;
	for(i = 0; i < hpCount; ++i)
		_healthBlocks[i]->Draw(_sprite);

	switch(_dead)
	{
	case false:
		_do->SetWidthRatio(1.0f + _height / HEIGHT_SCALE);
		_do->SetHeightRatio(1.0f + _height / HEIGHT_SCALE);
		_do->DrawWithAnimation(_sprite);
		break;
		
	case true:
		_do->Draw(_sprite);
		break;
	}
	_sprite->End();
}
