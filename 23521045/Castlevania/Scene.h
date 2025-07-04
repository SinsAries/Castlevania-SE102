#pragma once
#include "KeyEventHandler.h" // Dùng KeyEventHandler thay vì SampleKeyEventHandler

class CScene
{
protected:
	// Giữ một con trỏ đến bộ xử lý sự kiện phím chung
	CKeyEventHandler* key_handler;
	int sceneId;

public:
	CScene(int id) : sceneId(id), key_handler(nullptr) {} // Khởi tạo key_handler là nullptr

	int GetSceneId() { return sceneId; }

	// Trả về bộ xử lý sự kiện phím chung
	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};
typedef CScene* LPSCENE;