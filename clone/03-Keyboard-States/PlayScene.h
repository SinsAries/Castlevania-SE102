#pragma once

#include <string>
#include <vector>

#include "Scene.h"
#include "GameObject.h"
#include "Simon.h"
#include "Quadtree.h"
#include "TiledBackground.h"
#include "InfoBoard.h"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)

class CPlayScene : public CScene
{
protected:
	// Đối tượng người chơi chính
	CSimon* player;

	// Background của màn chơi
	CTiledBackground* background;

	// Danh sách tất cả các đối tượng trong scene
	// (trừ những đối tượng tĩnh được quản lý bởi Quadtree)
	std::vector<LPGAMEOBJECT> objects;

	// Quadtree để quản lý các đối tượng tĩnh (gạch, nền đất...)
	// giúp tối ưu hóa việc xét va chạm
	Quad* quadtree;

	// Đường dẫn đến file dữ liệu của scene (ví dụ: scene_main.json)
	std::string sceneFilePath;

	CInfoBoard* board;

	int score;
	int game_time;
	int stage_number;

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

	static LPCWSTR StringToLPCWSTR(const std::string& str)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		wchar_t* wide_str = new wchar_t[size];
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide_str, size);
		return wide_str;
	}

	// Hàm tiện ích để lấy con trỏ đến đối tượng người chơi
	CSimon* GetPlayer() { return player; }
};

// Định nghĩa một kiểu con trỏ cho dễ sử dụng
typedef CPlayScene* LPPLAYSCENE;