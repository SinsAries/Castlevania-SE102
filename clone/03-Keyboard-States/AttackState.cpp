#include "IdleState.h"
#include "Game.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"

AttackState::AttackState() {
	attackTime = 900;
	whip = new CWhip();
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

	whip->UpdatePosition(simon->x, simon->y, aniId, frame);

	bool check = attackTime > 0;
	DebugOut(L"[INFO] Current attack time: %d\n", attackTime);
	DebugOut(L"[INFO] Current attack time greater than 0: %d\n", check);
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