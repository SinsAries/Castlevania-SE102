#include "IdleState.h"
#include "Game.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"

void JumpState::Enter(CSimon* simon)
{
	if (simon->y == GROUND_Y)
	{
		simon->vy = -SIMON_JUMP_SPEED_Y;
	}
	simon->isSitting = false;
}

void JumpState::HandleInput(CSimon* simon, BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (game->IsKeyDown(DIK_SPACE) && !simon->isAttacking && simon->attackCoolDown <= 0) {
		simon->SetState(new AttackState());
	}
}

void JumpState::Update(CSimon* simon, DWORD dt)
{
	simon->vy += SIMON_GRAVITY * dt;
	simon->y += simon->vy * dt;

	if (simon->y > GROUND_Y)
	{
		simon->y = GROUND_Y;
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