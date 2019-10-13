#pragma once

#include "2DVector.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//유니코드 버전
LPWSTR MyStrcat(LPWSTR dest, LPCWSTR src);
LPWSTR MyStrcpy(LPWSTR dest, LPCWSTR src);

//멀티바이트 버전
/*
char *MyStrcat(char *dest, const char *src);
char *MyStrcpy(char *dest, const char *src);
*/
template <typename T>
float Square(T x){return x * x; }

bool CheckPowerOfTwo(int x);

template <typename map_t, typename iter_t>
void DeleteMap(map_t *map)
{
	iter_t iter, it_end;
	for(iter = map->begin(), it_end = map->end();
		iter != it_end;)
	{
		delete iter->second;
		iter = map->erase(iter);
	}
}

template <typename vector_t, typename iter_t>
void DeleteVector(vector_t *vector)
{
	iter_t iter, it_end;
	for(iter = vector->begin(), it_end = vector->end();
		iter != it_end; ++iter)
	{
		delete *iter;
		*iter = NULL;
	}
	vector->clear();
}

int CheckDirExists(LPCWSTR dir);