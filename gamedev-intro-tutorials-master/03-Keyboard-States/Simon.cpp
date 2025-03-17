#include "Simon.h"

void CSimon::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

	// simple fall down
	vy += ax * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;
	if (abs(vy) > abs(maxVx)) vy = maxVx;

	DebugOutTitle(L"vx = %0.5f", this->vx);

	// simple screen edge collision!!!
	if (x > 290) x = 290;
	if (x < 0) x = 0;
	if (y > 210) y = 210;
	if (y < 0) y = 0;
}

void CSimon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	aniId = ID_ANI_SIMON_IDLE_RIGHT;

	float d = 0;

	animations->Get(aniId)->Render(x, y);
}

void CSimon::SetState(int state)
{
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		maxVx = SIMON_WALKING_SPEED;
		ax = SIMON_ACCEL_WALK_X;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		maxVx = -SIMON_WALKING_SPEED;
		ax = -SIMON_ACCEL_WALK_X;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		maxVx = -SIMON_WALKING_SPEED;
		ax = -SIMON_ACCEL_WALK_X;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_SIT:
		maxVx = SIMON_WALKING_SPEED;
		ax = SIMON_ACCEL_WALK_X;
		vy = SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;
	default:
		break;
	}

	CGameObject::SetState(state);
}

void CSimon::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	//case DIK_S:
	//	SetState(SIMON_STATE_JUMP);  // Thực hiện nhảy khi phím 'S' được nhấn
	//	break;
	}
}

void CSimon::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	switch (KeyCode)
	{
	//case DIK_S:
	//	SetState(SIMON_STATE_RELEASE_JUMP);  // Thực hiện khi nhả phím 'S'
	//	break;
	//case DIK_DOWN:
	//	SetState(SIMON_STATE_SIT_RELEASE);  // Thực hiện khi phím 'Down' được thả
	//	break;
	}
}

void CSimon::KeyState(BYTE* states)
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
	else if (game->IsKeyDown(DIK_UP))
	{
		SetState(SIMON_STATE_JUMP);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		SetState(SIMON_STATE_SIT);
	}
	else
	{
		SetState(SIMON_STATE_IDLE);
	}
}
