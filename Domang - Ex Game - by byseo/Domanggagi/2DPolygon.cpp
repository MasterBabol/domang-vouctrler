#include "2DPolygon.h"
#include "2DLine.h"
#include "Math.h"

#define NULL	0

opz2DPolygon::opz2DPolygon(int n) :
	_n(n), _points(NULL), _dutyToDelete(true)
{
	_points = new opz2DVector[_n];
}

opz2DPolygon::opz2DPolygon(int n, opz2DVector *pool) :
	_n(n), _points(NULL), _dutyToDelete(false)
{
	_points = pool;
}

opz2DPolygon::~opz2DPolygon(void)
{
	if(_dutyToDelete)
		delete[] _points;
}

bool opz2DPolygon::CheckPointInside(const point_t &point, float epsilon) const
{
	opz2DLine line;
	int i, nextI;
	relation_t res;
	for(i = 0; i < _n; ++i)
	{
		if(i < _n - 1)
			nextI = i + 1;
		else
			nextI = 0;

		line.Set(_points[i], _points[nextI]);

		res = line.ClassifyPoint(point, epsilon);
		if(res == BEHIND) return false;
	}
	return true;
}

bool opz2DPolygon::CheckCollisionWithLine(const opz2DLine &line, float epsilon) const
{
	//포함하는지 먼저 본다.
	if(CheckPointInside(line._begin, epsilon) ||
		CheckPointInside(line._end, epsilon)
		) return true;

	point_t mid((line._begin + line._end) / 2.0f);
	if(CheckPointInside(mid, epsilon))
		return true;

	//아닐 경우 교차하는지 본다.
	int i, nextI;
	opz2DLine line2;
	for(i = 0; i < _n; ++i)
	{
		if(i < _n - 1)
			nextI = i + 1;
		else
			nextI = 0;

		line2.Set(_points[i], _points[nextI]);

		if(line.CheckCollisionWithLine(line2, epsilon))
			return true;
	}
	return false;
}

bool opz2DPolygon::CheckCollisionWithPolygon(const opz2DPolygon &other, float epsilon) const
{
	int i, j, nextI, nextJ;
	int m = other.GetN();
	opz2DLine line1, line2;
	for(i = 0; i < m; ++i)
	{
		if(i < m - 1)
			nextI = i + 1;
		else
			nextI = 0;

		line1.Set(other.GetPoint(i), other.GetPoint(nextI));

		for(j = 0; j < _n; ++j)
		{
			if(j < _n - 1)
				nextJ = j + 1;
			else
				nextJ = 0;

			line2.Set(_points[j], _points[nextJ]);

			if(line1.CheckCollisionWithLine(line2, FLOAT_EPSILON))
				return true;
		}
	}
	return false;
}