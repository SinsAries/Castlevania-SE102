#pragma once

#include "SimonState.h"
#include "Simon.h"
#include "Whip.h"

class AttackState : public ISimonState
{
private:
    int attackTime;
    CWhip* whip;
public:
    AttackState();
    ~AttackState();

    void Enter(CSimon* simon) override;
    void HandleInput(CSimon* simon, BYTE* states) override;
    void Update(CSimon* simon, DWORD dt) override;
    void Render(CSimon* simon) override;
    const wchar_t* GetStateName() override { return L"AttackState"; }
};

