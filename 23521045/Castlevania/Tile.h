#pragma once
#include "Texture.h" // <--- THAY ĐỔI: Include "Texture.h" (định nghĩa LPTEXTURE) thay vì "Textures.h" (lớp quản lý)
#include "Game.h"
#include "GameObject.h"

class CTile : public CGameObject
{
public:
    // Sửa lại hàm khởi tạo để nhận vào texture
    CTile(float x, float y, int left, int top, int right, int bottom, LPTEXTURE tex); // <--- SỬA Ở ĐÂY
    virtual void Render() override;
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};
typedef CTile* LPTILE;