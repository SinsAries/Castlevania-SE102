#include "IdleState.h"
#include "Game.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"

WalkState::WalkState(CSimon* simon, int direction) {
	nx = direction;
	simon->nx = direction;
}

void WalkState::Enter(CSimon* simon)
{
	simon->vx = nx > 0 ? SIMON_WALKING_SPEED : -SIMON_WALKING_SPEED;
	simon->isSitting = false;
}

void WalkState::HandleInput(CSimon* simon, BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (game->IsKeyDown(DIK_SPACE) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 0));
		return;
	} 
	if (game->IsKeyDown(DIK_X) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 1));
		return;
	}

	if (game->IsKeyDown(DIK_UP))
	{
		simon->SetState(new JumpState());
		return;
	}

	if (game->IsKeyDown(DIK_DOWN))
	{
		simon->SetState(new SitState());
		return;
	}

	if (!game->IsKeyDown(DIK_LEFT) && simon->vx < 0)
	{
		simon->SetState(new IdleState());
		return;
	}

	if (!game->IsKeyDown(DIK_RIGHT) && simon->vx > 0)
	{
		simon->SetState(new IdleState());
		return;
	}

	if (!(game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_RIGHT)))
	{
		simon->SetState(new IdleState());
		return;
	}
}

void WalkState::Update(CSimon* simon, DWORD dt)
{
	simon->x += simon->vx * dt;

	simon->vy += SIMON_GRAVITY * dt;
	simon->y += simon->vy * dt;
	
	if (simon->y > GROUND_Y)
	{
		simon->y = GROUND_Y;
		simon->vy = 0;
	}
	if (simon->x < 0) simon->x = 0;
	if (simon->x > 990) simon->x = 990;
	simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void WalkState::Render(CSimon* simon)
{
	int aniId = (nx > 0) ? ID_ANI_SIMON_WALKING_RIGHT : ID_ANI_SIMON_WALKING_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}