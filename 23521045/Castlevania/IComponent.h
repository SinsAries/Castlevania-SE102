#pragma once

#include <Windows.h>

// Forward declaration để tránh include vòng lặp
class CGameObject;

class IComponent
{
	friend class CGameObject; // Để CGameObject có thể truy cập vào SetOwner
protected:
	// Mỗi component cần một con trỏ để biết nó thuộc về GameObject nào
	CGameObject* owner;

public:
	IComponent() : owner(nullptr) {}

	// Hàm này sẽ được gọi khi component được thêm vào GameObject
	virtual void SetOwner(CGameObject* owner) { this->owner = owner; }

	// Các hàm vòng đời của component
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;

	// Destructor ảo là bắt buộc cho lớp cơ sở có hàm ảo
	virtual ~IComponent() {}
};