#include "IdleState.h"
#include "Game.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"

void AttackState::Enter(CSimon* simon)
{
	attackTime = 900;
	simon->isAttacking = true;
	simon->vx = 0;

	int aniId = (simon->nx > 0) ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Reset();
}

void AttackState::HandleInput(CSimon* simon, BYTE* states)
{
	// không nhận keyboard khi đang tấn công
}

void AttackState::Update(CSimon* simon, DWORD dt)
{
    attackTime -= dt;
    if (attackTime <= 0) {
        simon->isAttacking = false;
		simon->SetState(new IdleState());
    }

    simon->vy += SIMON_GRAVITY * dt;
    simon->y += simon->vy * dt;

    if (simon->y > GROUND_Y) {
        simon->y = GROUND_Y;
        simon->vy = 0;
    }
}

void AttackState::Render(CSimon* simon)
{
	int aniId = (simon->nx > 0) ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}