// PlatformerController.cpp
#include "PlatformerController.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"

PlatformerController::PlatformerController(float gravity) {
    this->gravity = gravity;
    this->rbody = nullptr;
}

void PlatformerController::SetOwner(CGameObject* owner) {
    IComponent::SetOwner(owner); 
    if (owner) {
        this->rbody = owner->GetComponent<RigidBodyComponent>();
    }
}

// Trong PlatformerController.cpp

void PlatformerController::Update(DWORD dt)
{
    if (rbody == nullptr)
    {
        // owner đã được gán khi AddComponent
        if (owner != nullptr)
        {
            rbody = owner->GetComponent<RigidBodyComponent>();
        }
    }
    if (rbody == nullptr)
    {
        return;
    }

    // --- LOGIC TRỌNG LỰC GIỮ NGUYÊN ---
    float vx, vy;
    rbody->GetSpeed(vx, vy);

    vy += gravity * dt;

    rbody->SetSpeed(vx, vy);
}