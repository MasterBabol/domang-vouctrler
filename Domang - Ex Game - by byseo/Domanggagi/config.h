#pragma once

#define WND_CLASS_NAME	L"run_hosung"
#define WND_TITLE		L"달려라 호성"

//#define FULL_SCREEN
#define WINDOW_SCREEN

#define SCREEN_WIDTH	800
#define	SCREEN_HEIGHT	600

#define SCREEN_F_WIDTH	SCREEN_WIDTH.0f
#define SCREEN_F_HEIGHT	SCREEN_HEIGHT.0f

#define SCREEN_BUFFER	384
#define SCREEN_F_BUFFER	SCREEN_BUFFER.0f

#define GRAVITY			10.0f
#define HEIGHT_SCALE	500.0f

/******************************
		내부 시스템 환경설정
*******************************/
#ifdef _DEBUG
#else
#define _HAS_ITERATOR_DEBUGGING 0
#define _SECURE_SCL 0
#endif

#define		CMD_BUFFER_LENGTH	32