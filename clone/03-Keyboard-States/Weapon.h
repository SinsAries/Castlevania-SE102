#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
#include "Game.h"

#define Y_TO_MARIO -50
#define LEFT_X_TO_MARIO -10
#define RIGHT_X_TO_MARIO 10

#pragma region ANIMATION_ID

#define ID_ANI_WEAPON_LEFT 2000
#define ID_ANI_WEAPON_RIGHT 2001

#pragma endregion

class CWeapon : public CGameObject
{
protected:
	int curStep = -1;
	int lastStepTime = 0;
public:
	CWeapon(float x, float y) : CGameObject(x, y) { }
	void Update(DWORD dt);
	void Render();
};

