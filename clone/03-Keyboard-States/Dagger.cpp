#include "Dagger.h"
#include "RigidBodyComponent.h"
#include "SpriteRendererComponent.h"
#include "ColliderComponent.h"
#include "GameIDs.h"

CDagger::CDagger(float x, float y, int nx) : CSubWeapon(x, y)
{
	this->nx = nx;

	// --- Lắp ráp các Component ---
	auto rbody = new RigidBodyComponent();
	rbody->SetSpeed(DAGGER_SPEED * this->nx, 0);
	// rbody->SetGravity(0.0f); // Không cần vì RigidBody mặc định không có trọng lực
	this->AddComponent(rbody);

	int aniId = (nx > 0) ? static_cast<int>(AnimationID::WeaponDaggerRight) : static_cast<int>(AnimationID::WeaponDaggerLeft);
	// ... SpriteRenderer và Collider giữ nguyên ...
	auto renderer = new SpriteRendererComponent(CAnimations::GetInstance()->Get(aniId));
	this->AddComponent(renderer);
	auto collider = new ColliderComponent(16, 16);
	this->AddComponent(collider);
}

// Bỏ hàm Update() và OnCollision() ở đây, vì đã được xử lý ở lớp cha CSubWeapon

void CDagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// Bounding box được quản lý bởi ColliderComponent
	this->GetComponent<ColliderComponent>()->GetBoundingBox(left, top, right, bottom);
}