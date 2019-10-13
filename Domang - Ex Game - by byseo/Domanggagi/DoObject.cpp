#include "DoObject.h"

/*******************
		public
*******************/
opzDoObject::opzDoObject()
	: _pos(), _height(0.0f),
	_rotated(false), _angle(0.0f), _widthRatio(1.0f), _heightRatio(1.0f),
	_world(), _translation(), _rotation(), _scaling()
{
	_pd3dDevice = opzDXMgr::S_GetDevice();
	D3DXMatrixRotationY(&_rotation, _angle);
	SetPos(0.0f, 0.0f);
}

opzDoObject::~opzDoObject(void)
{
}

void opzDoObject::Render()
{
	if(!_rotated)
	{
		D3DXMatrixRotationY(&_rotation, 0.0f);
	}
	_rotated = false;
	D3DXMatrixMultiply(&_scaling, &_scaling, &_rotation); //행렬곱
	D3DXMatrixMultiply(&_world, &_scaling, &_translation);//행렬곱

	_pd3dDevice->SetTransform(D3DTS_WORLD, &_world); //행렬을 적용

	_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, 4, 0, 2);

	return;
}