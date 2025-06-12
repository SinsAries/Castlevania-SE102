#include "Whip.h"
#include "Simon.h"
#include "Animations.h"

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Tạm thời không làm gì cả vì vị trí Whip được cập nhật thông qua Simon
}

void CWhip::UpdatePosition(float simon_x, float simon_y, int aniId, int frame, DWORD dt)
{
	// Cập nhật vị trí whip theo animation và frame hiện tại
	switch (aniId)
	{
	case static_cast<int>(AnimationID::SimonStandAttackRight):
		dx[0] = -15; dy[0] = 4;
		dx[1] = -7; dy[1] = 1;
		dx[2] = 20; dy[2] = -5;
		break;
	case static_cast<int>(AnimationID::SimonStandAttackLeft):
		dx[0] = 12; dy[0] = 4;
		dx[1] = 7; dy[1] = 1;
		dx[2] = -20; dy[2] = -5;
		break;
	case static_cast<int>(AnimationID::SimonSitAttackRight):
		dx[0] = -15; dy[0] = 6;
		dx[1] = -5; dy[1] = 5;
		dx[2] = 23; dy[2] = -2;
		break;
	case static_cast<int>(AnimationID::SimonSitAttackLeft):
		dx[0] = 13; dy[0] = 6;
		dx[1] = 5; dy[1] = 5;
		dx[2] = -22; dy[2] = -2;
		break;
	default:
		dx[0] = 0; dy[0] = 0;
		dx[1] = 0; dy[1] = 0;
		dx[2] = 0; dy[2] = 0;
		break;
	}

	frame = max(0, min(frame, 2)); // an toàn
	x = simon_x + dx[frame];
	y = simon_y + dy[frame];
}

void CWhip::Render()
{
	int aniId = nx > 0 ? static_cast<int>(AnimationID::WeaponWhipRight) : static_cast<int>(AnimationID::WeaponWhipLeft);
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
