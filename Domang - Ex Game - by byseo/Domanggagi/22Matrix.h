/*******************
		2*2 За·Д
*******************/

#pragma once

class opz2DVector;

class opz22Matrix
{
public:
	opz22Matrix(float a11, float a12, float a21, float a22);
	~opz22Matrix(void);

	float _a11, _a12;
	float _a21, _a22;
};

