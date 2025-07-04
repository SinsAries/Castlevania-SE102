#include "ColliderComponent.h"
#include "GameObject.h" // Cần để lấy vị trí của owner
#include "Game.h"       // Cần để truy cập các hàm vẽ debug

// Định nghĩa một ID cho texture debug (bạn cần load một texture hình chữ nhật trắng vào ID này)
#define ID_TEX_BBOX -100 

ColliderComponent::ColliderComponent(float width, float height, float offsetX, float offsetY, bool isTrigger)
{
	this->width = width;
	this->height = height;
	this->offsetX = offsetX;
	this->offsetY = offsetY;
	this->isTrigger = isTrigger; // Gán giá trị
}

void ColliderComponent::Update(DWORD dt)
{
}

// Hàm này vẽ bounding box để debug, rất hữu ích để trực quan hóa va chạm
void ColliderComponent::Render()
{

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
}

void ColliderComponent::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!owner) return;

	float owner_x, owner_y;
	owner->GetPosition(owner_x, owner_y);

	float collider_center_x = owner_x + this->offsetX;
	float collider_center_y = owner_y + this->offsetY;

	left = collider_center_x - this->width / 2;
	top = collider_center_y - this->height / 2;
	right = left + this->width;
	bottom = top + this->height;
}

bool ColliderComponent::CheckAABB(LPGAMEOBJECT other)
{
	if (other == nullptr || other->IsDeleted()) return false;

	// Lấy bounding box của chính mình (của roi)
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	float other_l, other_t, other_r, other_b;
	other->GetBoundingBox(other_l, other_t, other_r, other_b);

	// Nếu bounding box không có kích thước thì không thể va chạm
	if (r < l || b < t || other_r < other_l || other_b < other_t) {
		return false;
	}

	// Công thức kiểm tra AABB
	return (r >= other_l && l <= other_r && b >= other_t && t <= other_b);
}

void ColliderComponent::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
}