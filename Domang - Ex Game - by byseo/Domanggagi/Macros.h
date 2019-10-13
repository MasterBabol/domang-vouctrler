#pragma once

//자주 쓰는 매크로를 정의해 놓았습니다.
#define INT_MAX				2147483647
#define UNSIGNED_INT_MAX	4294967295U

#define CMD_BUFFER_LENGTH	32

//메크로
#define SafeDelete(p) if(p != NULL){delete p; p = NULL; }
#define SafeDeleteArray(p) if(p != NULL){delete[] p; p = NULL; }
#define SafeRelease(p) if(p != NULL){p->Release(); p = NULL; }

#define STR_SWITCH_BEGIN(key) \
{ \
	static stdext::hash_map< std::wstring, int > s_hm; \
	static bool s_bInit = false; \
	bool bLoop = true; \
	while(bLoop) \
	{ \
		int nKey = -1; \
		if(s_bInit) { nKey=s_hm[key]; bLoop = false; } \
		switch(nKey) \
		{ \
			case -1: {

#define CASE(token)  } case __LINE__: if(!s_bInit) s_hm[token] = __LINE__; else {

#define DEFAULT()    } case 0: default: if(s_bInit) {

#define STR_SWITCH_END() \
			} \
		} \
		if(!s_bInit) s_bInit=true; \
	} \
}
