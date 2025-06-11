#include "IdleState.h"
#include "Game.h"
#include "Simon.h"
#include "Animations.h"
#include "WalkState.h"
#include "SitState.h"
#include "JumpState.h"
#include "AttackState.h"

void JumpState::Enter(CSimon* simon)
{
	if (simon->y == CSimon::GROUND_Y)
	{
		simon->vy = -CSimon::JUMP_SPEED_Y;
	}
	simon->isSitting = false;
}

void JumpState::HandleInput(CSimon* simon, BYTE* states)
{
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
}

void JumpState::Update(CSimon* simon, DWORD dt)
{
	simon->vy += CSimon::GRAVITY * dt;
	simon->y += simon->vy * dt;

	if (simon->y > CSimon::GROUND_Y)
	{
		simon->y = CSimon::GROUND_Y;
		simon->vy = 0;
		simon->SetState(new IdleState());
	}
	simon->attackCoolDown = max(0, simon->attackCoolDown - dt);
}

void JumpState::Render(CSimon* simon)
{
	int aniId;
	// Sử dụng enum class AnimationID
	// Lưu ý: animation ngồi và nhảy của bạn đang dùng chung sprite
	// nên tên ID có thể hơi khác, ở đây tôi dùng SimonJumpRight/Left
	// cho rõ ràng, bạn cần đảm bảo ID này có trong GameIDs.h
	if (simon->nx >= 0)
		aniId = static_cast<int>(AnimationID::SimonSitRight);
	else
		aniId = static_cast<int>(AnimationID::SimonSitLeft);

	CAnimations::GetInstance()->Get(aniId)->Render(simon->x, simon->y);
}