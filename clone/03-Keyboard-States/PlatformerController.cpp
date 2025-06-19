// PlatformerController.cpp
#include "PlatformerController.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"

PlatformerController::PlatformerController(float gravity) {
    this->gravity = gravity;
    this->rbody = nullptr;
}

// Khi component này được gắn vào một GameObject, hàm này sẽ được gọi.
// Ta dùng nó để tự động lấy tham chiếu đến RigidBodyComponent.
void PlatformerController::SetOwner(CGameObject* owner) {
    IComponent::SetOwner(owner); // Gọi hàm của lớp cha
    if (owner) {
        this->rbody = owner->GetComponent<RigidBodyComponent>();
    }
}

// Trong PlatformerController.cpp

void PlatformerController::Update(DWORD dt)
{
    // "Lazy Initialization": Nếu chưa có con trỏ rbody, hãy thử lấy nó.
    if (rbody == nullptr)
    {
        // owner đã được gán khi AddComponent
        if (owner != nullptr)
        {
            rbody = owner->GetComponent<RigidBodyComponent>();
        }
    }

    // Sau khi đã thử lấy, nếu vẫn không có rbody thì mới thoát.
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