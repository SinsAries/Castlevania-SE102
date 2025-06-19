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
	// Đối với một collider tĩnh, hàm Update thường không cần làm gì cả.
	// Logic va chạm sẽ được xử lý bởi RigidBodyComponent.
}

// Hàm này vẽ bounding box để debug, rất hữu ích để trực quan hóa va chạm
void ColliderComponent::Render()
{
	// Bạn có thể tạo một biến cờ global để bật/tắt chế độ debug này
	// if (!DEBUG_MODE) return;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	// Đoạn code vẽ này tương tự với code D3D9 cũ, bạn có thể cần điều chỉnh
	// cho phù hợp với hàm Draw D3D10 của mình.
	// Mục đích là vẽ một hình chữ nhật tại vị trí (l,t) với kích thước (r-l, b-t)
	// LPTEXTURE bbox_tex = CTextures::GetInstance()->Get(ID_TEX_BBOX);
	// CGame::GetInstance()->Draw(l, t, bbox_tex);
}

// Hàm quan trọng nhất: tính toán bounding box trong World Space
void ColliderComponent::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!owner) return;

	// Lấy vị trí TÂM của GameObject owner
	float owner_x, owner_y;
	owner->GetPosition(owner_x, owner_y);

	// Tính toán vị trí TÂM của Collider
	float collider_center_x = owner_x + this->offsetX;
	float collider_center_y = owner_y + this->offsetY;

	// Tính toán các cạnh của bounding box
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

	// Lấy bounding box của đối tượng kia (của nến)
	float other_l, other_t, other_r, other_b;
	other->GetBoundingBox(other_l, other_t, other_r, other_b);

	// Nếu bounding box không có kích thước thì không thể va chạm
	if (r < l || b < t || other_r < other_l || other_b < other_t) {
		return false;
	}

	// Công thức kiểm tra AABB kinh điển
	return (r >= other_l && l <= other_r && b >= other_t && t <= other_b);
}

void ColliderComponent::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
}