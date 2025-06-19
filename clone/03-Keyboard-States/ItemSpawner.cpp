#include "ItemSpawner.h"
#include "Item.h"
#include "PlayScene.h"

// Khởi tạo thể hiện static là nullptr bên ngoài lớp
CItemSpawner* CItemSpawner::__instance = nullptr;

CItemSpawner* CItemSpawner::GetInstance()
{
	if (__instance == nullptr)
	{
		__instance = new CItemSpawner();
	}
	return __instance;
}

CItemSpawner::CItemSpawner()
{
	this->currentScene = nullptr;
}

void CItemSpawner::SpawnItem(float x, float y, ItemType type)
{
	// Luôn kiểm tra để đảm bảo Spawner đã được kết nối với một Scene
	if (this->currentScene == nullptr)
	{
		// Có thể ghi log debug ở đây để cảnh báo nếu cần
		return;
	}

	// 1. Tạo một đối tượng Item mới (Sản xuất sản phẩm)
	CItem* newItem = new CItem(x, y, type);

	// 2. Thêm đối tượng Item vừa tạo vào danh sách các đối tượng động của Scene (Thả sản phẩm vào thế giới)
	// Tham số thứ hai là 'false' vì Item là đối tượng động, không phải tĩnh.
	this->currentScene->AddObject(newItem, true);
}