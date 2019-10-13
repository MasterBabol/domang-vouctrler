/*********************
		다각형
*********************/

#pragma once

#include "2DVector.h"

#include <assert.h>

class opz2DLine;

class opz2DPolygon
{
public:
	opz2DPolygon(int n);
	opz2DPolygon(int n, opz2DVector *pool);
	~opz2DPolygon(void);

	int GetN() const {return _n; }
	const opz2DVector &GetPoint(int i) const {return *(_points + i); }

	void SetPoint(int i, const opz2DVector &point);
	
	bool CheckPointInside(const point_t &point, float epsilon) const;
	bool CheckCollisionWithLine(const opz2DLine &line, float epsilon) const;			//포함 관계일 때 반례임 수정 필요
	bool CheckCollisionWithPolygon(const opz2DPolygon &other, float epsilon) const;	//포함 관계일 때 반례임 수정 필요

private:
	int _n;
	opz2DVector *_points;
	bool _dutyToDelete;
};

inline void opz2DPolygon::SetPoint(int i, const opz2DVector &point)
{
	assert(i >= 0 && i < _n);
	if(i < 0 || i >= _n) return;

	_points[i] = point;
}