#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define SIMON_WIDTH 16
#define SIMON_LENGTH 32

class CSimon : public CGameObject
{
protected:
	float vx, hsx, hsy;
public:
	CSimon(float x, float y, float vx, float hsx = 1, float hsy = 1);
	void Update(DWORD dt);
	void Render();
};    

