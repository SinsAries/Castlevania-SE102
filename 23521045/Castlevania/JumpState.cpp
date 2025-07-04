#include "IdleState.h"
#include "Game.h"
#include "Simon.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"
#include "RigidBodyComponent.h"       // <<< THÊM
#include "SpriteRendererComponent.h" // <<< THÊM

// JumpState.cpp

void JumpState::Enter(CSimon* simon)
{
    auto rbody = simon->GetComponent<RigidBodyComponent>();
    if (rbody == nullptr) return;

    // SỬA LẠI ĐIỀU KIỆN: Chỉ cho phép nhảy khi component báo đã chạm đất
    if (rbody->IsGrounded())
    {
        // Lệnh nhảy vẫn giữ nguyên, rất tốt!
        float current_vx, current_vy;
        rbody->GetSpeed(current_vx, current_vy);
        rbody->SetSpeed(current_vx, -CSimon::JUMP_SPEED_Y);
        simon->isSitting = false; // Đảm bảo không ở trạng thái ngồi khi nhảy
        rbody->SetGrounded(false);
    }
    else
    {
        // Nếu đang ở trên không mà vào state này (ví dụ do bị đẩy lùi và rơi xuống)
        // thì không làm gì cả, cứ để nó rơi tự nhiên.
        // Ta không muốn có thể "double jump" hoặc nhảy trên không.
    }
}

void JumpState::HandleInput(CSimon* simon, BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (game->IsKeyDown(simon->keyMappings.jump) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(false));
		return;
	}
	else if (game->IsKeyDown(simon->keyMappings.subweapon) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(true));
		return;
	}
}

// JumpState.cpp

void JumpState::Update(CSimon* simon, DWORD dt)
{
    auto rbody = simon->GetComponent<RigidBodyComponent>();
    if (rbody == nullptr) return;

    // THÊM DÒNG NÀY ĐỂ ĐIỀU TRA
    DebugOut(L"[JUMP STATE] Vy: %f, IsGrounded: %d\n", rbody->GetVy(), rbody->IsGrounded());

    // Kiểm tra điều kiện đáp đất để chuyển state
    // Chỉ cần kiểm tra IsGrounded là đủ. Khi component vật lý báo chạm đất,
    // tức là đã kết thúc trạng thái nhảy/rơi.
    if (rbody->IsGrounded())
    {
        // Để chắc chắn, ta có thể đặt lại vận tốc Y về 0
        float current_vx, current_vy;
        rbody->GetSpeed(current_vx, current_vy);
        rbody->SetSpeed(current_vx, 0);

        simon->SetState(new IdleState());
        return;
    }

    simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void JumpState::Render(CSimon* simon)
{
    // Lấy renderer của Simon
    auto renderer = simon->GetComponent<SpriteRendererComponent>();
    if (renderer == nullptr) return;

    // Ra lệnh cho renderer phải dùng animation nào
    int aniId;
    
    // Khi nhảy, Simon sẽ co người lại, dùng animation ngồi là hợp lý
    // Nếu bạn có animation nhảy riêng (ví dụ SimonJumpRight) thì dùng nó
    if (simon->getNx() > 0)
        aniId = static_cast<int>(AnimationID::SimonSitRight); // Hoặc SimonJumpRight
    else
        aniId = static_cast<int>(AnimationID::SimonSitLeft);  // Hoặc SimonJumpLeft

    // Gán animation cho renderer, việc vẽ sẽ do PlayScene đảm nhiệm
    renderer->SetAnimation(CAnimations::GetInstance()->Get(aniId));
}