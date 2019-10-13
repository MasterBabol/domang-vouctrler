#pragma once

/*
좌표는 기본적으로 float형입니다.
그래서 float로 인자를 주면 더 빠릅니다.
*/

#include "TextureMgr.h"
#include "2DVector.h"

#include <d3dx9.h>
#include <tchar.h>

#define OVER_LEFT	1
#define OVER_RIGHT	2
#define OVER_TOP	4
#define OVER_BOTTOM	8

class opzDrawObject
{
public:
	void Draw(ID3DXSprite *sprite);
	void DrawWithCheckingInside(ID3DXSprite *sprite);
	void DrawWithAnimation(ID3DXSprite *sprite);
	void DrawWithScale(ID3DXSprite *sprite, float widthRatio, float heightRatio);

	void SetPos(float x, float y){_pos._x = x; _pos._y = y; D3DXMatrixTranslation(&_translation, _pos._x, _pos._y, 0.0f); }
	void SetAlpha(int alpha){_alpha = alpha; _color = D3DCOLOR_RGBA(255, 255, 255, _alpha); }
	void SetAngle(float angle)
	{
		 _angle = angle; D3DXMatrixRotationZ(&_rotation, _angle);
	}
	void ChangeAngle(float dr){SetAngle(dr + _angle); }
	void SetWidthRatio(float widthRatio){SetRatio(widthRatio, _heightRatio); }
	void SetHeightRatio(float heightRatio){SetRatio(_widthRatio, heightRatio); }
	void SetRatio(float widthRatio, float heightRatio)
	{
		_widthRatio = widthRatio;
		_heightRatio = heightRatio;
		 D3DXMatrixScaling(&_scaling, _widthRatio, _heightRatio, 1.0f);
	}
	void SetSize(int width, int height){SetRatio((float)width / _texture->_width, (float)height / _texture->_height);}
	void SetSize(float fWidth, float fHeight){SetRatio(fWidth / _texture->_width, fHeight / _texture->_height);}
	void SetWidthWithAdjustHeightRatio(float fWidth){float ratio = fWidth / _texture->_fWidth; SetRatio(ratio, ratio); }
	void SetHeightWithAdjustWidthRatio(float fHeight){float ratio = fHeight / _texture->_fHeight; SetRatio(ratio, ratio); }
	void SetRect(int left, int top, int right, int bottom){_rect.left = left; _rect.top = top; _rect.right = right; _rect.bottom = bottom; }
	void SetJ(int j){_j = j; SetRect(_texture->_width * j, 0, _texture->_width * (j + 1), _texture->_height); }
	WORD MoveRect(int dx, int dy);	//반환값은 0~15까지다. 1 : 왼쪽 넘음, 2: 오른쪽 넘음, 4: 위넘음, 8: 아래넘음
	void SetCenterToCenter(){_center.x =_texture->_fWidth / 2; _center.y = _texture->_fHeight / 2; _center.z = 0.0f; }
	void SetCenterToTopLeft(){_center.x = 0.0f; _center.y = 0.0f; _center.z = 0.0f;}	//SetCenterToTopLeft은 그림의 가운데를 왼쪽 위로 하는 함수이다.
	void SetCenterY(int y){_center.y = (float)y;}
	void SetCenterY(float y){_center.y = y;}
	void SetTexture(texture_t *texture){_texture = texture; init(); }
	void SetX(float x){SetPos(x, _pos._y);}
	void SetY(float y){SetPos(_pos._x, y);}
	void RandomizeJ(int max){_j = rand() % max; }
	
	void MovePos(float dx, float dy){SetPos(_pos._x + dx, _pos._y + dy);}
	void MoveX(float dx){MovePos(dx, 0.0f); }
	void MoveY(float dy){MovePos(0.0f, dy); }

	const opz2DVector &GetPos()const{return _pos; }
	const int &GetAlpha()const{return _alpha; }
	const float &GetAngle()const{return _angle; }
	const float &GetWidthRatio()const{return _widthRatio; }
	const float &GetHeightRatio()const{return _heightRatio; }
	int GetWidth()const{return int(_texture->_width * _widthRatio); }
	int GetHeight()const{return int(_texture->_height * _heightRatio);}
	float GetFWidth()const{return _texture->_fWidth * _widthRatio; }
	float GetFHeight()const{return _texture->_fHeight * _heightRatio;}
	const texture_t *GetTexture()const{return _texture; }

	void SetBoundary(float left, float top, float right, float bottom)
	{_leftBoundary = left; _topBoundary = top;
	_rightBoundary = right; _bottomBoundary = bottom; }

	void Animate(unsigned int delay, int maxJ);

	opzDrawObject(LPCWSTR name);
	opzDrawObject(LPCWSTR name, float sx, float sy);
	~opzDrawObject(void);

private:
	const texture_t *_texture;
	opz2DVector _pos;
	int _alpha;
	int _j;

	float _angle;
	float _widthRatio;
	float _heightRatio;

	D3DXMATRIX _world;
	D3DXMATRIX _translation;
	D3DXMATRIX _rotation;
	D3DXMATRIX _scaling;
	D3DXMATRIX _scale_rotation;
	D3DXCOLOR _color;
	D3DXVECTOR3 _center;
	RECT _rect;

	float _leftBoundary;
	float _topBoundary;
	float _rightBoundary;
	float _bottomBoundary;

	unsigned int _animationDelay;
	int	_maxJ;
	unsigned int _lastSwitchTime;

	void init();

	bool CheckInsideOfBoundary(const opz2DVector point);
};

