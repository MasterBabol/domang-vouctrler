#include "PlayForm.h"
#include "Hosung.h"
#include "GrassDrawer.h"
#include "CarMgr.h"
#include "PlayForm_config.h"
#include "Timer.h"
#include "FrameLimiter.h"
#include "config.h"
#include "ScoreMgr.h"

/******************
		public
******************/
opzPlayForm::opzPlayForm(void) :
	_sprite(NULL), _mode(PLAY_MODE),
	_startTime(opzTimer::S_GetAbsoluteTime()),
	_hosung(NULL),
	_gameoverTime(0U), _gameoverAlpha(0.0f)
{
	SetNextForm(FORM_SCORE);

	opzHosung::S_Create();
	opzGrassDrawer::S_Create();
	opzCarMgr::S_Create();

	D3DXCreateSprite(opzDXMgr::S_GetDevice(), &_sprite);

	_gameover = new opzDrawObject(L"gameover");
	_gameover->SetCenterToTopLeft();
	_gameover->SetSize(SCREEN_F_WIDTH, SCREEN_F_HEIGHT);
}

opzPlayForm::~opzPlayForm(void)
{
	SafeDelete(_gameover);
	
	SafeRelease(_sprite);

	opzCarMgr::S_Delete();
	opzGrassDrawer::S_Delete();
	opzHosung::S_Delete();
}

void opzPlayForm::Set()
{
	opzBGMMgr::S_SetBGM(BGM_MAIN);
}

void opzPlayForm::Reset()
{
	SetNextForm(FORM_SCORE);
	_mode = PLAY_MODE;
	_startTime = opzTimer::S_GetAbsoluteTime();
	_gameoverTime = 0U;
	_gameoverAlpha = 0.0f;
	opzHosung::S_Reset();
	opzGrassDrawer::S_Reset();
	opzCarMgr::S_Reset();
	opzScoreMgr::S_Reset();
}

/******************
		protected
******************/

void opzPlayForm::FrameMove()
{
	if(opzDInput::S_GetKeyState(DIK_ESCAPE, KEY_DOWN))
		Exit();

	switch(_mode)
	{
	case PLAY_MODE:
		if(opzHosung::S_CheckDead())
		{
			_mode = GAMEOVER_MODE;
			_gameoverTime = opzTimer::S_GetAbsoluteTime();
			_gameover->SetAlpha(0);
			opzScoreMgr::S_AddScore((_gameoverTime - _startTime) / 1000);
		}
		else
		{
			opzCarMgr::S_FrameMove();
			opzHosung::S_FrameMove();
		}
		break;

	case GAMEOVER_MODE:
		if(opzTimer::S_GetAbsoluteTime() - _gameoverTime > GAMEOVER_SHOW_TIME)
		{
			Exit();
		}
		else
		{
			_gameoverAlpha += GAMEOVER_OPAQUE_SPEED * opzFrameLimiter::S_GetDT();
			int nextAlpha = static_cast<int>(_gameoverAlpha);
			if(nextAlpha > 255)
			{
				_gameover->SetAlpha(255);
			}
			else
			{
				_gameover->SetAlpha(nextAlpha);
			}
		}
		break;
	}
}

void opzPlayForm::FrameDraw()
{
	switch(_mode)
	{
	case PLAY_MODE:
		opzGrassDrawer::S_Draw();
		opzCarMgr::S_Draw();
		opzHosung::S_FrameDraw();
		break;

	case GAMEOVER_MODE:
		opzGrassDrawer::S_Draw();
		opzCarMgr::S_Draw();
		opzHosung::S_FrameDraw();
		_sprite->Begin(D3DXSPRITE_ALPHABLEND);
		_gameover->Draw(_sprite);
		_sprite->End();
		break;
	}
}
