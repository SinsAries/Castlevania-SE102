#include <iomanip>
#include <sstream>

#include "Simon.h"
#include "PlayScene.h"
#include "InfoBoard.h"
#include "font.h"       // Cần để dùng CFont
#include "Game.h"       // Cần để lấy Camera và các thông tin game
#include "GameIDs.h"    // Cần để lấy SpriteID

// Hàm tiện ích để format số (ví dụ: 5 -> "05", 100 -> "000100")
std::string FormatNumber(int num, int width) {
	std::stringstream ss;
	ss << std::setw(width) << std::setfill('0') << num;
	return ss.str();
}


// --- HÀM KHỞI TẠO MỚI ---
CInfoBoard::CInfoBoard(CSimon* player, CPlayScene* scene)
{
	this->player = player;
	this->scene = scene;

	// Lấy các tài nguyên sprite cần thiết từ singleton
	CSprites* sprites = CSprites::GetInstance();
	this->black_background = sprites->Get(static_cast<int>(SpriteID::UI_Black_Background));
	this->health_tick_red = sprites->Get(static_cast<int>(SpriteID::UI_Health_Red));
	this->health_tick_white = sprites->Get(static_cast<int>(SpriteID::UI_Health_White));
	this->heart_icon = sprites->Get(static_cast<int>(SpriteID::UI_Heart));

	// Sử dụng singleton Font để vẽ chữ
	this->font_renderer = CFont::GetInstance();
}

CInfoBoard::~CInfoBoard()
{
	// Không cần delete font_renderer vì nó là singleton
}

// Trong infoBoard.cpp

void CInfoBoard::Render()
{
	// Nếu không có player hoặc scene thì không vẽ gì cả
	if (player == nullptr || scene == nullptr) return;

	// --- BƯỚC 1: "KÉO" (PULL) DỮ LIỆU MỚI NHẤT ---
	int score = player->GetScore();
	int time = scene->GetTime();
	int stage = scene->GetStageNumber();
	int playerHealth = player->GetHealth();
	int playerMaxHealth = player->GetMaxHealth();
	int enemyHealth = 16; // Tạm thời hardcode máu của enemy
	int enemyMaxHealth = 16;
	int lives = player->GetLives();
	int hearts = player->GetHearts();

	// --- BƯỚC 2: VẼ GIAO DIỆN VỚI TỌA ĐỘ ĐÃ CĂN CHỈNH ---
	CGame* game = CGame::GetInstance();
	float cam_x, cam_y;
	game->GetCamera()->GetPosition(cam_x, cam_y);

	// Vẽ nền đen cho UI
	if (black_background)
		black_background->Draw(cam_x, cam_y);

	// --- Tinh chỉnh lại tọa độ và bố cục ---
	// Các giá trị này được căn chỉnh để giống với game gốc hơn
	// Bạn có thể tự do thay đổi các con số này để đạt độ chính xác pixel
	float text_row1_y = cam_y + 7;
	float text_row2_y = cam_y + 23;
	float text_row3_y = cam_y + 39;

	float score_x = cam_x + 16;
	float time_x = cam_x + 224;
	float stage_x = cam_x + 400;

	float label_x = cam_x + 16;
	float health_bar_x = cam_x + 122;

	float right_hud_x = cam_x + 300;

	// Dòng 1: SCORE, TIME, STAGE
	font_renderer->Draw(score_x, text_row1_y, "SCORE-" + FormatNumber(score, 6));
	font_renderer->Draw(time_x, text_row1_y, "TIME-" + FormatNumber(time, 4));
	font_renderer->Draw(stage_x, text_row1_y, "STAGE-" + FormatNumber(stage, 2));

	// Dòng 2: PLAYER và thanh máu
	font_renderer->Draw(label_x, text_row2_y, "PLAYER");
	RenderHealthBar(health_bar_x, text_row2_y, playerHealth, playerMaxHealth);

	// Dòng 3: ENEMY và thanh máu
	font_renderer->Draw(label_x, text_row3_y, "ENEMY");
	RenderHealthBar(health_bar_x, text_row3_y, enemyHealth, enemyMaxHealth); // Tạm thời hardcode máu enemy

	// Cụm thông tin bên phải
	if (heart_icon) heart_icon->Draw(right_hud_x, text_row2_y);
	font_renderer->Draw(right_hud_x + 24, text_row2_y, "-" + FormatNumber(hearts, 2));
	font_renderer->Draw(right_hud_x, text_row3_y, "P-" + FormatNumber(lives, 2));

	// Bạn có thể vẽ ô vũ khí phụ ở đây
	// ví dụ: subweapon_box_sprite->Draw(right_hud_x + 90, text_row2_y);
}

// --- HÀM TIỆN ÍCH ĐỂ VẼ THANH MÁU ---
void CInfoBoard::RenderHealthBar(float x, float y, int currentHealth, int maxHealth)
{
	for (int i = 0; i < maxHealth; i++)
	{
		// Mỗi vạch máu rộng 5px, cách nhau 1px
		float tick_x = x + i * (5 + 1);

		if (i < currentHealth)
		{
			if (health_tick_red) health_tick_red->Draw(tick_x, y);
		}
		else
		{
			if (health_tick_white) health_tick_white->Draw(tick_x, y);
		}
	}
} // <--- DẤU NGOẶC NHỌN "}" BỊ THIẾU CỦA BẠN NẰM Ở ĐÂY

// Hàm này bạn có thể cài đặt sau
void CInfoBoard::RenderSubWeaponIcon(float x, float y)
{
	// ...
}