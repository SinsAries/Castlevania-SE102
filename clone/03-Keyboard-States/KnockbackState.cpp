// KnockbackState.cpp

#include "KnockbackState.h"
#include "IdleState.h"
#include "Simon.h"
#include "Animations.h"
#include "RigidBodyComponent.h"
#include "SpriteRendererComponent.h"

KnockbackState::KnockbackState(float nx)
{
    this->knockback_dir_x = nx;
}

void KnockbackState::Enter(CSimon* simon)
{
    // Lấy component vật lý
    auto rbody = simon->GetComponent<RigidBodyComponent>();
    if (rbody)
    {
        // Áp dụng lực văng ngược ra và hơi nảy lên
        float knockback_vx = this->knockback_dir_x * CSimon::PVP_KNOCKBACK_SPEED_X;
        rbody->SetSpeed(knockback_vx, CSimon::PVP_KNOCKBACK_SPEED_Y);

        rbody->SetGrounded(false);
    }

    // Bắt đầu đếm thời gian cooldown để không bị va chạm liên tục
    simon->pvpCollisionCooldownTimer = CSimon::PVP_COLLISION_COOLDOWN_MS;
}

void KnockbackState::HandleInput(CSimon* simon, BYTE* states)
{
    // QUAN TRỌNG: Bỏ trống hàm này.
    // Khi đang ở trạng thái bị văng, Simon không nhận input từ người chơi.
}

void KnockbackState::Update(CSimon* simon, DWORD dt)
{
    // Lấy component vật lý để kiểm tra xem Simon đã chạm đất chưa
    auto rbody = simon->GetComponent<RigidBodyComponent>();
    if (rbody && rbody->IsGrounded())
    {
        // Nếu đã chạm đất, trạng thái văng lùi kết thúc. Chuyển về Idle.
        simon->SetState(new IdleState());
    }
}

void KnockbackState::Render(CSimon* simon)
{
    // Dùng animation ngồi/nhảy để mô phỏng trạng thái bị thương/mất thăng bằng
    auto renderer = simon->GetComponent<SpriteRendererComponent>();
    if (renderer == nullptr) return;

    int aniId;
    if (simon->getNx() > 0)
        aniId = static_cast<int>(AnimationID::SimonSitRight); // Hoặc một animation "bị thương" riêng
    else
        aniId = static_cast<int>(AnimationID::SimonSitLeft);

    renderer->SetAnimation(CAnimations::GetInstance()->Get(aniId));
}