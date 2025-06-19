// Tile.cpp
#include "Tile.h"
#include "SpriteRendererComponent.h" // THÊM INCLUDE NÀY
#include "Sprites.h"                 // Cần để tạo sprite
#include "Animations.h"              // Cần để tạo animation

// Sửa lại hàm khởi tạo để tạo và gắn component
CTile::CTile(float x, float y, int left, int top, int right, int bottom, LPTEXTURE tex) : CGameObject(x, y)
{
    // === BẮT ĐẦU TẠO SPRITE RENDERER COMPONENT ===

    // 1. Tạo một Sprite duy nhất cho tile này
    // Ta không cần đăng ký nó vào Sprites manager, chỉ cần tạo đối tượng là đủ
    // Dùng một ID động hoặc một ID âm để tránh trùng lặp nếu cần
    LPSPRITE tile_sprite = new CSprite(-1, left, top, right, bottom, tex);

    // 2. Tạo một Animation rỗng
    LPANIMATION tile_animation = new CAnimation(100);

    // 3. Thêm sprite vừa tạo vào animation như một frame duy nhất
    // Thời gian của frame này không quan trọng vì nó chỉ có 1 frame
    tile_animation->Add(tile_sprite, 100);

    // 4. Tạo SpriteRendererComponent với animation một frame này
    SpriteRendererComponent* renderer = new SpriteRendererComponent(tile_animation);

    // 5. Gắn component vừa tạo vào chính đối tượng Tile này
    this->AddComponent(renderer);

    // (Tùy chọn) Nếu tile của bạn có va chạm, bạn cũng có thể thêm ColliderComponent ở đây
    // float tile_width = right - left;
    // float tile_height = bottom - top;
    // this->AddComponent(new ColliderComponent(tile_width, tile_height));
}

// HÀM RENDER TRỐNG RỖNG!
void CTile::Render()
{
    // Không cần viết gì ở đây cả!
    // Lớp cha CGameObject::Render() sẽ tự động tìm và gọi renderer->Render() cho bạn.
    // Đây chính là sức mạnh của Component Pattern.
    CGameObject::Render();
}

void CTile::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    // Giả sử Tile có kích thước cố định, ví dụ 16x16
    float tile_width = 16.0f;
    float tile_height = 16.0f;
    l = x;
    t = y;
    r = x + tile_width;
    b = y + tile_height;
}