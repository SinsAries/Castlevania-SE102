#include "Weapon.h"

#include "Mario.h"
#include "Simon.h"

extern CMario* mario;
extern CSimon* simon;

void CWeapon::Update(DWORD dt)
{
    // Nếu Simon không tấn công, ẩn vũ khí
    if (!simon->isAttacking)
    {
        x = -1000; // Đưa vũ khí ra ngoài màn hình
        y = -1000;
        curStep = -1;
        return; // Không cần tiếp tục cập nhật khi không tấn công
    }


	ULONGLONG now = GetTickCount64();
	if (curStep == -1)
	{
		curStep = 0;
		lastStepTime = now;
	}
	else
	{
		SetDxDy(simon->currentId);
		DWORD t = 300;
		if (now - lastStepTime > t)
		{
			curStep = (curStep + 1) % 3;
			lastStepTime = now;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}
		x = simon->x + dx[curStep];
		y = simon->y + dy[curStep];
		
		/*if (curStep == 0) 
		{
			x = simon->x + dx;
			y = simon->y + 2;
		}
		else if (curStep == 1)
		{
			x = simon->x;
			y = simon->y + 1;
		}
		else if (curStep == 2)
		{
			x = simon->x - 20;
			y = simon->y - 5;
		}*/
	}
}

void CWeapon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (simon->isAttacking) {
		if (simon->nx < 0) aniId = ID_ANI_WEAPON_LEFT;
		else aniId = ID_ANI_WEAPON_RIGHT;
	}
	else
		aniId = ID_ANI_WEAPON_IDLE;

	animations->Get(aniId)->Render(x, y);
}

void CWeapon::SetDxDy(int state = simon->currentId) {
	if (!simon->isAttacking) return;
	switch (state)
	{
	case ID_ANI_SIMON_STAND_ATTACK_RIGHT:
		dx[0] = -15; dy[0] = 4;
		dx[1] = -7; dy[1] = 1;
		dx[2] = 20; dy[2] = -5;
		break;
	case ID_ANI_SIMON_STAND_ATTACK_LEFT:
		dx[0] = 12; dy[0] = 4;
		dx[1] = 7; dy[1] = 1;
		dx[2] = -20; dy[2] = -5;
		break;
	case ID_ANI_SIMON_SIT_ATTACK_RIGHT:
		dx[0] = -15; dy[0] = 6;
		dx[1] = -5; dy[1] = 5;
		dx[2] = 23; dy[2] = -2;
		break;
	case ID_ANI_SIMON_SIT_ATTACK_LEFT:
		dx[0] = 13; dy[0] = 6;
		dx[1] = 5; dy[1] = 5;
		dx[2] = -22; dy[2] = -2;
		break;
	default:
		dx[0] = 14; dy[0] = 2;
		dx[1] = 0; dy[1] = 1;
		dx[2] = -20; dy[2] = -5;
		break;
	}
}
