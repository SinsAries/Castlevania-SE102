#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define SIMON_WIDTH 16
#define SIMON_LENGTH 32

#define SIMON_GRAVITY			0.002f
#define GROUND_Y_2 50.0f

#pragma region ANIMATION_ID

#define ID_ANI_SIMON_WALKING_RIGHT 1500
#define ID_ANI_SIMON_WALKING_LEFT 1501

#pragma endregion

class CAutoWalkSimon : public CGameObject
{
protected:
	float vx;
public:
	CAutoWalkSimon(float x, float y, float vx) : CGameObject(x, y)
	{
		this->vx = vx;
	};
	void Update(DWORD dt);
	void Render();
};


