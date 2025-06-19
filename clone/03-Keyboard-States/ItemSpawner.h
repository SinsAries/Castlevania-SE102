#pragma once

#include "GameIDs.h"

// Forward Declaration để tránh include vòng lặp, tăng tốc độ biên dịch
class CPlayScene;

/**
 * @brief Lớp quản lý Singleton chịu trách nhiệm tạo ra các vật phẩm.
 * Hoạt động như một "nhà máy" sản xuất item.
 */
class CItemSpawner
{
private:
	// Biến static để lưu trữ thể hiện duy nhất của lớp
	static CItemSpawner* __instance;

	// Con trỏ đến scene hiện tại để biết nơi "thả" item vào
	CPlayScene* currentScene;

	// Constructor là private để ngăn việc tạo đối tượng từ bên ngoài
	CItemSpawner();

public:
	/**
	 * @brief Lấy ra thể hiện duy nhất của ItemSpawner.
	 */
	static CItemSpawner* GetInstance();

	/**
	 * @brief Thiết lập scene hiện tại cho Spawner.
	 * Phải được gọi mỗi khi chuyển cảnh.
	 */
	void SetScene(CPlayScene* scene) { this->currentScene = scene; }

	/**
	 * @brief Hàm sản xuất chính: tạo ra một item cụ thể tại một vị trí.
	 */
	void SpawnItem(float x, float y, ItemType type);
};