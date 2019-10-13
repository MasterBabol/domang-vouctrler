#pragma once

#include "basicform.h"
#include "Hosung.h"
#include "DXMgr.h"

enum playFormMode_t
{
	PLAY_MODE,
	GAMEOVER_MODE
};

class opzPlayForm :
	public opzBasicForm
{
public:
	opzPlayForm(void);
	~opzPlayForm(void);

protected:
	void Set();
	void Reset();

	void FrameMove();
	void FrameDraw();

private:
	ID3DXSprite*	_sprite;

	playFormMode_t	_mode;

	unsigned int	_startTime;

	opzHosung		*_hosung;
	opzDrawObject	*_gameover;
	float			_gameoverAlpha;

	unsigned int	_gameoverTime;
};

