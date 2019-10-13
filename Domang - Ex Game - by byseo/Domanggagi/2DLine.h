/*********************
		선분 클래스
*********************/
#pragma once

#include "2DVector.h"

enum relation_t
{
	NOTHING,
	COINCIDING,
	INFRONT,
	BEHIND,
	SPANNING,
	SAME_DIRECTION,
	OPPOSITE_DIRECTION,
	INNER,
	OUTER
};

class opz2DLine
{
public:
	point_t _begin;
	point_t _end;
	
	opz2DLine(void);
	opz2DLine(const point_t &begin, const point_t &end);
	~opz2DLine(void);

	relation_t ClassifyPoint(const point_t &point, float epsilon) const;
	void GetNormalInside(opz2DVector *normalVector) const;
	void GetNormalOutside(opz2DVector *normalVector) const;

	void Set(const point_t &begin, const point_t &end)
	{
		_begin = begin;		_end = end;
	}

	const opz2DLine &operator = (const opz2DLine &other)
	{
		_begin = other._begin;	_end = other._end;
		return *this;
	}

	bool CheckCollisionWithLine(const opz2DLine &other, float epsilon) const;
};

inline void opz2DLine::GetNormalInside(opz2DVector *normalVector) const
{
	normalVector->Set(_end._y - _begin._y, _begin._x - _end._x);
}

inline void opz2DLine::GetNormalOutside(opz2DVector *normalVector) const
{
	normalVector->Set(_begin._y - _end._y, _end._x - _begin._x);
}

inline void GetABC(float *a, float *b, float *c, const opz2DLine line)
{
	*a = line._end._y - line._begin._y;
	*b = line._begin._x - line._end._x;
	*c = *a * line._begin._x + *b * line._begin._y;
}

