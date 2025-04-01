#include "IdleState.h"
#include "Game.h"
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

    if (game->IsKeyDown(DIK_SPACE) && !simon->isAttacking) {
        simon->SetState(new AttackState());
        return;
    }

    if (!game->IsKeyDown(DIK_DOWN)) {
        simon->SetState(new IdleState());
    }
}

void SitState::Update(CSimon* simon, DWORD dt) {
    simon->vy += SIMON_GRAVITY * dt;
    simon->y += simon->vy * dt;

    if (simon->y > GROUND_Y) {
        simon->y = GROUND_Y;
        simon->vy = 0;
    }
}

void SitState::Render(CSimon* simon) {
    int aniId;

    if (simon->isAttacking) {
        aniId = (simon->nx > 0) ? ID_ANI_SIMON_SIT_ATTACK_RIGHT : ID_ANI_SIMON_SIT_ATTACK_LEFT;
    }
    else {
        aniId = (simon->nx > 0) ? ID_ANI_SIMON_SIT_RIGHT : ID_ANI_SIMON_SIT_LEFT;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}
