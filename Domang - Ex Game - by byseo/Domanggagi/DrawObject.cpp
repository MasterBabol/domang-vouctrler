#include "DrawObject.h"
#include "22Matrix.h"
#include "Timer.h"
#include "2DPolygon.h"

void opzDrawObject::Draw(ID3DXSprite *sprite)
{
	if(_texture == NULL)return;

	D3DXMatrixMultiply(&_scale_rotation, &_scaling, &_rotation); //행렬곱
	D3DXMatrixMultiply(&_world, &_scale_rotation, &_translation);//행렬곱

	sprite -> SetTransform(&_world); //행렬을 적용

	sprite -> Draw(_texture->_texture, &_rect, &_center, NULL, _color); //그림

	return;
}

inline bool opzDrawObject::CheckInsideOfBoundary(const opz2DVector point)
{
	return point._x >= _leftBoundary && point._y >= _topBoundary &&
		point._x <= _rightBoundary && point._y <= _bottomBoundary;
}

void opzDrawObject::DrawWithCheckingInside(ID3DXSprite *sprite)
{
	if(CheckInsideOfBoundary(_pos))
	{
		Draw(sprite);
		return;
	}
	
	//center 중심으로
	float leftWidth, rightWidth, topHeight, bottomHeight;
	
	leftWidth = _center.x;
	topHeight = _center.y;
	rightWidth = _widthRatio * _texture->_fWidth - _center.x;
	bottomHeight = _heightRatio * _texture->_fHeight - _center.y;

	opz2DVector p1(- leftWidth, - topHeight);
	opz2DVector p2(+ rightWidth, - topHeight);
	opz2DVector p3(+ rightWidth, - bottomHeight);
	opz2DVector p4(- leftWidth, + bottomHeight);

	float cosValue = cos(_angle);
	float sinValue = sin(_angle);

	opz22Matrix rotationMatrix(cosValue, -sinValue, sinValue, cosValue);

	p1.MultipiedBy22Matrix(rotationMatrix);
	p2.MultipiedBy22Matrix(rotationMatrix);
	p3.MultipiedBy22Matrix(rotationMatrix);
	p4.MultipiedBy22Matrix(rotationMatrix);

	p1 += _pos;
	p2 += _pos;
	p3 += _pos;
	p4 += _pos;

	if(CheckInsideOfBoundary(p1) || CheckInsideOfBoundary(p2)
		|| CheckInsideOfBoundary(p3) || CheckInsideOfBoundary(p4))
	{
		Draw(sprite);
		return;
	}
}

void opzDrawObject::DrawWithAnimation(ID3DXSprite *sprite)
{
	if(opzTimer::S_GetAbsoluteTime() - _lastSwitchTime > _animationDelay)
	{
		_lastSwitchTime  += _animationDelay;
		++_j;
		if(_j >= _maxJ) _j = 0;
		SetJ(_j);
	}
	Draw(sprite);
}

void opzDrawObject::DrawWithScale(ID3DXSprite *sprite, float widthRatio, float heightRatio)
{
	D3DXMatrixScaling(&_scaling, _widthRatio * widthRatio, _heightRatio * heightRatio, 1.0f);
	Draw(sprite);
}

void opzDrawObject::init()
{
	SetAlpha(255);
	SetJ(0);
	SetAngle(0.0f);
	SetRatio(1.0f, 1.0f);
	SetCenterToCenter();
	
	return;
}

WORD opzDrawObject::MoveRect(int dx, int dy)
{
	int ReturnValue = 0;
	if(_rect.left + dx < 0)
	{
		_rect.right -= _rect.left;
		_rect.left = 0;
		ReturnValue += OVER_LEFT;
	}
	else if(_rect.right + dx > _texture->_width)
	{
		_rect.left = _texture->_width - _rect.right + _rect.left;
		_rect.right = _texture->_width;
		ReturnValue += OVER_RIGHT;
	}
	else
	{
		_rect.left += dx;
		_rect.right += dx;
	}
	if(_rect.top + dy < 0)
	{
		_rect.bottom -= _rect.top;
		_rect.top = 0;
		ReturnValue += OVER_TOP;
	}
	else if(_rect.bottom + dy > _texture->_height)
	{
		_rect.top = _texture->_height - _rect.bottom + _rect.top;
		_rect.bottom = _texture->_height;
		ReturnValue += OVER_BOTTOM;
	}
	else
	{
		_rect.top += dy;
		_rect.bottom += dy;	
	}

	return ReturnValue;
}

opzDrawObject::opzDrawObject(LPCWSTR name) :
	_texture(NULL), _pos(), _alpha(0), _j(0),
	_angle(0.0f), _widthRatio(0.0f), _heightRatio(0.0f),
	_leftBoundary(0.0f), _topBoundary(0.0f), _rightBoundary(0.0f), _bottomBoundary(0.0f),
	_animationDelay(0U), _maxJ(0)
{
	_texture = opzTextureMgr::S_Bisr(name);
	SetPos(0.0f, 0.0f);
	init();

	return;
}

opzDrawObject::opzDrawObject(LPCWSTR name, float sx, float sy) :
	_texture(NULL), _pos(), _alpha(0), _j(0),
	_angle(0.0f), _widthRatio(0.0f), _heightRatio(0.0f),
	_leftBoundary(0.0f), _topBoundary(0.0f), _rightBoundary(0.0f), _bottomBoundary(0.0f),
	_animationDelay(0U), _maxJ(0)
{
	_texture = opzTextureMgr::S_Bisr(name);
	SetPos(sx, sy);
	init();

	return; 
}

opzDrawObject::~opzDrawObject(void)
{
}

void opzDrawObject::Animate(unsigned int delay, int maxJ)
{
	_animationDelay = delay;
	_maxJ = maxJ;
	_lastSwitchTime = opzTimer::S_GetAbsoluteTime();
}