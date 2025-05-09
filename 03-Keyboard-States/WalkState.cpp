#include "IdleState.h"
#include "Game.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"

WalkState::WalkState(CSimon* simon, int direction) {
	nx = direction;
	simon->nx = direction;
}

void WalkState::Enter(CSimon* simon)
{
	simon->vx = nx > 0 ? SIMON_WALKING_SPEED : -SIMON_WALKING_SPEED;
	simon->isSitting = false;
	//DebugOut(L"Cuong ngu nhu con cu: %f\n", simon->vx);
}

void WalkState::HandleInput(CSimon* simon, BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (game->IsKeyDown(DIK_SPACE) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 0));
		return;
	}

	if (game->IsKeyDown(DIK_X) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 1));
		return;
	}

	if (game->IsKeyDown(DIK_B) && simon->attackCoolDown <= 0)
	{
		simon->SetState(new AttackState(simon, 2));
		return;
	}

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

	if (game->IsKeyDown(DIK_DOWN))
	{
		simon->SetState(new SitState());
		return;
	}

	if (!game->IsKeyDown(DIK_LEFT) && simon->vx < 0)
	{
		simon->SetState(new IdleState());
		return;
	}

	if (!game->IsKeyDown(DIK_RIGHT) && simon->vx > 0)
	{
		simon->SetState(new IdleState());
		return;
	}

	if (!(game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_RIGHT)))
	{
		simon->SetState(new IdleState());
		return;
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		simon->SetState(new WalkState(simon, 1));
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		simon->SetState(new WalkState(simon, -1));
	}
}

void WalkState::Update(CSimon* simon, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	simon->vy += SIMON_GRAVITY * dt;

	CCollision::GetInstance()->Process(simon, dt, coObjects);

	simon->y += simon->vy * dt;

	simon->x += simon->vx * dt;

	if (simon->isOnPlatform)
	{
		//simon->y = GROUND_Y;
		simon->vy = 0;
	}
	if (simon->x < 0) simon->x = 0;
	if (simon->x > 990) simon->x = 990;
	simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void WalkState::Render(CSimon* simon)
{
	int aniId = (nx > 0) ? ID_ANI_SIMON_WALKING_RIGHT : ID_ANI_SIMON_WALKING_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}