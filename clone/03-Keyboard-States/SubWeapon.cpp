#include "SubWeapon.h"
#include "Game.h"
#include "ColliderComponent.h"
#include "Enemy.h" // Giả sử bạn có lớp Enemy

CSubWeapon::CSubWeapon(float x, float y) : CGameObject(x, y)
{
	this->damage = 1; // Sát thương mặc định
}

void CSubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Tự động gọi Update của các component
	CGameObject::Update(dt, coObjects);

	// Tự hủy khi bay ra khỏi Camera
	Camera* cam = CGame::GetInstance()->GetCamera();
	float cam_l, cam_t, cam_r, cam_b;
	cam->GetBoundingBox(cam_l, cam_t, cam_r, cam_b);

	if (this->x < cam_l || this->x > cam_r || this->y < cam_t || this->y > cam_b)
	{
		this->Delete();
	}
}

void CSubWeapon::OnCollision(LPCOLLISIONEVENT e)
{
	// Nếu va chạm với Enemy, gây sát thương
	if (dynamic_cast<CEnemy*>(e->obj))
	{
		// Logic gây sát thương cho Enemy
		// e->obj->TakeDamage(this->damage);
	}

	// Nếu va chạm với vật thể cứng (không phải trigger) thì tự hủy
	ColliderComponent* otherCollider = e->obj->GetComponent<ColliderComponent>();
	if (otherCollider && !otherCollider->IsTrigger())
	{
		this->Delete();
	}
}