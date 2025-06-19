#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>
#include <algorithm> // Để dùng std::find
#include "IComponent.h"
#include "Collision.h"

using namespace std;

class CGameObject
{
	friend class Quad;

protected:
	float x;
	float y;

	int nx;

	int state;
	bool isDeleted = false; // Cờ để quản lý việc xóa đối tượng

	vector<IComponent*> components;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	int getNx() { return this->nx; }
	void SetNx(int nx) { this->nx = nx; }

	virtual void OnCollision(LPCOLLISIONEVENT e) {}

	void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }

	// Quản lý cờ xóa
	void Delete() { this->isDeleted = true; }
	bool IsDeleted() { return this->isDeleted; }

	CGameObject();
	CGameObject(float x, float y);

	// --- Quản lý Component ---
	void AddComponent(IComponent* comp);

	template<typename T>
	T* GetComponent() const
	{
		for (auto& comp : components) {
			T* target = dynamic_cast<T*>(comp);
			if (target != nullptr) {
				return target;
			}
		}
		return nullptr;
	}

	float GetWidth();
	float GetHeight();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool IsVisible();

	// --- Vòng lặp Game ---
	// Update và Render có phần thực thi cơ sở để gọi component
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	virtual void Render();

	virtual ~CGameObject();
};

typedef CGameObject* LPGAMEOBJECT;