#include "Weapon.h"

#include "Mario.h"

extern CMario* mario;

void CWeapon::Update(DWORD dt)
{
	if (mario->nx > 0)
	{
		x = mario->x + RIGHT_X_TO_MARIO;
		y = mario->y + Y_TO_MARIO;
	}
	else
	{
		x = mario->x + LEFT_X_TO_MARIO;
		y = mario->y + Y_TO_MARIO;
	}
}

void CWeapon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_WEAPON;
	animations->Get(aniId)->Render(x, y);
}