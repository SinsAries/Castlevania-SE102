#pragma once

#include <string>
#include <vector>

#include "Scene.h"
#include "GameObject.h"
#include "Simon.h"
#include "Quadtree.h"
#include "TiledBackground.h"
#include "InfoBoard.h"
#include "IComponent.h"
#include "Grid.h"

class CInfoBoard;

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)

class CPlayScene : public CScene
{
protected:
	// Đối tượng người chơi chính
	vector<CSimon*> players;
	CGrid* grid; // Dùng Grid thay cho Quadtree
	vector<LPGAMEOBJECT> dynamic_objects; // Danh sách các object động (Simon, enemy, item di chuyển)
	vector<LPGAMEOBJECT> static_objects; // Danh sách các object tĩnh (gạch, nến...) để quản lý bộ nhớ
	vector<LPGAMEOBJECT> tiles;

	// Đường dẫn đến file dữ liệu của scene (ví dụ: scene_main.json)
	std::string sceneFilePath;

	CInfoBoard* board;

	int score;
	int game_time;
	int stage_number;
	DWORD last_tick_count;
public:
	// Constructor: nhận vào một ID và đường dẫn đến file data
	CPlayScene(int id, std::string filePath);

	// Tải tài nguyên và các đối tượng cho scene từ file data
	virtual void Load() override;

	// Cập nhật trạng thái của tất cả các đối tượng trong scene
	virtual void Update(DWORD dt) override;

	// Vẽ tất cả các đối tượng trong scene lên màn hình
	virtual void Render() override;

	// Dọn dẹp tất cả tài nguyên, đối tượng đã được tạo trong scene
	virtual void Unload() override;

	// Hàm tiện ích để lấy con trỏ đến đối tượng người chơi
	CSimon* GetPlayer(size_t player_index = 0) {
		if (player_index >= 0 && player_index < players.size())
			return players[player_index];
		return nullptr;
	}

	const vector<LPGAMEOBJECT>& GetStaticObjects() const { return static_objects; }

	void AddObject(LPGAMEOBJECT obj, bool is_static = false);

	int GetTime() const { return game_time; }

	int GetStageNumber() const { return stage_number; }
};

// Định nghĩa một kiểu con trỏ cho dễ sử dụng
typedef CPlayScene* LPPLAYSCENE;