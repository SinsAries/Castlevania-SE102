#pragma once

#include "GameObject.h"
#include "GameIDs.h"

// --- Định nghĩa các hằng số cho Candle để code dễ đọc hơn ---

// Các trạng thái của Candle
static const int CANDLE_STATE_ALIVE = 100;
static const int CANDLE_STATE_DESTROYED = 200;

// Thời gian hiệu ứng vỡ tồn tại trước khi Candle biến mất hoàn toàn (miliseconds)
static const int CANDLE_DESTROY_TIMEOUT = 300;

/**
 * @brief Lớp Candle đại diện cho các cây nến có thể phá hủy để rơi ra item.
 */
class CCandle : public CGameObject
{
protected:
	// Loại item mà cây nến này sẽ "đặt hàng" Spawner tạo ra
	ItemType itemType;

	// Timer để tính toán khi nào nên xóa object sau khi hiệu ứng vỡ bắt đầu
	ULONGLONG destroy_start;

public:
	/**
	 * @brief Constructor cho Candle.
	 * @param x Vị trí x
	 * @param y Vị trí y
	 * @param itemType Loại item sẽ rơi ra khi bị phá hủy.
	 */
	CCandle(float x, float y, ItemType itemType);	
	virtual ~CCandle() {}

	/**
	 * @brief Hàm được gọi từ bên ngoài (thường là từ roi của Simon) khi nó bị đánh trúng.
	 * Kích hoạt logic phá hủy và yêu cầu spawn item.
	 */
	void OnHit();

	/**
	 * @brief Cập nhật trạng thái của Candle.
	 * Chỉ dùng để xử lý việc tự xóa sau khi hiệu ứng vỡ kết thúc.
	 */
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;

	// Lưu ý: Candle không cần override Render() hay GetBoundingBox()
	// vì các Component được thêm vào sẽ tự động xử lý các việc đó.
};