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
    else if (game->IsKeyDown(DIK_B) && simon->attackCoolDown <= 0)
    {
        simon->SetState(new AttackState(simon, 2));
        return;
    }

    if (!game->IsKeyDown(DIK_DOWN)) {
        simon->SetState(new IdleState());
    }
}

void SitState::Update(CSimon* simon, DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    simon->vy += SIMON_GRAVITY * dt;

    CCollision::GetInstance()->Process(simon, dt, coObjects);

    simon->y += simon->vy * dt;

    if (simon->isOnPlatform) {
        //simon->y = GROUND_Y;
        simon->vy = 0;
    }
    simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
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
