#include "IdleState.h"
#include "Game.h"
#include "Simon.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"

// Trong SitState.cpp
#include "RigidBodyComponent.h"       // <<< THÊM
#include "SpriteRendererComponent.h" // <<< THÊM
#include "ColliderComponent.h"

void SitState::Enter(CSimon* simon) {
    // Ra lệnh cho RigidBody dừng di chuyển ngang
    auto rbody = simon->GetComponent<RigidBodyComponent>();
    if (rbody)
    {
        float current_vx, current_vy;
        rbody->GetSpeed(current_vx, current_vy); // Lấy vy hiện tại
        rbody->SetSpeed(0, current_vy);          // Set vx = 0
    }
    simon->isSitting = true;

    // 3. Ra lệnh cho VA CHẠM thay đổi kích thước Bounding Box
    auto collider = simon->GetComponent<ColliderComponent>();
    if (collider) {
        collider->SetSize(CSimon::SIMON_SITTING_BBOX_WIDTH, CSimon::SIMON_SITTING_BBOX_HEIGHT);
    }
}

void SitState::HandleInput(CSimon* simon, BYTE* states) {
    CGame* game = CGame::GetInstance();

    if (game->IsKeyDown(simon->keyMappings.jump) && simon->attackCoolDown <= 0)
    {
        //simon->SetPosition(simon->x, simon->y - 7);
        simon->SetState(new AttackState(false));
        return;
    }
    else if (game->IsKeyDown(simon->keyMappings.subweapon) && simon->attackCoolDown <= 0)
    {
        //simon->SetPosition(simon->x, simon->y - 7);
        simon->SetState(new AttackState(true));
        return;
    }

    if (!game->IsKeyDown(simon->keyMappings.down)) {
        simon->SetPosition(simon->x, simon->y - 7);
        simon->SetState(new IdleState());
    }
}

// Trong SitState.cpp

void SitState::Update(CSimon* simon, DWORD dt) {
    simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void SitState::Render(CSimon* simon) {
    // Lấy renderer của Simon
    auto renderer = simon->GetComponent<SpriteRendererComponent>();
    if (renderer == nullptr) return;

    // Ra lệnh cho renderer phải dùng animation nào
    int aniId;
    if (simon->getNx() > 0)
        aniId = static_cast<int>(AnimationID::SimonSitRight);
    else
        aniId = static_cast<int>(AnimationID::SimonSitLeft);

    // Gán animation cho renderer. PlayScene sẽ lo việc vẽ.
    renderer->SetAnimation(CAnimations::GetInstance()->Get(aniId));
}