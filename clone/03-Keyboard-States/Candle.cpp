#include "Candle.h"

// Include các thành phần cần thiết để "lắp ráp" nên một Candle
#include "Animations.h"
#include "SpriteRendererComponent.h"
#include "ColliderComponent.h"

// Include "nhà máy" để gửi yêu cầu spawn item
#include "ItemSpawner.h"
#include "RigidBodyComponent.h"

CCandle::CCandle(float x, float y, ItemType itemType) : CGameObject(x, y)
{
	// Lưu lại "đơn hàng" item
	this->itemType = itemType;

	// Trạng thái ban đầu
	this->state = CANDLE_STATE_ALIVE;
	this->destroy_start = 0;

	// --- Lắp ráp các Component cho Candle ---

	// 1. Component Va chạm (Collider)
	// Giúp roi của Simon biết đã đánh trúng nó
	this->AddComponent(new ColliderComponent(16, 32, 0, 0, true)); // Kích thước 16x32

	// 2. Component Hiển thị (SpriteRenderer)
	// Lấy animation lửa cháy từ kho animation đã load
	LPANIMATION flickering_anim = CAnimations::GetInstance()->Get(static_cast<int>(AnimationID::GroundCandle));
	this->AddComponent(new SpriteRendererComponent(flickering_anim));

	// QUAN TRỌNG: Candle là vật thể tĩnh, không di chuyển -> KHÔNG CẦN RigidBodyComponent
}

void CCandle::OnHit()
{
	// Nếu đã bị phá hủy rồi thì không xử lý nữa để tránh spawn nhiều item
	if (state == CANDLE_STATE_DESTROYED)
		return;

	// 1. Chuyển sang trạng thái bị phá hủy và bắt đầu đếm giờ
	this->state = CANDLE_STATE_DESTROYED;
	this->destroy_start = GetTickCount64();

	// 2. Thay đổi animation thành hiệu ứng "vỡ"
	SpriteRendererComponent* renderer = this->GetComponent<SpriteRendererComponent>();
	if (renderer != nullptr)
	{
		LPANIMATION break_anim = CAnimations::GetInstance()->Get(static_cast<int>(AnimationID::GroundCandle));
		renderer->SetAnimation(break_anim);
	}

	// 3. Gửi "đơn đặt hàng" đến nhà máy ItemSpawner
	CItemSpawner::GetInstance()->SpawnItem(this->x, this->y, this->itemType);
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Gọi Update của các component (nếu có)
	CGameObject::Update(dt, coObjects);

	// Chỉ xử lý logic khi đang ở trạng thái đã bị phá hủy
	if (this->state == CANDLE_STATE_DESTROYED)
	{
		// Kiểm tra xem thời gian hiệu ứng vỡ đã hết chưa
		if (GetTickCount64() - destroy_start > CANDLE_DESTROY_TIMEOUT)
		{
			// Nếu đã hết, đánh dấu để đối tượng này bị xóa khỏi scene
			this->Delete();
		}
	}
}