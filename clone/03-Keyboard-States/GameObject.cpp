#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "IMoverController.h"
#include "RigidBodyComponent.h"

CGameObject::CGameObject()
{
	x = y = 0;
	nx = 1;
	state = -1;
	isDeleted = false;
}

CGameObject::CGameObject(float x, float y) : CGameObject()
{
	this->x = x;
	this->y = y;
}

CGameObject::~CGameObject()
{
	for (auto& comp : components)
	{
		delete comp;
	}
	components.clear();
}

void CGameObject::AddComponent(IComponent* comp)
{
	if (find(components.begin(), components.end(), comp) == components.end())
	{
		comp->owner = this;
		components.push_back(comp);
	}
}

// Trong CGameObject.cpp

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Lấy con trỏ đến các component đặc biệt trước
	IMoverController* controller = this->GetComponent<IMoverController>();
	RigidBodyComponent* rbody = this->GetComponent<RigidBodyComponent>();

	// 1. CHẠY CONTROLLER TRƯỚC
	if (controller != nullptr)
	{
		controller->Update(dt);
	}

	// 2. CHẠY RIGIDBODY TIẾP THEO
	if (rbody != nullptr)
	{
		rbody->Update(dt, coObjects);
	}

	// 3. CHẠY VÒNG LẶP CÁC COMPONENT CÒN LẠI
	for (auto& comp : components)
	{
		if (comp != controller && comp != rbody)
		{
			comp->Update(dt);
		}
	}
}

void CGameObject::Render()
{
	// Tìm component renderer
	SpriteRendererComponent* renderer = GetComponent<SpriteRendererComponent>();

	// Nếu có component renderer, yêu cầu nó tự vẽ
	if (renderer != nullptr)
	{
		renderer->Render();
	}
}

float CGameObject::GetWidth()
{
	// Tìm đến ColliderComponent
	ColliderComponent* collider = GetComponent<ColliderComponent>();

	// Nếu tìm thấy component này
	if (collider != nullptr)
	{
		// Trả về chiều rộng của nó
		return collider->GetWidth();
	}

	// Nếu GameObject không có ColliderComponent, trả về 0 để an toàn
	return 0;
}

float CGameObject::GetHeight()
{
	// Tương tự, tìm đến ColliderComponent
	ColliderComponent* collider = GetComponent<ColliderComponent>();

	if (collider != nullptr)
	{
		// Trả về chiều cao của nó
		return collider->GetHeight();
	}

	return 0;
}

void CGameObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// Giả sử bạn có một hàm để lấy component, ví dụ tên là GetComponent<T>()
	ColliderComponent* collider = GetComponent<ColliderComponent>();

	if (collider != nullptr)
	{
		// Ủy quyền việc tính toán cho component
		collider->GetBoundingBox(left, top, right, bottom);
	}
	else
	{
		// Nếu không có collider, trả về một bounding box rỗng tại vị trí của object
		// để tránh lỗi
		left = right = this->x;
		top = bottom = this->y;
	}
}

// Trong CGameObject.cpp
bool CGameObject::IsVisible()
{
	SpriteRendererComponent* renderer = GetComponent<SpriteRendererComponent>();
	if (renderer != nullptr)
	{
		return renderer->IsVisible();
	}

	// Nếu một object không có renderer, ta mặc định nó là "vô hình"
	return false;
}