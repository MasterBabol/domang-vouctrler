/********************
		수학함수
********************/

#pragma once

#include <math.h>

extern const float PI;
extern const float DOUBLE_PI;
extern const float HALF_PI;
extern const float QUARTER_PI;

extern const float FLOAT_EPSILON;

#define SQRTTABLESIZE 256 /* Note: code below assumes this is 256. */

typedef union
{
    int      i;
    float    f;
} INTORFLOAT;

extern unsigned int ZSQRTTABLE[SQRTTABLESIZE];

__forceinline float fSQRT(float f)
{ 
	INTORFLOAT fi; unsigned int e, n; /* Get raw bits of floating point f. */
	fi.f = f; n = fi.i; /* Divide exponent by 2 -- this is done in-place, no need to shift all the way down to 0 the least significant bits and then back up again. Note that we are also dividing the exponent bias (127) by 2, this gets corrected when we add in the sqrttable entry. */
	e = (n >> 1) & 0x3f800000; /* n is the table index -- we're using 1 bit from the original exponent (e0) plus 7 bits from the mantissa. */
	n = (n >> 16) & 0xff; /* Add calculated exponent to mantissa + re-biasing constant from table. */
	fi.i = ZSQRTTABLE[n] + e; /* Return floating point result. */
	return fi.f;
}

inline float atan3(float y, float x)
{
	float theta = atan2(y, x);
	if(theta < 0)
		return DOUBLE_PI - theta;
	else
		return theta;
}

inline float SubtractThetaCCW(float theta1, float theta2)
{
	float ccw = theta1 - theta2;
	if(ccw < 0)
		return DOUBLE_PI + ccw;
	else
		return ccw;
}