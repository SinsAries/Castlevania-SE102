#pragma once
#include "Weapon.h"
#include "Animations.h"

const DWORD KnifeTime = 500;

class CKnife : public CWeapon
{
private:
	int dx[3], dy[3];
public:
	CKnife(float x, float y, int nx) : CWeapon(x, y, nx) {
		this->activeTime = KnifeTime;
		this->attackTime = KnifeTime;
	};

	//void SetDirection(int direction) { nx = direction; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void UpdatePosition(float simon_x, float simon_y, int aniId, int frame, DWORD dt);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x;
		top = y;
		right = x;
		bottom = y;
	}
};