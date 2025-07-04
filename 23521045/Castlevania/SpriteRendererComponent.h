// SpriteRendererComponent.h

#pragma once
#include "IComponent.h"
#include "Animations.h" // Cần để dùng LPANIMATION

class SpriteRendererComponent : public IComponent
{
protected:
    LPANIMATION animation; // Animation cần vẽ
    bool isVisible;

    // (Tùy chọn) Thêm các thuộc tính render khác
    // int alpha = 255;
    // D3DCOLOR color_tint = D3DCOLOR_ARGB(255, 255, 255, 255);

public:
    // Hàm khởi tạo nhận vào animation
    SpriteRendererComponent(LPANIMATION anim) : IComponent()
    {
        this->animation = anim;
        this->isVisible = true; // Mặc định là hiển thị
    }

    // Hàm Render sẽ chứa logic vẽ
    virtual void Render() override;
    virtual void Update(DWORD dt) override;

    // Các hàm để điều khiển trạng thái
    void Show() { this->isVisible = true; }
    void Hide() { this->isVisible = false; }
    bool IsVisible() const { return this->isVisible; }

    void SetAnimation(LPANIMATION anim) { this->animation = anim; }
    LPANIMATION GetAnimation() { return this->animation; }
};