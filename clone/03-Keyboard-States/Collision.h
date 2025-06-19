#pragma once

// Forward declaration để báo cho CCollisionEvent biết CGameObject là một lớp tồn tại.
// Điều này giúp tránh phải #include "GameObject.h" và gây ra lỗi include vòng tròn.
class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

// Định nghĩa sự kiện va chạm
struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// Quãng đường di chuyển TƯƠNG ĐỐI giữa vật này và obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = nullptr)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	// Hàm so sánh để sắp xếp các sự kiện va chạm theo thời gian xảy ra (t)
	static bool compare(const LPCOLLISIONEVENT& a, const LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};