#include "Simon.h"
#include "IdleState.h"

CSimon::CSimon(float x, float y) : CGameObject(x, y) {
	// Khởi tạo trạng thái ban đầu
	currentState = new IdleState();
	currentState->Enter(this);

	// Khởi tạo các giá trị mặc định
	vx = vy = 0;
	nx = 1;
	isSitting = false;
	isAttacking = false;
	attackCoolDown = 0;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"[INFO] Current state: %s\n", currentState->GetStateName());
	isOnPlatform = false;
	currentState->Update(this, dt, coObjects);
}


void CSimon::Render()
{
	currentState->Render(this);
}


void CSimon::SetState(ISimonState* state)
{
	if (currentState) delete currentState;
	currentState = state;
	currentState->Enter(this);
}

void CSimon::KeyState(BYTE* states)
{
	currentState->HandleInput(this, states);
}


void CSimon::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
}

void CSimon::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	// Implementation of GetBoundingBox method
	left = x;
	top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}

void CSimon::OnCollisionWith(LPCOLLISIONEVENT e)
{
	// Handle collision with Simon
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}
}