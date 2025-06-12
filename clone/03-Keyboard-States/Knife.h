#pragma once
#include "Weapon.h"
#include "Animations.h"

class CKnife : public CWeapon
{
private:
	int nx;
	int dx[3], dy[3];
public:
	CKnife(float x, float y, int nx) : CWeapon(x, y, nx) {};

	void SetDirection(int direction) { nx = direction; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void UpdatePosition(float simon_x, float simon_y, int aniId, int frame, DWORD dt);
	void Render();
	float GetWidth() const override { return 0; }
	float GetHeight() const override { return 0; }
};