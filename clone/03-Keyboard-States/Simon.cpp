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
}

void CSimon::Update(DWORD dt)
{
	currentState->Update(this, dt);
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