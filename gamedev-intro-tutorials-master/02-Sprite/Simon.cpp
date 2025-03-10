#include "Game.h"
#include "Simon.h"

CSimon::CSimon(float x, float y, float vx) : CGameObject(x, y)
{
	this->vx = vx;
};

void CSimon::Update(DWORD dt)
{
	x += vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - SIMON_WIDTH) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - SIMON_WIDTH)
		{
			x = (float)(BackBufferWidth - SIMON_WIDTH);
		}
	}
}

void CSimon::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) ani = CAnimations::GetInstance()->Get(540);
	else ani = CAnimations::GetInstance()->Get(541);

	ani->Render(x, y);
}