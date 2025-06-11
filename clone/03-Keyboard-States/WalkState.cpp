#include "IdleState.h"
#include "Simon.h"
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
	// Sử dụng hằng số từ CSimon.h
	simon->vx = (nx > 0) ? CSimon::WALKING_SPEED : -CSimon::WALKING_SPEED;
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

	// Sử dụng hằng số từ CSimon.h
	simon->vy += CSimon::GRAVITY * dt;
	simon->y += simon->vy * dt;

	if (simon->y > CSimon::GROUND_Y)
	{
		simon->y = CSimon::GROUND_Y;
		simon->vy = 0;
	}

	// Giới hạn di chuyển trong màn hình, dùng hằng số
	if (simon->x < 0) simon->x = 0;
	if (simon->x > CSimon::WORLD_BOUNDARY_RIGHT) simon->x = CSimon::WORLD_BOUNDARY_RIGHT;

	simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void WalkState::Render(CSimon* simon)
{
	int aniId;
	// Sử dụng enum class AnimationID
	if (nx > 0)
		aniId = static_cast<int>(AnimationID::SimonWalkRight);
	else
		aniId = static_cast<int>(AnimationID::SimonWalkLeft);

	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}