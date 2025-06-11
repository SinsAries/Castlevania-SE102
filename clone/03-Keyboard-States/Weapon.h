#pragma once
#include "GameObject.h"
#include "Simon.h"
#include "GameIDs.h"

class CWeapon : public CGameObject
{
protected:
    bool isActive;       // Trạng thái weapon có đang hoạt động không
    int nx;              // hướng của weapon (theo hướng Simon)
    DWORD activeTime;    // thời gian tồn tại của weapon
public:
    CWeapon(float x, float y, int nx) : CGameObject(x, y), nx(nx) {
        isActive = true;
        activeTime = 0;
    }

    virtual void Update(DWORD dt) = 0;
    virtual void Render() = 0;
    virtual void UpdatePosition(float simon_x, float simon_y, int aniId, int frame, DWORD dt) = 0;

    bool IsActive() const { return isActive; }
    void SetInactive() { isActive = false; }
    void SetDirection(int direction) { nx = direction; }
};
