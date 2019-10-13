#pragma once

#include "DXMgr.h"
#include "TextureMgr.h"
#include "vector2D.h"

class opzDoObject
{
public:
	opzDoObject();
	virtual ~opzDoObject(void);

	void Render();

	virtual void SetPos(float x, float z){_pos._x = x; _pos._y = z; D3DXMatrixTranslation(&_translation, _pos._x, _height, _pos._y); }
	void SetHeight(float h){_height = h;  D3DXMatrixTranslation(&_translation, _pos._x, _height, _pos._y); }
	void SetAngle(float angle)
	{
		_rotated = true;
		D3DXMatrixRotationY(&_rotation, _angle - angle); _angle = angle;
	}
	void ChangeAngle(float dr){SetAngle(dr + _angle); }
	void SetWidthRatio(float widthRatio){SetRatio(widthRatio, _heightRatio); }
	void SetHeightRatio(float heightRatio){SetRatio(_widthRatio, heightRatio); }
	inline void SetRatio(float widthRatio, float heightRatio)
	{
		_widthRatio = widthRatio;
		_heightRatio = heightRatio;
		 D3DXMatrixScaling(&_scaling, _widthRatio, 1.0f, _heightRatio);
	}
	inline void SetSize(int width, int height){SetRatio((float)width, (float)height);}
	inline void SetSize(float fWidth, float fHeight){SetRatio(fWidth, fHeight);}
	inline void SetX(float x){SetPos(x, _pos._y);}
	inline void SetY(float y){SetPos(_pos._x, y);}
	
	inline void MovePos(float dx, float dy){SetPos(_pos._x + dx, _pos._y + dy);}
	void MoveX(float dx){MovePos(dx, 0.0f); }
	void MoveY(float dy){MovePos(0.0f, dy); }

	const vector2D_t &GetPos()const{return _pos; }
	const float &GetAngle()const{return _angle; }
	const D3DXMATRIX &GetWorld()const{return _world; }

protected:
	LPDirectDevice		_pd3dDevice;
	vector2D_t			_pos;
	float				_height;
	bool				_rotated;
	float				_angle;
	float				_widthRatio;
	float				_heightRatio;

	D3DXMATRIX	_world;
	D3DXMATRIX	_translation;
	D3DXMATRIX	_rotation;
	D3DXMATRIX	_scaling;
};

