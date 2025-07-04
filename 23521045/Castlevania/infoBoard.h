#pragma once
#include "Sprite.h"

// --- KHAI BÁO TIỀN PHƯƠNG (FORWARD DECLARATION) ---
// "Hứa" với compiler rằng các lớp này tồn tại, không cần include đầy đủ.
class CSimon;
class CPlayScene;
class CFont;
// --------------------------------------------------

class CInfoBoard
{
private:
    CSimon* player;
    CPlayScene* scene;

    // Các tài nguyên để vẽ
    CFont* font_renderer;
    LPSPRITE black_background;
    LPSPRITE health_tick_red;
    LPSPRITE health_tick_white;
    LPSPRITE heart_icon;

public:
    // Hàm khởi tạo giờ sẽ nhận vào con trỏ
    CInfoBoard(CSimon* player, CPlayScene* scene);
    ~CInfoBoard();

    // Không cần hàm Update

    // Chỉ cần hàm Render
    void Render();

private:
    void RenderHealthBar(float x, float y, int currentHealth, int maxHealth);
    void RenderSubWeaponIcon(float x, float y);
};