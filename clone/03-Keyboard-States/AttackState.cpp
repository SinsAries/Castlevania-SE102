#include "IdleState.h"
#include "Game.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"
#include "Whip.h"
#include "Knife.h"

AttackState::AttackState() {
	attackTime = 900;
	whip = new CWhip(0, 0, 1);
}

AttackState::AttackState(CSimon* simon, int type) {
	attackTime = 900;
	if (type == 0)
		whip = new CWhip(0, 0, 1);
	else
		whip = new CKnife(simon->x + 10, simon->y - 10, 1);
}

AttackState::~AttackState() {
	delete whip;
}

void AttackState::Enter(CSimon* simon)
{
	attackTime = 900;
	simon->isAttacking = true;
	simon->vx = 0;
	whip->SetDirection(simon->nx);

	int aniId = simon->isSitting ?
		(simon->nx > 0 ? ID_ANI_SIMON_SIT_ATTACK_RIGHT : ID_ANI_SIMON_SIT_ATTACK_LEFT) :
		(simon->nx > 0 ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT);

	CAnimations::GetInstance()->Get(aniId)->Reset();
}

void AttackState::HandleInput(CSimon* simon, BYTE* states)
{
	// không nhận keyboard khi đang tấn công
}

void AttackState::Update(CSimon* simon, DWORD dt)
{
	attackTime -= dt;

	// Tính frame hiện tại
	float progress = (900.0f - attackTime) / 900.0f;
	int frame = min((int)(progress * 3), 2);

	// Xác định ID Animation dựa vào trạng thái Simon
	int aniId = simon->isSitting ?
		(simon->nx > 0 ? ID_ANI_SIMON_SIT_ATTACK_RIGHT : ID_ANI_SIMON_SIT_ATTACK_LEFT) :
		(simon->nx > 0 ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT);

	whip->UpdatePosition(simon->x, simon->y, aniId, frame, dt);

	bool check = attackTime > 0;
    if (attackTime <= 0) {
        simon->isAttacking = false;
		simon->SetState(new IdleState());
		simon->attackCoolDown = 150;
		return;
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
	int aniId = simon->isSitting ?
		(simon->nx > 0 ? ID_ANI_SIMON_SIT_ATTACK_RIGHT : ID_ANI_SIMON_SIT_ATTACK_LEFT) :
		(simon->nx > 0 ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT);

	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);

	whip->Render();
}