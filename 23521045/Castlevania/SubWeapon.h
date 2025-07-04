#pragma once
#include "GameObject.h"

class CSubWeapon : public CGameObject
{
protected:
	int damage;

public:
	CSubWeapon(float x, float y);
	virtual ~CSubWeapon() {}

	// Update chung cho tất cả subweapon
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;

	// Xử lý va chạm chung
	virtual void OnCollision(LPCOLLISIONEVENT e) override;

	// Hàm ảo để lấy BBox, vì mỗi vũ khí có kích thước riêng
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
};