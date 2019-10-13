#pragma once

#include "2DVector.h"
#include "DXMgr.h"
#include "DrawObject.h"

class opzGrassDrawer
{
public:
	static void S_Create();
	static void S_Delete();

	static void S_Reset(){_instance->Reset(); }

	static void S_Draw(){_instance->Draw(); }

private:
	static opzGrassDrawer *_instance;

	ID3DXSprite		*_sprite;
	opzDrawObject	*_unitTile;

	opzGrassDrawer(void);
	~opzGrassDrawer(void);

	void Reset();

	void Draw();
};