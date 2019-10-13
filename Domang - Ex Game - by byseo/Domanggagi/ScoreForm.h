#pragma once

#include "basicform.h"
#include "Label.h"

class opzLabel;
class opzDrawObject;

class opzScoreForm :
	public opzBasicForm
{
public:
	opzScoreForm(void);
	~opzScoreForm(void);
	
protected:
	void Set();
	void Reset();

	void FrameMove();
	void FrameDraw();

private:
	ID3DXSprite*	_textSprite;
	ID3DXSprite*	_sprite;

	opzLabel *_message;
	opzDrawObject *_background;

	void GetMessage(LPWSTR message);
};

