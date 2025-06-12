#pragma once

#include "SimonState.h"
#include "Simon.h"

class IdleState : public ISimonState
{
public:
	void Enter(CSimon* simon) override;
	void HandleInput(CSimon* simon, BYTE* states) override;
	void Update(CSimon* simon, DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render(CSimon* simon) override;
	const wchar_t* GetStateName() override { return L"IdleState"; }
};

