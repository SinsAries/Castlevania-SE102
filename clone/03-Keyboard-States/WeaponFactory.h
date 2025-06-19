#pragma once
#include "GameObject.h"
#include "GameIDs.h"

class CWeaponFactory
{
public:
	static LPGAMEOBJECT CreateSubWeapon(ESubWeaponType type, float x, float y, int nx);
};

