#include "Simon.h"

void Simon::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	// simple fall down
	vy += SIMON_GRAVITY * dt;
	if (y > GROUND_Y)
	{
		vy = 0; y = GROUND_Y;
	}
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}

void Simon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (y < GROUND_Y)
	{
		if (nx >= 0)
			aniId = ID_ANI_SIMON_SIT_RIGHT;
		else
			aniId = ID_ANI_SIMON_SIT_LEFT;
	}
	else
	{
		if (vx > 0)
			aniId = ID_ANI_SIMON_WALKING_RIGHT;
		else if (vx < 0)
			aniId = ID_ANI_SIMON_WALKING_LEFT;
		else
		{
			if (nx > 0)
				aniId = ID_ANI_SIMON_IDLE_RIGHT;
			else
				aniId = ID_ANI_SIMON_IDLE_LEFT;
		}
	}
	animations->Get(aniId)->Render(x, y);
}

void Simon::SetState(int state)
{
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		if (y == GROUND_Y)
		{
			vy = -SIMON_JUMP_SPEED_Y;
		}
		break;
	case SIMON_STATE_SIT:
		if (y == GROUND_Y)
		{
			vx = 0;
			vy = 0;
			isSitting = true;
		}
		break;
	}
	CGameObject::SetState(state);
}

void Simon::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	if (game->IsKeyDown(DIK_RIGHT))
	{
		SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		SetState(SIMON_STATE_WALKING_LEFT);
	}
	else
		SetState(SIMON_STATE_IDLE);

	if (game->IsKeyDown(DIK_SPACE))
	{
		SetState(SIMON_STATE_JUMP);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		SetState(SIMON_STATE_SIT);
	}
	else if (isSitting)
	{
		isSitting = false;
		y -= 10;
	}
}