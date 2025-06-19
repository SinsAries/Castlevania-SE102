#include "Item.h"
#include "Animations.h"
#include "SpriteRendererComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "Utils.h"

CItem::CItem(float x, float y, ItemType type) : CGameObject(x, y)
{
	this->itemType = type;
	this->disappear_start = GetTickCount64();

	this->AddComponent(new RigidBodyComponent());
	this->AddComponent(new ColliderComponent(30, 30, 0, 0, true));

	// --- LOGIC MỚI GỌN GÀNG HƠN ---

	LPANIMATION item_anim = nullptr;

	// Tìm animation ID tương ứng trong "từ điển"
	auto it = ITEM_ANIMATION_MAP.find(type);

	if (it != ITEM_ANIMATION_MAP.end())
	{
		// Nếu tìm thấy, lấy ra AnimationID
		AnimationID animId = it->second;
		item_anim = CAnimations::GetInstance()->Get(static_cast<int>(animId));
	}

	//item_anim = CAnimations::GetInstance()->Get(10100);

	if (item_anim != nullptr)
	{
		this->AddComponent(new SpriteRendererComponent(item_anim));
	}
	else
	{
		AnimationID animId = it->second;
		DebugOut(L"id: %d\n", static_cast<int>(animId));
		DebugOut(L"[ERROR] CItem: Animation mapping not found for ItemType: %d\n", static_cast<int>(type));
	}
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Luôn gọi Update của lớp cha để các component (như RigidBody) được cập nhật
	CGameObject::Update(dt, coObjects);
	// --- Xử lý logic vòng đời của Item ---

	// 1. Kiểm tra nếu đã hết thời gian tồn tại
	if (GetTickCount64() - disappear_start > ITEM_LIFETIME)
	{
		this->Delete(); // Đánh dấu để xóa khỏi scene
		return;
	}

	// 2. Xử lý hiệu ứng nhấp nháy trước khi biến mất
	if (GetTickCount64() - disappear_start > ITEM_LIFETIME - ITEM_FLICKER_TIME)
	{
		SpriteRendererComponent* renderer = this->GetComponent<SpriteRendererComponent>();
		if (renderer != nullptr)
		{
			// Cứ mỗi 150ms thì đảo ngược trạng thái hiển thị
			bool is_currently_visible = (int)((GetTickCount64() - disappear_start) / 150) % 2 == 0;

			if (is_currently_visible)
				renderer->Show();
			else
				renderer->Hide();
		}
	}
}

void CItem::OnCollected()
{
	// Khi được nhặt, đơn giản là đánh dấu để xóa khỏi game
	this->Delete();
}