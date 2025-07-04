#pragma once

#include "Sprite.h"

class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

	int anchorX; 
	int anchorY; 

public:
	CAnimationFrame(LPSPRITE sprite, int time, int anchorX, int anchorY) {
		this->sprite = sprite;
		this->time = time;
		this->anchorX = anchorX; 
		this->anchorY = anchorY; 
	}

	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }

	int GetAnchorX() { return anchorX; }
	int GetAnchorY() { return anchorY; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;