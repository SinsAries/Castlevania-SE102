#pragma once
#include "IComponent.h"
#include "Collision.h"
#include "GameObject.h" // Cần để dùng CCollisionEvent, LPGAMEOBJECT

class RigidBodyComponent : public IComponent
{
protected:
    float vx, vy;
    // Có thể thêm các thuộc tính vật lý khác ở đây: mass, gravity_scale...
    bool grounded = false;
public:
    RigidBodyComponent();

    void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }
    void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

    // Update sẽ chứa toàn bộ logic di chuyển và va chạm
    virtual void Update(DWORD dt) override;
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render() override { /* Có thể để trống */ };
    void   SetGrounded(bool g) { grounded = g; }
    bool   IsGrounded() const { return grounded; }
    float GetVy() { return vy; }
    float GetVx() { return vx; }
private:
    LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO, DWORD dt);
    void CalcPotentialCollisions(const vector<LPGAMEOBJECT>& coObjects, vector<LPCOLLISIONEVENT>& coEvents, DWORD dt);
    void FilterCollision(
        vector<LPCOLLISIONEVENT>& coEvents,
        vector<LPCOLLISIONEVENT>& coEventsResult,
        float& min_tx,
        float& min_ty,
        float& nx,
        float& ny,
        float& rdx,
        float& rdy);
};