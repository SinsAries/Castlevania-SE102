#include "IdleState.h"
#include "Game.h"
#include "Simon.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"

void SitState::Enter(CSimon* simon) {
    simon->vx = 0;
    simon->isSitting = true;
}

void SitState::HandleInput(CSimon* simon, BYTE* states) {
    CGame* game = CGame::GetInstance();

    if (game->IsKeyDown(DIK_SPACE) && simon->attackCoolDown <= 0)
    {
        simon->SetState(new AttackState(simon, 0));
        return;
    }
    else if (game->IsKeyDown(DIK_X) && simon->attackCoolDown <= 0)
    {
        simon->SetState(new AttackState(simon, 1));
        return;
    }

    if (!game->IsKeyDown(DIK_DOWN)) {
        simon->SetState(new IdleState());
    }
}

void SitState::Update(CSimon* simon, DWORD dt) {
    // Sử dụng hằng số từ CSimon.h
    simon->vy += CSimon::GRAVITY * dt;
    simon->y += simon->vy * dt;

    if (simon->y > CSimon::GROUND_Y) {
        simon->y = CSimon::GROUND_Y;
        simon->vy = 0;
    }
    simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void SitState::Render(CSimon* simon) {
    int aniId;
    // Sử dụng enum class AnimationID
    if (simon->nx > 0)
        aniId = static_cast<int>(AnimationID::SimonSitRight);
    else
        aniId = static_cast<int>(AnimationID::SimonSitLeft);

    CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}