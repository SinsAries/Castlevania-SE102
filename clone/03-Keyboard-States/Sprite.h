#pragma once

#include "Texture.h"
#include "Game.h"

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;
public:
	friend class CTiledBackground;
	
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);

	void Draw(float x, float y);

	int GetWidth() { return right - left; }
	int GetHeight() { return bottom - top; }
	int GetLeft() { return left; }
	int GetTop() { return top; }
	int GetRight() { return right; }
	int GetBottom() { return bottom; }
	LPTEXTURE GetTexture() { return texture; }
};

typedef CSprite* LPSPRITE;