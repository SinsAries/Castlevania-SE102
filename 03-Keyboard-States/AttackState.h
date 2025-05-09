#pragma once

#include "SimonState.h"
#include "Simon.h"
#include "Weapon.h"

class AttackState : public ISimonState
{
private:
    float attackTime;
    CWeapon* weapon;
public:
    AttackState();
    AttackState(CSimon* simon, int type);
    ~AttackState();

    void Enter(CSimon* simon) override;
    void HandleInput(CSimon* simon, BYTE* states) override;
    void Update(CSimon* simon, DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render(CSimon* simon) override;
    const wchar_t* GetStateName() override { return L"AttackState"; }
};

