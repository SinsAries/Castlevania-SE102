#include "AttackState.h"
#include "IdleState.h"
#include "Simon.h"      // Quan trọng: Thêm Simon.h để lấy hằng số
#include "Animations.h"
#include "Whip.h"
#include "Knife.h"

// --- Hàm hỗ trợ để tránh lặp code ---
int GetAttackAnimationId(CSimon* simon)
{
	if (simon->isSitting) {
		return (simon->getNx() > 0)
			? static_cast<int>(AnimationID::SimonSitAttackRight)
			: static_cast<int>(AnimationID::SimonSitAttackLeft);
	}
	else {
		return (simon->getNx() > 0)
			? static_cast<int>(AnimationID::SimonStandAttackRight)
			: static_cast<int>(AnimationID::SimonStandAttackLeft);
	}
}
// ------------------------------------


AttackState::AttackState() {
	// Sử dụng hằng số từ CSimon]
	weapon = new CWhip(0, 0, 1);
	attackTime = CSimon::ATTACK_TIME_MS;
}

AttackState::AttackState(CSimon* simon, int type) {
	// Sử dụng hằng số từ CSimon
	attackTime = CSimon::ATTACK_TIME_MS;
	if (type == 0)
		weapon = new CWhip(0, 0, 1);
	else
		weapon = new CKnife(simon->x + 10, simon->y - 10, 1);
}

AttackState::~AttackState() {
	delete weapon;
}

void AttackState::Enter(CSimon* simon)
{
	attackTime = CSimon::ATTACK_TIME_MS;
	simon->isAttacking = true;
	simon->vx = 0;
	weapon->SetDirection(simon->getNx());

	// Sử dụng hàm hỗ trợ
	int aniId = GetAttackAnimationId(simon);
	CAnimations::GetInstance()->Get(aniId)->Reset();
}

void AttackState::HandleInput(CSimon* simon, BYTE* states)
{
	// không nhận keyboard khi đang tấn công
}

void AttackState::Update(CSimon* simon, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	attackTime -= dt;

	// Tính frame hiện tại
	float progress = (float)(CSimon::ATTACK_TIME_MS - attackTime) / CSimon::ATTACK_TIME_MS;
	int frame = min((int)(progress * 3), 2); // Giả sử animation có 3 frame

	// Sử dụng hàm hỗ trợ
	int aniId = GetAttackAnimationId(simon);

	weapon->UpdatePosition(simon->x, simon->y, aniId, frame, dt);

	if (attackTime <= 0) {
		simon->isAttacking = false;
		simon->SetState(new IdleState());
		simon->attackCoolDown = CSimon::ATTACK_COOLDOWN_MS; // Sử dụng hằng số
		return;
	}

	simon->vy += CSimon::SIMON_GRAVITY * dt;

	CCollision::GetInstance()->Process(simon, dt, coObjects);

	if (simon->isOnPlatform) {
		//simon->y = GROUND_Y;
		simon->vy = 0;
	}
}

void AttackState::Render(CSimon* simon)
{
	// Sử dụng hàm hỗ trợ
	int aniId = GetAttackAnimationId(simon);
	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);

	weapon->Render();
}