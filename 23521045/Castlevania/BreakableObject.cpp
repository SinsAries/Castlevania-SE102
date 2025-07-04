// GameObject/BreakableObject.cpp

#include "BreakableObject.h"
#include "Animations.h"
#include "Animation.h"

CBreakableObject::CBreakableObject(
	float x, float y,
	int idleAniId, int breakAniId,
	ItemType itemType, DWORD breakTime) : CGameObject(x, y)
{
	this->idleAnimationId = idleAniId;
	this->breakAnimationId = breakAniId;
	this->itemToDrop = itemType;
	this->breakTimeDuration = breakTime;

	this->isBroken = false;
	this->breakTimeStart = 0;

	// Thêm cờ isDeleted cho CGameObject để quản lý việc hủy đối tượng
	// this->isDeleted = false; 
}

void CBreakableObject::OnAttacked()
{
	if (isBroken) return; // Nếu đã vỡ thì không xử lý nữa

	isBroken = true;
	breakTimeStart = GetTickCount64();

	// Reset animation vỡ để nó chạy từ đầu
	CAnimations::GetInstance()->Get(breakAnimationId)->Reset();
}

void CBreakableObject::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (isBroken)
	{
		animations->Get(breakAnimationId)->Render(x, y);
	}
	else
	{
		animations->Get(idleAnimationId)->Render(x, y);
	}
}