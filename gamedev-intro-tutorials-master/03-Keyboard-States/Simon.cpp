#include "Simon.h"

void CSimon::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

	// simple fall down
	vy += SIMON_GRAVITY * dt;

	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	DebugOutTitle(L"vx = %0.5f", this->vx);

	// BAD & sinful platform collision handling, see next sample for correct collision handling
	if (y > GROUND_Y)
	{
		vy = 0; y = GROUND_Y;
	}

	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}

void CSimon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	// Simon is still on air check
	if (y < GROUND_Y)
	{
		if (nx >= 0)
			aniId = ID_ANI_SIMON_SIT_RIGHT;
		else
			aniId = ID_ANI_SIMON_SIT_LEFT;
	}
	else if (isSitting)
	{
		if (nx > 0)
			aniId = ID_ANI_SIMON_SIT_RIGHT;
		else
			aniId = ID_ANI_SIMON_SIT_LEFT;
	}
	else if (vx == 0)
	{
		if (nx > 0) aniId = ID_ANI_SIMON_IDLE_RIGHT;
		else aniId = ID_ANI_SIMON_IDLE_LEFT;
	}
	else if (vx > 0)
	{
		aniId = ID_ANI_SIMON_WALKING_RIGHT;
	}
	else // vx < 0
	{
		aniId = ID_ANI_SIMON_WALKING_LEFT;
	}

	if (aniId == -1) aniId = ID_ANI_SIMON_IDLE_RIGHT;

	float d = 0;
	if (isSitting) d = SIMON_SIT_HEIGHT_ADJUST;

	animations->Get(aniId)->Render(x, y + d);
}

void CSimon::SetState(int state)
{
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = SIMON_WALKING_SPEED;
		ax = SIMON_ACCEL_WALK_X;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -SIMON_WALKING_SPEED;
		ax = -SIMON_ACCEL_WALK_X;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		if (isSitting) break;
		if (y == GROUND_Y)
		{
			if (abs(this->vx) == SIMON_RUNNING_SPEED)
				vy = -SIMON_JUMP_RUN_SPEED_Y;
			else
				vy = -SIMON_JUMP_SPEED_Y;
		}
		break;

	case SIMON_STATE_RELEASE_JUMP:
		if (vy < 0) vy += SIMON_JUMP_SPEED_Y / 2;
		break;

	case SIMON_STATE_SIT:
		if (y == GROUND_Y)
		{
			state = SIMON_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0;
		}
		break;

	case SIMON_STATE_SIT_RELEASE:
		isSitting = false;
		state = SIMON_STATE_IDLE;
		break;

	case SIMON_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;
	}

	CGameObject::SetState(state);
}
