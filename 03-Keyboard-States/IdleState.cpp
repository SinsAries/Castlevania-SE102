#include "IdleState.h"
#include "Game.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"


void IdleState::Enter(CSimon* simon)
{
    simon->vx = 0;
    simon->isSitting = false;
    simon->isAttacking = false;
}

void IdleState::HandleInput(CSimon* simon, BYTE* states)
{
    //DebugOut(L"[INFO] Current attack cooldown: %d\n", simon->attackCoolDown);

    CGame* game = CGame::GetInstance();

    if (game->IsKeyDown(DIK_UP)) {
        if (game->IsKeyDown(DIK_RIGHT)) {
            simon->SetState(new JumpState(simon, 1));
            return;
        }
        else
            if (game->IsKeyDown(DIK_LEFT)) {
                simon->SetState(new JumpState(simon, -1));
                return;
            }
        simon->SetState(new JumpState(simon, 0));
        return;
    }

    if (game->IsKeyDown(DIK_RIGHT)) {
        simon->SetState(new WalkState(simon, 1));
    }
    else if (game->IsKeyDown(DIK_LEFT)) {
        simon->SetState(new WalkState(simon, -1));
    }
    else if (game->IsKeyDown(DIK_DOWN)) {
        simon->SetState(new SitState());
    }
    else if (game->IsKeyDown(DIK_SPACE) && simon->attackCoolDown <= 0)
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
}

void IdleState::Update(CSimon* simon, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    simon->vy += SIMON_GRAVITY * dt;

    CCollision::GetInstance()->Process(simon, dt, coObjects);

    simon->y += simon->vy * dt;

    if (simon->isOnPlatform) {
        //simon->y = GROUND_Y;
        simon->vy = 0;
    }
    simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void IdleState::Render(CSimon* simon)
{
    int aniId = (simon->nx > 0) ? ID_ANI_SIMON_IDLE_RIGHT : ID_ANI_SIMON_IDLE_LEFT;
    CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}