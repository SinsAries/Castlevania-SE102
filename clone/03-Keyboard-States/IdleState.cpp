#include "IdleState.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"
#include "Simon.h"      // Quan trọng: Thêm Simon.h
#include "Game.h"
#include "Animations.h"

void IdleState::Enter(CSimon* simon)
{
	simon->vx = 0;
	simon->isSitting = false;
	simon->isAttacking = false;
}

void IdleState::HandleInput(CSimon* simon, BYTE* states)
{
	CGame* game = CGame::GetInstance();

	// ... (Phần logic không đổi)
	if (game->IsKeyDown(DIK_RIGHT)) {
		simon->SetState(new WalkState(simon, 1));
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		simon->SetState(new WalkState(simon, -1));
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		simon->SetState(new SitState());
	}
	else if (game->IsKeyDown(DIK_SPACE) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 0));
		return;
	}
	else if (game->IsKeyDown(DIK_X) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 1));
		return;
	}
	else if (game->IsKeyDown(DIK_UP)) {
		simon->SetState(new JumpState());
	}
}

void IdleState::Update(CSimon* simon, DWORD dt)
{
	// Sử dụng hằng số từ CSimon.h
	simon->vy += CSimon::GRAVITY * dt;
	simon->y += simon->vy * dt;

	if (simon->y > CSimon::GROUND_Y)
	{
		simon->y = CSimon::GROUND_Y;
		simon->vy = 0;
	}
	simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void IdleState::Render(CSimon* simon)
{
	int aniId;
	// Sử dụng enum class AnimationID
	if (simon->nx > 0)
		aniId = static_cast<int>(AnimationID::SimonIdleRight);
	else
		aniId = static_cast<int>(AnimationID::SimonIdleLeft);

	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}