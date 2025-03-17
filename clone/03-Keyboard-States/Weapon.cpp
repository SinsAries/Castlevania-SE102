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
		DWORD t = 300;
		if (now - lastStepTime > t)
		{
			curStep = (curStep + 1) % 3;
			lastStepTime = now;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}
		if (curStep == 0) 
		{
			x = simon->x + 14;
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
		}
	}
}
	

void CWeapon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (simon->isAttacking)
		aniId = ID_ANI_WEAPON_LEFT;
	else
		aniId = ID_ANI_WEAPON_RIGHT;

	animations->Get(aniId)->Render(x, y);
}