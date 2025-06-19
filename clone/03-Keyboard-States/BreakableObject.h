// GameObject/BreakableObject.h

#pragma once

#include "GameObject.h"
#include "GameIDs.h" // Để lấy ItemType và AnimationID

class CBreakableObject : public CGameObject
{
protected:
	int idleAnimationId;
	int breakAnimationId;
	ItemType itemToDrop;

	bool isBroken;
	ULONGLONG breakTimeStart; // Thời điểm bắt đầu vỡ
	DWORD breakTimeDuration;  // Thời gian tồn tại của animation vỡ

public:
	// Constructor nhận vào đầy đủ thông tin để các lớp con có thể tùy biến
	CBreakableObject(
		float x, float y,
		int idleAniId, int breakAniId,
		ItemType itemType, DWORD breakTime = 300
	);

	virtual void Render() override;

	// Hàm được gọi khi bị vũ khí của Simon tấn công
	void OnAttacked();

	bool IsBroken() const { return isBroken; }
	ItemType GetItemToDrop() const { return itemToDrop; }
};