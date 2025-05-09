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
	//if (weapon != NULL) delete weapon;
	weapon = new CWhip(0, 0, 1);
	attackTime = weapon->attackTime;
}

AttackState::AttackState(CSimon* simon, int type) {
	//if (weapon != NULL) delete weapon;
	if (type == 0)
		weapon = new CWhip(0, 0, 1);
	else if (type == 1)
		weapon = new CKnife(simon->x, simon->y, 1);
	attackTime = weapon->attackTime;
}

AttackState::~AttackState() {
	delete weapon;
}

void AttackState::Enter(CSimon* simon)
{
	attackTime = weapon->attackTime;
	simon->isAttacking = true;
	simon->vx = 0;
	weapon->SetDirection(simon->nx);

	int aniId = simon->isSitting ?
		(simon->nx > 0 ? ID_ANI_SIMON_SIT_ATTACK_RIGHT : ID_ANI_SIMON_SIT_ATTACK_LEFT) :
		(simon->nx > 0 ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT);

	CAnimations::GetInstance()->Get(aniId)->Reset();
}

void AttackState::HandleInput(CSimon* simon, BYTE* states)
{
	// không nhận keyboard khi đang tấn công
}

void AttackState::Update(CSimon* simon, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	attackTime -= dt;
	float Time = weapon->attackTime;
	// Tính frame hiện tại
	float progress = (Time - attackTime) / Time;
	int frame = min((int)(progress * 3), 2);

	// Xác định ID Animation dựa vào trạng thái Simon
	int aniId = simon->isSitting ?
		(simon->nx > 0 ? ID_ANI_SIMON_SIT_ATTACK_RIGHT : ID_ANI_SIMON_SIT_ATTACK_LEFT) :
		(simon->nx > 0 ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT);

	weapon->UpdatePosition(simon->x, simon->y, aniId, frame, dt);

	//DebugOut(L"Cuong ngu nhu con cu: %d\n", attackTime);

	bool check = attackTime > 0;
	if (attackTime <= 0) {
		simon->isAttacking = false;
		simon->SetState(new IdleState());
		simon->attackCoolDown = 150;
		return;
	}

	simon->vy += SIMON_GRAVITY * dt;

	CCollision::GetInstance()->Process(simon, dt, coObjects);

	simon->y += simon->vy * dt;

	if (simon->isOnPlatform) {
		//simon->y = GROUND_Y;
		simon->vy = 0;
	}
}

void AttackState::Render(CSimon* simon)
{
	int aniId = simon->isSitting ?
		(simon->nx > 0 ? ID_ANI_SIMON_SIT_ATTACK_RIGHT : ID_ANI_SIMON_SIT_ATTACK_LEFT) :
		(simon->nx > 0 ? ID_ANI_SIMON_STAND_ATTACK_RIGHT : ID_ANI_SIMON_STAND_ATTACK_LEFT);

	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);

	weapon->Render();
}