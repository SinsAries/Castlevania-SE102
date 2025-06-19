// Whip.cpp - PHIÊN BẢN MỚI, GỌN GÀNG HƠN

#include "Whip.h"
#include "Simon.h" // Cần include đầy đủ ở đây để lấy thông tin
#include "SpriteRendererComponent.h"
#include "ColliderComponent.h"
#include "Animations.h"
#include "debug.h"
#include "Candle.h"
#include "Utils.h"

CWhip::CWhip() : CGameObject()
{
    this->AddComponent(new SpriteRendererComponent(new CAnimation(100)));
    this->AddComponent(new ColliderComponent(0, 0));
    this->Reset(); // Gọi Reset để ẩn đi ngay từ đầu
}

void CWhip::Reset()
{
    hit_objects.clear();
    GetComponent<SpriteRendererComponent>()->Hide();
    GetComponent<ColliderComponent>()->SetSize(0, 0);
}

// HÀM TẤT CẢ TRONG MỘT
void CWhip::ProcessAttack(CSimon* owner, const vector<LPGAMEOBJECT>& targets)
{
    if (!owner) return;

    // --- Phần 1: Lấy thông tin cần thiết từ Simon (owner) ---
    this->nx = owner->getNx();
    int simonAniId = GetAttackAnimationId(owner); // Dùng lại hàm helper từ AttackState
    LPANIMATION simonAttackAni = CAnimations::GetInstance()->Get(simonAniId);
    if (!simonAttackAni) return;

    GetComponent<SpriteRendererComponent>()->Show();
    auto renderer = GetComponent<SpriteRendererComponent>();
    auto collider = GetComponent<ColliderComponent>();
    if (!renderer || !collider) return;

    // --- Phần 2: Đồng bộ Animation và Vị trí của Roi ---
    int whipAniId = (nx > 0) ? static_cast<int>(AnimationID::WeaponWhipRight) : static_cast<int>(AnimationID::WeaponWhipLeft);
    LPANIMATION whipAnimation = CAnimations::GetInstance()->Get(whipAniId);
    renderer->SetAnimation(whipAnimation);
    if (!whipAnimation) return;

    whipAnimation->SetCurrentFrame(simonAttackAni->GetCurrentFrame());

    int simonHandAnchorX, simonHandAnchorY;
    float simon_x, simon_y;
    owner->GetPosition(simon_x, simon_y);
    simonAttackAni->GetCurrentFrameAnchorPoint(simonHandAnchorX, simonHandAnchorY);
	float simonWidth = simonAttackAni->GetCurrentFrameWidth();
	float simonHeight = simonAttackAni->GetCurrentFrameHeight();
    float handX = simon_x - simonWidth/2.0f + simonHandAnchorX;
    float handY = simon_y - simonHeight/2.0f + simonHandAnchorY;

    int whipHandleAnchorX, whipHandleAnchorY;
    whipAnimation->GetCurrentFrameAnchorPoint(whipHandleAnchorX, whipHandleAnchorY);
    float whipWidth = whipAnimation->GetCurrentFrameWidth();
    float whipHeight = whipAnimation->GetCurrentFrameHeight();
    this->SetPosition(handX - whipHandleAnchorX + whipWidth/2.0f, handY - whipHandleAnchorY + whipHeight/2.0f);


    // --- Phần 3: Cập nhật Collider và Kiểm tra Va chạm ---
    if (whipAnimation->IsFinished()) // Chỉ kích hoạt ở frame cuối
    {
        collider->SetSize(whipWidth, whipHeight);
    }
    else
    {
        collider->SetSize(0, 0);
        return; // Không phải frame tấn công, không cần kiểm tra va chạm
    }

    for (auto& target : targets)
    {
        if (std::find(hit_objects.begin(), hit_objects.end(), target) != hit_objects.end()) {
            continue;
        }

        if (collider->CheckAABB(target))
        {
            // SỬA LỖI: Cần dynamic_cast để gọi hàm OnHit() an toàn
            if (CCandle* candle = dynamic_cast<CCandle*>(target))
            {
                candle->OnHit();
            }
            // else if (CEnemy* enemy = dynamic_cast<CEnemy*>(target)) { enemy->TakeDamage(...) }

            hit_objects.push_back(target);
        }
    }
}