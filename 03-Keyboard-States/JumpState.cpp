#include "IdleState.h"
#include "Game.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"

JumpState::JumpState(CSimon* simon, int direction) {
	nx = direction;
	simon->nx = direction;
}

void JumpState::Enter(CSimon* simon)
{
	if (simon->isOnPlatform)
	{
		simon->vy = -SIMON_JUMP_SPEED_Y;
	}
	simon->isSitting = false;
}

void JumpState::HandleInput(CSimon* simon, BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (game->IsKeyDown(DIK_SPACE) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 0));
		return;
	}
	else if (game->IsKeyDown(DIK_X) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 1));
		return;
	}
	else if (game->IsKeyDown(DIK_B) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 2));
		return;
	}
}

void JumpState::Update(CSimon* simon, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	simon->vy += SIMON_GRAVITY * dt;

	CCollision::GetInstance()->Process(simon, dt, coObjects);

	simon->y += simon->vy * dt;

	CGame* game = CGame::GetInstance();
	if (game->IsKeyDown(DIK_RIGHT))
		simon->vx = SIMON_WALKING_SPEED;
	else if (game->IsKeyDown(DIK_LEFT))
		simon->vx = -SIMON_WALKING_SPEED;
	else
		simon->vx = 0;

	simon->x += simon->vx * dt;

	if (simon->isOnPlatform)
	{
		//simon->y = GROUND_Y;
		simon->vy = 0;
		simon->SetState(new IdleState());
	}
	simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void JumpState::Render(CSimon* simon)
{
	int aniId;

	if (simon->isAttacking)
	{
		aniId = (simon->nx >= 0) ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT;
	}
	else
	{
		aniId = (simon->nx >= 0) ? ID_ANI_SIMON_SIT_RIGHT : ID_ANI_SIMON_SIT_LEFT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}