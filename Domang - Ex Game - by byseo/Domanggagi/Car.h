#pragma once

#include "2DVector.h"

class opzCar
{
public:
	opzCar(const opz2DVector &sponPos);
	~opzCar(void);

	void Update();

	const opz2DVector &GetPos()const{return _pos; }
	float GetDir()const{return _dir; }
	const opz2DVector &GetLeftTop() const{return _leftTop;}
	const opz2DVector &GetRightTop() const{return _rightTop;}
	const opz2DVector &GetRightBottom() const{return _rightBottom;}
	const opz2DVector &GetLeftBottom() const{return _leftBottom;}

	void Dead() {_dead = true;}
	bool CheckDead() const {return _dead; }

private:
	bool _dead;

	opz2DVector	_pos;
	float		_dir;
	opz2DVector	_v;

	opz2DVector _leftTop;
	opz2DVector _rightTop;
	opz2DVector _rightBottom;
	opz2DVector _leftBottom;

	void UpdateBoundary(float cosValue, float sinValue);
};

