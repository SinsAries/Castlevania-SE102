#include "Game.h"
#include "AutoWalkSimon.h"

void CAutoWalkSimon::Update(DWORD dt)
{

	// simple fall down
	vy += SIMON_GRAVITY * dt;
	y += vy * dt;

	if (y > GROUND_Y_2)
	{
		vy = 0; y = GROUND_Y_2;
	}

	x += vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - SIMON_WIDTH) {
		if (vx > 0 && x > 290) x = 290;
		if (vx < 0 && x < 0) x = 0;

		vx = -vx;
	}
}

void CAutoWalkSimon::Render()
{
	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) CAnimations::GetInstance()->Get(ID_ANI_SIMON_WALKING_RIGHT)->Render(x, y);
	else CAnimations::GetInstance()->Get(ID_ANI_SIMON_WALKING_LEFT)->Render(x, y);
}