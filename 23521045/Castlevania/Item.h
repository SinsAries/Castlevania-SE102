#pragma once

#include "GameObject.h"
#include "GameIDs.h"

// Thời gian tồn tại của item trước khi tự biến mất (miliseconds)
static const int ITEM_LIFETIME = 5000;

// Item sẽ bắt đầu nhấp nháy trong khoảng thời gian cuối cùng này
static const int ITEM_FLICKER_TIME = 2000;

class CItem : public CGameObject
{
protected:
	ItemType itemType; // Lưu loại item để xử lý logic
	ULONGLONG disappear_start; // Thời điểm item được tạo ra, dùng để tính toán thời gian tồn tại

public:
	CItem(float x, float y, ItemType type);
	virtual ~CItem() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;

	void OnCollected();
	ItemType GetType() const { return this->itemType; }
};