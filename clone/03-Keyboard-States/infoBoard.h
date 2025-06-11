#pragma once
#include <string>
#include "Sprite.h"

#define HEALTH_BAR_TICKS 16

class CFont; // Forward declaration

class CInfoBoard
{
	// Dữ liệu của game cần hiển thị
	int score;
	int time;
	int stage;
	int playerHealth;
	int enemyHealth;
	int playerLives;

	// Các tài nguyên để vẽ
	CFont* font_renderer;
	LPSPRITE black_background;
	LPSPRITE health_tick_red;
	LPSPRITE health_tick_white;
	LPSPRITE heart_icon;
	// Thêm các sprite cho subweapon...

public:
	CInfoBoard();
	~CInfoBoard();

	// Cập nhật thông tin từ game
	void Update(int score, int time, int stage, int playerHealth, int enemyHealth, int lives);

	// Vẽ bảng thông tin
	void Render();
};