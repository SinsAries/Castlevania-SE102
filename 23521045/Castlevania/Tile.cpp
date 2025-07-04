// Tile.cpp
#include "Tile.h"
#include "SpriteRendererComponent.h" 
#include "Sprites.h"                
#include "Animations.h"             

// Sửa lại hàm khởi tạo để tạo và gắn component
CTile::CTile(float x, float y, int left, int top, int right, int bottom, LPTEXTURE tex) : CGameObject(x, y)
{
    LPSPRITE tile_sprite = new CSprite(-1, left, top, right, bottom, tex);
    LPANIMATION tile_animation = new CAnimation(100);
    tile_animation->Add(tile_sprite, 100);

    SpriteRendererComponent* renderer = new SpriteRendererComponent(tile_animation);

    this->AddComponent(renderer);
}
void CTile::Render()
{
    CGameObject::Render();
}

void CTile::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    float tile_width = 16.0f;
    float tile_height = 16.0f;
    l = x;
    t = y;
    r = x + tile_width;
    b = y + tile_height;
}