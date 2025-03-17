#include "Game.h"
#include "Simon.h"
#include<random>

CSimon::CSimon(float x, float y, float vx, float hsx, float hsy) : CGameObject(x, y)
{
	this->vx = vx;
	this->hsx = hsx;
	this->hsy = hsy;
}

void CSimon::Update(DWORD dt)
{
	x += hsx * vx * dt;
	y += hsy * vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
	if (x <= 0 || x >= BackBufferWidth - SIMON_WIDTH || y <= 0 || y >= BackBufferHeight - SIMON_LENGTH) {
		hsx = (rand() % 2 == 1 ? -1 : 1) * (rand() % 5);
		hsy = (rand() % 2 == 1 ? -1 : 1) * (rand() % 5);
		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - SIMON_WIDTH)
		{
			x = (float)(BackBufferWidth - SIMON_WIDTH);
		}

		if (y <= 0)
		{
			y = 0;
		}
		else if (y >= BackBufferHeight - SIMON_LENGTH) {
			y = (float)(BackBufferHeight - SIMON_LENGTH);
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