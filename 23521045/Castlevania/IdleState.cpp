#include "IdleState.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"
#include "Simon.h"      // Quan trọng: Thêm Simon.h
#include "Game.h"
#include "Animations.h"
#include "RigidBodyComponent.h"       // <<< THÊM
#include "SpriteRendererComponent.h" // <<< THÊM
#include "ColliderComponent.h"

void IdleState::Enter(CSimon* simon)
{
	// Ra lệnh cho RigidBody dừng di chuyển ngang
	auto rbody = simon->GetComponent<RigidBodyComponent>();
	if (rbody)
	{
		//float current_vy = 0;
		//rbody->GetSpeed(current_vy, current_vy); // Lấy vy hiện tại để không ảnh hưởng đến việc rơi
		rbody->SetSpeed(0, 0);          // Set vx = 0
	}

	auto collider = simon->GetComponent<ColliderComponent>();
	if (collider) {
		collider->SetSize(CSimon::SIMON_STANDING_BBOX_WIDTH, (CSimon::SIMON_STANDING_BBOX_HEIGHT));
	}

	simon->isSitting = false;
	simon->isAttacking = false;
}

void IdleState::HandleInput(CSimon* simon, BYTE* states)
{
	CGame* game = CGame::GetInstance();

	// ... (Phần logic không đổi)
	if (game->IsKeyDown(simon->keyMappings.right)) {
		simon->SetState(new WalkState(simon, 1));
	}
	else if (game->IsKeyDown(simon->keyMappings.left)) {
		simon->SetState(new WalkState(simon, -1));
	}
	else if (game->IsKeyDown(simon->keyMappings.down)) {
		simon->SetState(new SitState());
	}
	else if (game->IsKeyDown(simon->keyMappings.attack) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(false));
		return;
	}
	else if (game->IsKeyDown(simon->keyMappings.subweapon) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(true));
		return;
	}
	else if (game->IsKeyDown(simon->keyMappings.jump)) {
		simon->SetState(new JumpState());
	}
}

void IdleState::Update(CSimon* simon, DWORD dt)
{
	simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void IdleState::Render(CSimon* simon)
{
	// Lấy renderer của Simon
	auto renderer = simon->GetComponent<SpriteRendererComponent>();
	if (renderer == nullptr) return;

	// Ra lệnh cho renderer phải dùng animation nào
	int aniId;
	if (simon->getNx() > 0)
		aniId = static_cast<int>(AnimationID::SimonIdleRight);
	else
		aniId = static_cast<int>(AnimationID::SimonIdleLeft);

	// Gán animation cho renderer, việc vẽ sẽ do PlayScene đảm nhiệm
	renderer->SetAnimation(CAnimations::GetInstance()->Get(aniId));
}