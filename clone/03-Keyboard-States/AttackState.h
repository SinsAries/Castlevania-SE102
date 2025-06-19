#pragma once

#include "SimonState.h"
#include "Simon.h"
#include "Weapon.h"
#include "Whip.h"

class AttackState : public ISimonState
{
private:
    int attackTime;
    bool isSubWeaponAttack;  // Cờ để xác định đây là đòn đánh roi hay ném vũ khí phụ
    bool hasSpawnedWeapon;
    CWhip* whip;
public:
    AttackState();
    AttackState(bool isSubWeaponAttack);
    virtual ~AttackState() {}

    void Enter(CSimon* simon) override;
    void HandleInput(CSimon* simon, BYTE* states) override;
    void Update(CSimon* simon, DWORD dt) override;
    void Render(CSimon* simon) override;
    const wchar_t* GetStateName() override { return L"AttackState"; }
    int GetAttackAnimationId(CSimon* simon);
};

