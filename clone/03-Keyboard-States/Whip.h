#pragma once
#include "Weapon.h"
#include "Animations.h"

class CWhip : public CWeapon
{
private:
	int dx[3], dy[3];
public:
	CWhip(float x, float y, int nx) : CWeapon(x, y, nx) {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void UpdatePosition(float simon_x, float simon_y, int aniId, int frame, DWORD dt);
	void Render();

	float GetWidth() const override { return 0; }
	float GetHeight() const override { return 0; }
};