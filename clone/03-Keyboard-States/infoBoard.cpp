#include <iomanip>
#include <sstream>

#include "infoBoard.h"
#include "font.h"
#include "Sprites.h"
#include "GameIDs.h"

std::string FormatNumber(int num, int width) {
	std::stringstream ss;
	ss << std::setw(width) << std::setfill('0') << num;
	return ss.str();
}

CInfoBoard::CInfoBoard()
{
	score = 0;
	time = 400;
	stage = 1;
	playerHealth = HEALTH_BAR_TICKS;
	enemyHealth = HEALTH_BAR_TICKS;
	playerLives = 3;

	font_renderer = CFont::GetInstance();

	black_background = CSprites::GetInstance()->Get(static_cast<int>(SpriteID::UI_Black_Background));
	health_tick_red = CSprites::GetInstance()->Get(static_cast<int>(SpriteID::UI_Health_Red));
	health_tick_white = CSprites::GetInstance()->Get(static_cast<int>(SpriteID::UI_Health_White));
	heart_icon = CSprites::GetInstance()->Get(static_cast<int>(SpriteID::UI_Heart));
}

CInfoBoard::~CInfoBoard() {}

void CInfoBoard::Update(int score, int time, int stage, int playerHealth, int enemyHealth, int lives)
{
	this->score = score;
	this->time = time;
	this->stage = stage;
	this->playerHealth = playerHealth;
	this->enemyHealth = enemyHealth;
	this->playerLives = lives;
}

void CInfoBoard::Render()
{
	// Lấy vị trí camera để vẽ UI tương đối theo nó
	CGame* game = CGame::GetInstance();
	float cam_x, cam_y;
	game->GetCamera()->GetPosition(cam_x, cam_y);

	// Lấy kích thước màn hình để căn lề
	int screen_width = game->GetBackBufferWidth();
	// int screen_height = game->GetBackBufferHeight(); // Dùng nếu cần căn theo chiều dọc

	// 1. Vẽ nền đen
	// Vẽ một sprite 1x1 được scale ra toàn bộ chiều rộng màn hình và cao 40 pixel
	if (black_background)
		black_background->Draw(cam_x, cam_y);

	// 2. Vẽ các dòng text
	// Các giá trị 8, 14, 24 là khoảng cách lề (padding) so với góc trên-trái của màn hình
	float text_base_x = cam_x + 8;
	float text_row1_y = cam_y + 18;
	float text_row2_y = cam_y + 36;
	float text_row3_y = cam_y + 54;
	float text_row4_y = cam_y + 72;

	// Dòng 1: SCORE và TIME
	std::string score_str = "SCORE-" + FormatNumber(score, 6);
	std::string time_str = "TIME-" + FormatNumber(time, 4);
	font_renderer->Draw(text_base_x, text_row1_y, score_str);
	font_renderer->Draw(text_base_x + 200, text_row1_y, time_str); // Cách score 120px

	// Dòng 1: STAGE (Căn lề phải)
	std::string stage_str = "STAGE-" + FormatNumber(1, 2);
	float stage_text_width = stage_str.length() * 15; // Giả sử mỗi ký tự rộng 8px
	float stage_x = cam_x + screen_width - stage_text_width - 8; // Căn lề phải 8px
	font_renderer->Draw(stage_x, text_row1_y, stage_str);

	// Dòng 2 & 3: PLAYER, ENEMY và thanh máu
	font_renderer->Draw(text_base_x, text_row2_y, "PLAYER");
	font_renderer->Draw(text_base_x, text_row3_y, "ENEMY");

	float health_x_start = text_base_x + 90; // Vị trí bắt đầu của thanh máu
	for (int i = 0; i < HEALTH_BAR_TICKS; i++)
	{
		float tick_x = health_x_start + i * 8;
		int tick_width = 4;
		int tick_height = 8;

		// Máu người chơi
		if (i < this->playerHealth) {
			health_tick_red->Draw(tick_x, text_row2_y);
			health_tick_red->Draw(tick_x + 2, text_row2_y);
			health_tick_red->Draw(tick_x + 4, text_row2_y);
		}
		else {
			health_tick_white->Draw(tick_x, text_row2_y);
			health_tick_white->Draw(tick_x + 2, text_row2_y);
			health_tick_white->Draw(tick_x + 4, text_row2_y);
		}

		// Máu kẻ địch
		if (i < this->enemyHealth) {
			health_tick_red->Draw(tick_x, text_row3_y);
			health_tick_red->Draw(tick_x + 2, text_row3_y);
			health_tick_red->Draw(tick_x + 4, text_row3_y);
		}
		else {
			health_tick_white->Draw(tick_x, text_row3_y);
			health_tick_white->Draw(tick_x + 2, text_row3_y);
			health_tick_white->Draw(tick_x + 4, text_row3_y);
		}
	}

	// 4. Vẽ mạng và sub-weapon
	heart_icon->Draw(cam_x + 335, text_row2_y);
	font_renderer->Draw(cam_x + 350, text_row2_y, "-");
	font_renderer->Draw(cam_x + 365, text_row2_y, FormatNumber(score, 2)); // Vẽ số mạng
	font_renderer->Draw(cam_x + 335, text_row3_y, "P-" + FormatNumber(playerLives, 2));

	// Tương tự, vẽ ô sub-weapon ở đây
	// Ví dụ: subweapon_box->Draw(cam_x + 240, cam_y + 14);
}