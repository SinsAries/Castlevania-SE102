// SpriteRendererComponent.cpp

#include "SpriteRendererComponent.h"
#include "GameObject.h"
#include "debug.h"

void SpriteRendererComponent::Render()
{
    // Nếu không hiển thị hoặc không có owner, không làm gì cả
    if (!this->isVisible || owner == nullptr)
    {
        return;
    }

    // Lấy thông tin vị trí và hướng từ owner
    float x, y;
    owner->GetPosition(x, y);
    int nx = owner->getNx();

    // Yêu cầu animation tự vẽ tại vị trí của owner
    if (this->animation != nullptr)
    {
        //DebugOut(L"Owner x y: %f, %f\n", x, y);
        this->animation->Render(x, y);
    }
}

void SpriteRendererComponent::Update(DWORD dt)
{
    // Hiện tại không cần làm gì trong Update của renderer
    // Nhưng vẫn phải có để lớp không bị trừu tượng
}