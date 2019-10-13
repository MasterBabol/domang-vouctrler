#include "VOUCtrlerClass.h"
#include "ScoreForm.h"
#include "ScoreMgr.h"
#include "Label.h"
#include "DrawObject.h"
#include "FontMgr.h"
#include "ScoreMgr.h"
#include "config.h"
#include "ScoreForm_config.h"

/******************
		public
******************/
opzScoreForm::opzScoreForm(void) :
	_sprite(NULL),
	_message(NULL), _background(NULL)
{
	D3DXCreateSprite(opzDXMgr::S_GetDevice(), &_textSprite);
	D3DXCreateSprite(opzDXMgr::S_GetDevice(), &_sprite);

	WCHAR message[LABEL_BUFFER_LENGTH];
	GetMessage(message);

	_message = new opzLabel(message, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 0,
		opzFontMgr::S_GetNewFont(SCORE_FORM_FONT_FACE, SCORE_FORM_FONT_SIZE));
	_message->SetColor(255, 255, 255);

	_background = new opzDrawObject(L"score");
	_background->SetCenterToTopLeft();
	_background->SetSize(SCREEN_F_WIDTH, SCREEN_F_HEIGHT);
}

opzScoreForm::~opzScoreForm(void)
{
	SafeDelete(_background);
	SafeDelete(_message);

	SafeRelease(_sprite);
	SafeRelease(_textSprite);
}

void opzScoreForm::Set()
{	
	opzBGMMgr::S_SetBGM(BGM_SCORE);

	WCHAR message[LABEL_BUFFER_LENGTH];
	GetMessage(message);

	_message->SetStr(message);
}

void opzScoreForm::Reset()
{
}

void opzScoreForm::FrameMove()
{
	VOUCtrlerInputs* joyInput = VOUCtrlerClass::GetInput();

	if(opzDInput::S_GetKeyState(DIK_ESCAPE, KEY_DOWN)
		|| VOUCtrlerClass::GetButtonState(VOUCtrlerButtons::VOUCButton_LRight, VOUCtrlerButtonsState::KEYDOWN))
		Exit();

	if(opzDInput::S_GetKeyState(DIK_RETURN, KEY_DOWN)
		 || VOUCtrlerClass::GetButtonState(VOUCtrlerButtons::VOUCButton_RLeft, VOUCtrlerButtonsState::KEYDOWN))
	{
		SetNextForm(FORM_PLAY);
		Exit();
	}
}

void opzScoreForm::FrameDraw()
{
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	_background->Draw(_sprite);
	_sprite->End();
	
	_textSprite->Begin(D3DXSPRITE_ALPHABLEND);
	_message->Draw(_textSprite, 0);
	_textSprite->End();
}

/******************
		private
******************/
void opzScoreForm::GetMessage(LPWSTR message)
{
	swprintf_s(message, LABEL_BUFFER_LENGTH, L"%d초 동안 버텼당께", opzScoreMgr::S_GetScore());
}