#include "Knife.h"
#include "Simon.h"
#include "Animations.h"

void CKnife::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Tạm thời không làm gì cả vì vị trí Whip được cập nhật thông qua Simon
}

void CKnife::UpdatePosition(float simon_x, float simon_y, int aniId, int frame, DWORD dt)
{
	// Cập nhật vị trí whip theo animation và frame hiện tại
	vx = 0.5;
	x += vx * dt;
}

void CKnife::Render()
{
	int aniId = nx > 0 ? ID_ANI_WEAPON_KNIFE_RIGHT : ID_ANI_WEAPON_KNIFE_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
