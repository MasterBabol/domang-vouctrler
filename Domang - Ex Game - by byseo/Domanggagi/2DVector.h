//////////////////////////////////////////////////////////////////////////////////////////////////
//
// ÆÄÀÏÀÌ¸§: 2DVector.h
//
// ³»¿ë: ÁÂÇ¥Ã³¸®
//
//	±èµ¿Çö
//
//////////////////////////////////////////////////////////////////////////////////////////////////

/***********************
		2D º¤ÅÍ
***********************/
#pragma once

#include "2DVector.h"
#include "Math.h"
#include "22Matrix.h"

#include <math.h>
#include <limits>

class opz2DVector
{
public:
    float _x;
    float _y;

    explicit opz2DVector( float x_in = 0.0f, float y_in = 0.0f ) : _x(x_in), _y(y_in) { }

	opz2DVector(const opz2DVector &other)
	{
		_x = other._x;
		_y = other._y;
	}

    void Set(float xi, float yi)
    {
        _x = xi;
        _y = yi;
    }

    void Reset()
    {
        Set(0.0f, 0.0f);
    }

    bool operator <  (const opz2DVector &other) const
    {
        return (_x < other._x) || (_x == other._x && _y < other._y);
    }

    bool operator >  (const opz2DVector &other) const
    {
        return (_x > other._x) || (_x == other._x && _y > other._y);
    }

	const opz2DVector &operator = (const opz2DVector &other)
	{
		_x = other._x;		_y = other._y;
		return *this;
	}

    const opz2DVector &operator += (const opz2DVector &other)
    {
        _x += other._x;
        _y += other._y;
        return (*this);
    }

    const opz2DVector &operator += (float offset)
    {
        _x += offset;
        _y += offset;
        return (*this);
    }

    const opz2DVector &operator -= (const opz2DVector &other)
    {
        _x -= other._x;
        _y -= other._y;
        return (*this);
    }

    const opz2DVector &operator -= (float offset)
    {
        _x -= offset;
        _y -= offset;
        return (*this);
    }

    const opz2DVector &operator /= (float div)
    {
        _x /= div;
        _y /= div;
        return (*this);
    }

    const opz2DVector &operator *= (float mul)
    {
        _x *= mul;
        _y *= mul;
        return (*this);
    }

    opz2DVector operator + (const opz2DVector &other) const
    {
        opz2DVector copy = *this;
        return (copy += other);
    }

    opz2DVector operator + (float other) const
    {
        opz2DVector copy = *this;
        return (copy += other);
    }

    opz2DVector operator - (const opz2DVector &other) const
    {
        opz2DVector copy = *this;
        return (copy -= other);
    }

    opz2DVector operator -() const
    {
        return (opz2DVector(0, 0) - *this);
    }

    opz2DVector operator - (float other) const
    {
        opz2DVector copy = *this;
        return (copy -= other);
    }

    opz2DVector operator / (float div) const
    {
        opz2DVector copy = *this;
        return (copy /= div);
    }

    opz2DVector operator * (const float mul) const
    {
        opz2DVector copy = *this;
        return (copy *= mul);
    }

	float operator * (const opz2DVector &other) const
    {
		return this->_x * other._x + this->_y * other._y;
    }
	
	float GetSize() const;

    float GetSquareSize() const
    {
        return (_x * _x + _y * _y);
    }

    bool CheckOrigin(float epsilon) const;
	
	bool CheckEquals(const opz2DVector &other, float epsilon) const;

    bool CheckEquals(const float xi, const float yi, float epsilon) const;
	
	float GetDirectionInWorld() const;

	void AdjustSize(float size);

	void MultipiedBy22Matrix(const opz22Matrix &matrix);

};

typedef opz2DVector point_t;


inline float opz2DVector::GetSize() const
{
	return sqrt(_x * _x + _y * _y);
}

inline bool opz2DVector::CheckOrigin(float epsilon) const
{
    return abs(_x) < epsilon && abs(_y) < epsilon;
}

inline bool opz2DVector::CheckEquals(const opz2DVector &other, float epsilon) const
{
    return abs(_x - other._x) <= epsilon && abs(_y - other._y) <= epsilon;
}

inline bool opz2DVector::CheckEquals(const float xi, const float yi, float epsilon) const
{
    return abs(xi - _x) <= epsilon && abs(yi - _y) <= epsilon;
}

inline float opz2DVector::GetDirectionInWorld() const
{
	float t = atan2(_y, _x);
	while(t < 0.0f)t += DOUBLE_PI;
	return t;
}

inline void opz2DVector::AdjustSize(float size)
{
	if(CheckOrigin(std::numeric_limits<float>::epsilon()) == true)
	{
		return;
	}

	*this /= GetSize();
	*this *= size;
}

inline void opz2DVector::MultipiedBy22Matrix(const opz22Matrix &matrix)
{
	float newX = matrix._a11 * _x + matrix._a12 * _y;
	float newY = matrix._a21 * _x + matrix._a22 * _y;
	_x = newX;
	_y = newY;
}