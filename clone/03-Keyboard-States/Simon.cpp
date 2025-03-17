#include "Simon.h"

void CSimon::Update(DWORD dt)
{
	if (isAttacking)
	{
		attackTime -= dt;
		if (attackTime <= 0)
		{
			isAttacking = false;
		}
	}

	// Di chuyển và rơi như bình thường
	x += vx * dt;
	y += vy * dt;
	vy += SIMON_GRAVITY * dt;

	// Kiểm tra va chạm với mặt đất
	if (y > GROUND_Y)
	{
		vy = 0; y = GROUND_Y;
	}

	// Kiểm tra biên giới
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}


void CSimon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	// Kiểm tra nếu Simon đang trên không
	if (y < GROUND_Y)
	{
		if (isAttacking)
		{
			aniId = (nx >= 0) ? ID_ANI_SIMON_SIT_ATTACK_RIGHT : ID_ANI_SIMON_SIT_ATTACK_LEFT;
		}
		else
		{
			aniId = (nx >= 0) ? ID_ANI_SIMON_SIT_RIGHT : ID_ANI_SIMON_SIT_LEFT;
		}
	}
	else if (isSitting)
	{
		if (isAttacking)
		{
			aniId = (nx >= 0) ? ID_ANI_SIMON_SIT_ATTACK_RIGHT : ID_ANI_SIMON_SIT_ATTACK_LEFT;
		}
		else
		{
			aniId = (nx >= 0) ? ID_ANI_SIMON_SIT_RIGHT : ID_ANI_SIMON_SIT_LEFT;
		}
	}
	else
	{
		if (isAttacking)
		{
			aniId = (nx > 0) ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT;
		}
		else if (vx > 0)
		{
			aniId = ID_ANI_SIMON_WALKING_RIGHT;
		}
		else if (vx < 0)
		{
			aniId = ID_ANI_SIMON_WALKING_LEFT;
		}
		else
		{
			aniId = (nx > 0) ? ID_ANI_SIMON_IDLE_RIGHT : ID_ANI_SIMON_IDLE_LEFT;
		}
	}

	animations->Get(aniId)->Render(x, y);
}


void CSimon::SetState(int state)
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
	case SIMON_STATE_ATTACK:
		isAttacking = true;
		attackTime = 900;
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		isSitting = false;
		isAttacking = false;
		break;
	}
	CGameObject::SetState(state);
}

void CSimon::KeyState(BYTE* states)
{
	if (isAttacking) {
		return;
	}

	CGame* game = CGame::GetInstance();
	if (game->IsKeyDown(DIK_RIGHT))
	{
		SetState(SIMON_STATE_WALKING_RIGHT);
		if (game->IsKeyDown(DIK_UP))
		{
			SetState(SIMON_STATE_JUMP);
		}
		if (game->IsKeyDown(DIK_SPACE))
		{
			SetState(SIMON_STATE_ATTACK);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		SetState(SIMON_STATE_WALKING_LEFT);
		if (game->IsKeyDown(DIK_UP))
		{
			SetState(SIMON_STATE_JUMP);
		}
		if (game->IsKeyDown(DIK_SPACE))
		{
			SetState(SIMON_STATE_ATTACK);
		}
	}
	else if (!isAttacking)
	{
		SetState(SIMON_STATE_IDLE);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		SetState(SIMON_STATE_JUMP);
	}

	if (game->IsKeyDown(DIK_DOWN))
	{
		SetState(SIMON_STATE_SIT);
	}

	if (game->IsKeyDown(DIK_SPACE))
	{
		SetState(SIMON_STATE_IDLE);
		SetState(SIMON_STATE_ATTACK);
	}
}

void CSimon::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
}

void CSimon::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		// Nếu đang tấn công, chỉ chuyển trạng thái khi attackTime đã hết
		if (!isAttacking || attackTime <= 0)
		{
			SetState(SIMON_STATE_IDLE);
		}
		break;
	}
}
