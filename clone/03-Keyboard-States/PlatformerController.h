// PlatformerController.h
#pragma once
#include "IMoverController.h"

// Forward declaration để tránh include vòng lặp
class RigidBodyComponent;

class PlatformerController : public IMoverController
{
private:
    RigidBodyComponent* rbody; // Tham chiếu đến RigidBody của cùng một GameObject
    float gravity;

public:
    PlatformerController(float gravity = 0.002f);
    virtual ~PlatformerController() {}

    // Ghi đè hàm SetOwner để lấy tham chiếu đến RigidBody
    virtual void SetOwner(CGameObject* owner) override;

    // Ghi đè hàm Update để áp dụng trọng lực
    virtual void Update(DWORD dt) override;
};