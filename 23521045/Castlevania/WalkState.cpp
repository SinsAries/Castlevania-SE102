#include "IdleState.h"
#include "Simon.h"
#include "Game.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"
#include "RigidBodyComponent.h"       // <<< THÊM
#include "SpriteRendererComponent.h" // <<< THÊM
#include "ColliderComponent.h"

// Constructor chỉ lưu lại hướng đi
WalkState::WalkState(CSimon* simon, int direction) {
	this->nx = direction;
}

void WalkState::Enter(CSimon* simon)
{
	// Cập nhật hướng của Simon
	simon->SetNx(this->nx);

	//// Ra lệnh cho RigidBody di chuyển
	//auto rbody = simon->GetComponent<RigidBodyComponent>();
	//if (rbody)
	//{
	//	float current_vy = 0;
	//	rbody->GetSpeed(current_vy, current_vy); // Lấy vy để không ảnh hưởng việc rơi

	//	// Ra lệnh cho component di chuyển theo hướng và tốc độ đi bộ
	//	float move_speed = (this->nx > 0) ? CSimon::WALKING_SPEED : -CSimon::WALKING_SPEED;
	//	rbody->SetSpeed(move_speed, current_vy);
	//}

	simon->isSitting = false;
}
void WalkState::HandleInput(CSimon* simon, BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (game->IsKeyDown(simon->keyMappings.jump) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(false));
		return;
	} 
	if (game->IsKeyDown(simon->keyMappings.subweapon) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(true));
		return;
	}

	if (game->IsKeyDown(simon->keyMappings.up))
	{
		simon->SetState(new JumpState());
		return;
	}

	if (game->IsKeyDown(simon->keyMappings.down))
	{
		simon->SetState(new SitState());
		return;
	}

	if (!game->IsKeyDown(simon->keyMappings.left) && simon->GetComponent<RigidBodyComponent>()->GetVx() < 0)
	{
		simon->SetState(new IdleState());
		return;
	}

	if (!game->IsKeyDown(simon->keyMappings.right) && simon->GetComponent<RigidBodyComponent>()->GetVx() > 0)
	{
		simon->SetState(new IdleState());
		return;
	}

	if (!(game->IsKeyDown(simon->keyMappings.left) || game->IsKeyDown(simon->keyMappings.right)))
	{
		simon->SetState(new IdleState());
		return;
	}
}

// Trong WalkState.cpp
void WalkState::Update(CSimon* simon, DWORD dt)
{
	// Lấy component vật lý
	auto rbody = simon->GetComponent<RigidBodyComponent>();
	if (rbody == nullptr) return;

	// Lấy vận tốc dọc hiện tại để không ảnh hưởng đến việc nhảy/rơi
	float vx, vy;
	rbody->GetSpeed(vx, vy);

	// Xác định tốc độ di chuyển dựa trên hướng của state
	float move_speed = (this->nx > 0)
		? CSimon::WALKING_SPEED
		: -CSimon::WALKING_SPEED;

	// RA LỆNH LẠI VẬN TỐC MỖI FRAME
	// Kể cả khi va chạm ở frame trước set vx = 0, thì frame này nó sẽ được đặt lại
	// thành tốc độ đi bộ, giúp Simon có thể "trượt" dọc theo tường.
	rbody->SetSpeed(move_speed, vy);

	simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

// Trong WalkState.cpp
void WalkState::Render(CSimon* simon)
{
	auto renderer = simon->GetComponent<SpriteRendererComponent>();
	if (renderer == nullptr) return;

	int aniId;
	if (this->nx > 0)
		aniId = static_cast<int>(AnimationID::SimonWalkRight);
	else
		aniId = static_cast<int>(AnimationID::SimonWalkLeft);

	renderer->SetAnimation(CAnimations::GetInstance()->Get(aniId));
}