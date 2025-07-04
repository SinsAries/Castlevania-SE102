	#pragma once
	#include "IComponent.h"
	#include "GameObject.h"

	// Forward declaration
	class CGameObject;

	class ColliderComponent : public IComponent
	{
	protected:
		float width, height;      // Kích thước của collider
		float offsetX, offsetY;   // Độ lệch của tâm collider so với tâm của GameObject
		bool isTrigger;
	public:
		// Constructor nhận vào kích thước và độ lệch tùy chọn
		ColliderComponent(float width, float height, float offsetX = 0, float offsetY = 0, bool isTrigger = false);

		// Update thường có thể để trống đối với collider tĩnh
		virtual void Update(DWORD dt) override;

		// Render sẽ được dùng để vẽ bounding box khi debug
		virtual void Render() override;

		// === Các hàm chức năng chính ===

		// Lấy ra hình chữ nhật bao quanh ở tọa độ thế giới (World Space)
		void GetBoundingBox(float& left, float& top, float& right, float& bottom);

		// Các hàm tiện ích để lấy kích thước
		float GetWidth() const { return this->width; }
		float GetHeight() const { return this->height; }
		bool IsTrigger() const { return isTrigger; }

		bool CheckAABB(LPGAMEOBJECT other);

		// --- THÊM HÀM NÀY ---
		/**
		 * @brief Thiết lập lại kích thước cho collider.
		 */
		void SetSize(float width, float height);
	};