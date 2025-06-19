// Brick.cpp

#include "Brick.h"
#include "SpriteRendererComponent.h"
#include "ColliderComponent.h"
#include "Animations.h"
#include "Sprites.h"
#include "GameIDs.h" // Nhớ include

CBrick::CBrick(float x, float y) : CGameObject(x, y)
{
    this->AddComponent(new ColliderComponent(32.0f, 32.0f));

    // Lấy ID của sprite gạch từ GameIDs.h một cách an toàn
    int spriteId = static_cast<int>(SpriteID::BRICK_GROUND_STYLE_1);

    LPANIMATION brick_anim = new CAnimation(100);
    brick_anim->Add(spriteId);

    // Thêm animation này vào kho animation chung
    // Bạn có thể tạo một AnimationID riêng cho gạch nếu muốn, ví dụ AnimationID::BrickNormal
    int brickAnimationId = 10002; // Một ID chưa dùng đến cho animation của gạch thường
    CAnimations::GetInstance()->Add(brickAnimationId, brick_anim);

    this->AddComponent(new SpriteRendererComponent(brick_anim));
}