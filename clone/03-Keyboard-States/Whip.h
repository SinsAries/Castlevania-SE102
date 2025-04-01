#pragma once
#include "GameObject.h"
#include "Animations.h"

class CWhip : public CGameObject
{
private:
	int nx;
	int dx[3], dy[3];
public:
	CWhip() : CGameObject(0, 0), nx(1) {}

	void SetDirection(int direction) { nx = direction; }
	void Update(DWORD dt) override;
	void UpdatePosition(float simon_x, float simon_y, int aniId, int frame);
	void Render();
};