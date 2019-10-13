#include "Car.h"
#include "Hosung.h"
#include "Car_config.h"
#include "Math.h"
#include "FrameLimiter.h"
#include "22Matrix.h"

#include <assert.h>

opzCar::opzCar(const opz2DVector &sponPos)
	: _dead(false), _pos(sponPos), _dir(0.0f), _v(), 
	_leftTop(), _rightTop(), _rightBottom(), _leftBottom()
{
}


opzCar::~opzCar(void)
{
}

void AdjustAngle(float hopeAngle, float angleSpeed, float *angle)
{
	float ccw = SubtractThetaCCW(hopeAngle, *angle);
	float cw = SubtractThetaCCW(*angle, hopeAngle);
	if(min(ccw, cw) < angleSpeed)
	{
		*angle = hopeAngle;
	}
	else
	{
		if(cw < ccw)
		{
			*angle -= angleSpeed;
			if(*angle > DOUBLE_PI)*angle -= DOUBLE_PI;
		}
		else
		{
			*angle += angleSpeed;
			if(*angle < 0.0f)*angle += DOUBLE_PI;
		}
	}
}

void opzCar::Update()
{
	const opz2DVector hosungPos = opzHosung::S_GetPos();
	float _hopeDir = (hosungPos - _pos).GetDirectionInWorld();

	float dt = opzFrameLimiter::S_GetDT();
	
	//차의 방향을 계산한다.
	AdjustAngle(_hopeDir, CAR_ANGLE_SPEED * dt, &_dir);
	
	float cosValue = cos(_dir),
		sinValue = sin(_dir);

	opz2DVector f(cosValue, sinValue);

	f *= CAR_SPEED * dt;

	_v += f;

	float size = _v.GetSquareSize();
	if(size > CAR_SPEED_SQUARE)
	{
		_v *= CAR_SPEED / sqrt(size);
	}

	_pos += _v;

	//외곽 사각형도 업데이트 시켜준다.
	UpdateBoundary(cosValue, sinValue);
}

void opzCar::UpdateBoundary(float cosValue, float sinValue)
{
	opz22Matrix rotationMatrix(cosValue, -sinValue, sinValue, cosValue);

	_leftTop.Set(-CAR_F_HALF_HEIGHT, -CAR_F_HALF_WIDTH);
	_leftTop.MultipiedBy22Matrix(rotationMatrix);
	_leftTop += _pos;
	_rightTop.Set(CAR_F_HALF_HEIGHT, -CAR_F_HALF_WIDTH);
	_rightTop.MultipiedBy22Matrix(rotationMatrix);
	_rightTop += _pos;
	_rightBottom.Set(CAR_F_HALF_HEIGHT, CAR_F_HALF_WIDTH);
	_rightBottom.MultipiedBy22Matrix(rotationMatrix);
	_rightBottom += _pos;
	_leftBottom.Set(-CAR_F_HALF_HEIGHT, CAR_F_HALF_WIDTH);
	_leftBottom.MultipiedBy22Matrix(rotationMatrix);
	_leftBottom += _pos;
}