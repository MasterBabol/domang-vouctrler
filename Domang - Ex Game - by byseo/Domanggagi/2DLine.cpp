#include "2DLine.h"
#include "Math.h"

#include <math.h>
#include <algorithm>

opz2DLine::opz2DLine(void) :
	_begin(), _end()
{
}

opz2DLine::opz2DLine(const point_t &begin, const point_t &end) :
	_begin(begin), _end(end)
{
}

opz2DLine::~opz2DLine(void)
{
}

relation_t opz2DLine::ClassifyPoint(const point_t &point, float epsilon) const
{
	opz2DVector normal;
	GetNormalInside(&normal);
	float dis = -(point - _begin) * normal;
	if(-epsilon <= dis && dis <= epsilon) return COINCIDING;
	else if(dis < 0.0f) return BEHIND;
	else return INFRONT;
}

void GetSmallBoundary(float p1x1, float p1x2, float p2x1, float p2x2, float *psx, float *psy)
{
	float sx, ex;
	if(p1x1 < p1x2)
	{
		sx = p1x1;
		ex = p1x2;
	}
	else
	{
		sx = p1x2;
		ex = p1x1;
	}

	if(p2x1 < p2x2)
	{
		sx = std::max(p2x1, sx);
		ex = std::min(p2x2, ex);
	}
	else
	{
		sx = std::max(p2x2, sx);
		ex = std::min(p2x1, ex);
	}
	*psx = sx;
	*psy = ex;
}

bool opz2DLine::CheckCollisionWithLine(const opz2DLine &other, float epsilon = FLOAT_EPSILON) const
{
	float a1, b1, c1, a2, b2, c2;
	GetABC(&a1, &b1, &c1, *this);
	GetABC(&a2, &b2, &c2, other);

	float det = a1 * b2 - a2 * b1;

	if(fabsf(det) < epsilon)
	{
		return false;
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / det;
		float y = (a1 * c2 - a2 * c1) / det;

		float sx, ex;
		GetSmallBoundary(_begin._x, _end._x, other._begin._x, other._end._x, &sx, &ex);
		float sy, ey;
		GetSmallBoundary(_begin._y, _end._y, other._begin._y, other._end._y, &sy, &ey);

		if(sx <= x && x <= ex && sy <= y && y <= ey)
			return true;
	}
	return false;
}