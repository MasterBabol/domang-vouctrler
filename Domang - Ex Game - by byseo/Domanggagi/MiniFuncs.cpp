#include "MiniFuncs.h"

#include <io.h>
#include <stdio.h>

/***********************
유니코드 버전
************************/
LPWSTR MyStrcat(LPWSTR dest, LPCWSTR src)
{
	while(*dest)
		dest++;  
 
	while(*src)
		*(dest++) = *(src++);
		*dest = 0;
	return dest;
}

LPWSTR MyStrcpy(LPWSTR dest, LPCWSTR src)
{
	while(*src)
	{
		*(dest++) = *(src++);
		*dest = 0;
	}
	return dest;
}

/***********************
멀티바이트 버전
************************/
/*
char *MyStrcat(char *dest, const char *src)
{
	while(*dest)
		dest++;  
 
	while(*src)
		*(dest++) = *(src++);
		*dest = 0;
	return dest;
}

char *MyStrcpy(char *dest, const char *src)
{
	while(*src)
	{
		*(dest++) = *(src++);
		*dest = 0;
	}
	return dest;
}
*/
bool CheckPowerOfTwo(int x)
{
	return (!(x & (x - 1)));
}

int CheckDirExists(LPCWSTR dir) {
  _wfinddatai64_t c_file;
  intptr_t hFile;
  int result = 0;
  
  hFile = _wfindfirsti64(dir, &c_file);
  if (c_file.attrib & _A_SUBDIR ) result = 1;
  _findclose(hFile);

  return result;
}