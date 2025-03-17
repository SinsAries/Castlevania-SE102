#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
#include "Game.h"

#define Y_TO_MARIO -22
#define LEFT_X_TO_MARIO -10
#define RIGHT_X_TO_MARIO 10

#pragma region ANIMATION_ID

#define ID_ANI_WEAPON 2000

#pragma endregion

class CWeapon : public CGameObject
{
public:
	CWeapon(float x, float y) : CGameObject(x, y) { }
	void Update(DWORD dt);
	void Render();
};

