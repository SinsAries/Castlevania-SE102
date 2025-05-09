#pragma once
#include "Weapon.h"
#include "Animations.h"

const DWORD WhipTime = 900;

class CWhip : public CWeapon
{
private:
	int dx[3], dy[3];
public:
	CWhip(float x, float y, int nx) : CWeapon(x, y, nx) {
		this->activeTime = WhipTime;
		this->attackTime = WhipTime;
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void UpdatePosition(float simon_x, float simon_y, int aniId, int frame, DWORD dt) override;
	void Render() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x;
		top = y;
		right = x;
		bottom = y;
	}
};