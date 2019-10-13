#include "GrassDrawer.h"
#include "GrassDrawer_config.h"
#include "Hosung.h"
#include "Macros.h"
#include "config.h"
#include "Hosung_config.h"

/*****************************
			public
*****************************/
void opzGrassDrawer::S_Create()
{
	if(_instance == NULL)
	{
		new opzGrassDrawer();
	}
}

void opzGrassDrawer::S_Delete()
{
	SafeDelete(_instance);
}

/*****************************
			private
*****************************/
opzGrassDrawer *opzGrassDrawer::_instance = NULL;

opzGrassDrawer::opzGrassDrawer(void) :
	_sprite(NULL), _unitTile(NULL)
{
	_instance = this;

	D3DXCreateSprite(opzDXMgr::S_GetDevice(), &_sprite);
	_unitTile = new opzDrawObject(L"grass");
	_unitTile->SetSize(TILE_F_WIDTH, TILE_F_HEIGHT);
	_unitTile->SetBoundary(-20.0f, -20.0f, SCREEN_F_WIDTH + 20.0f, SCREEN_F_HEIGHT + 20.0f);
}

opzGrassDrawer::~opzGrassDrawer(void)
{
	SafeDelete(_unitTile);
	SafeRelease(_sprite);
}

void opzGrassDrawer::Reset()
{
}

void opzGrassDrawer::Draw()
{
	const opz2DVector &hosungPos = opzHosung::S_GetPos();
	
	int beginUnitX = static_cast<int>(hosungPos._x - SCREEN_F_HEIGHT / 2.0f - SCREEN_F_BUFFER) / TILE_HEIGHT,
		beginUnitY = static_cast<int>(hosungPos._y - SCREEN_F_WIDTH / 2.0f - SCREEN_F_BUFFER) / TILE_WIDTH,
		endUnitX = static_cast<int>(hosungPos._x + SCREEN_F_HEIGHT / 2.0f + SCREEN_F_BUFFER) / TILE_HEIGHT,
		endUnitY = static_cast<int>(hosungPos._y + SCREEN_F_WIDTH / 2.0f + SCREEN_F_BUFFER) / TILE_WIDTH;

	float hosungDir = opzHosung::S_GetDir();

	_unitTile->SetAngle(-hosungDir);
	float cosValue = cos(-hosungDir);
	float sinValue = sin(-hosungDir);
	opz2DVector baseX(cosValue, sinValue);
	opz2DVector baseY(-sinValue, cosValue);
	
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	int unitX, unitY;
	opz2DVector currentPos;	//절대 위치
	opz2DVector relativePos;	//호성에게 상대 위치
	for(unitX = beginUnitX; unitX < endUnitX; ++unitX)
	{
		for(unitY = beginUnitY; unitY < endUnitY; ++unitY)
		{
			currentPos._x = static_cast<const float>(unitX) * TILE_F_HEIGHT;
			currentPos._y = static_cast<const float>(unitY) * TILE_F_WIDTH;

			relativePos = baseX * (currentPos._x - hosungPos._x);
			relativePos += baseY * (currentPos._y - hosungPos._y);

			_unitTile->SetPos(relativePos._y + HOSUNG_SCREEN_F_X, -relativePos._x + HOSUNG_SCREEN_F_Y);
			_unitTile->DrawWithCheckingInside(_sprite);
		}
	}

	_sprite->End();
}