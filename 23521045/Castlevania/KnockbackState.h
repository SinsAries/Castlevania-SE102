// KnockbackState.h

#pragma once
#include "SimonState.h"

class KnockbackState : public ISimonState
{
private:
    // Hướng bị đẩy lùi, sẽ là 1.0f (sang phải) hoặc -1.0f (sang trái)
    float knockback_dir_x;

public:
    KnockbackState(float nx);
    virtual ~KnockbackState() {}

    void Enter(CSimon* simon) override;
    void HandleInput(CSimon* simon, BYTE* states) override;
    void Update(CSimon* simon, DWORD dt) override;
    void Render(CSimon* simon) override;
    const wchar_t* GetStateName() override { return L"KnockbackState"; }
};