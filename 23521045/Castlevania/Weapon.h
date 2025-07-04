#pragma once
#include "GameObject.h"
#include "GameIDs.h"

// Forward declaration để tránh lỗi include vòng lặp
class CSimon;

class CWeapon : public CGameObject
{
protected:
    bool isActive;
    int nx; // Hướng của vũ khí

    // Thêm một con trỏ tới chủ sở hữu, rất hữu ích cho các logic phức tạp sau này
    CSimon* owner;

public:
    // Constructor giờ đây nhận vào con trỏ tới chủ sở hữu
    CWeapon(CSimon* owner);

    // --- Chữ ký hàm Update MỚI, SẠCH SẼ HƠN ---
    // Vũ khí chỉ cần nhận vào vị trí điểm gắn (bàn tay) của chủ sở hữu.
    // Nó sẽ tự quyết định vị trí của mình dựa vào điểm này.
    virtual void Update(DWORD dt, float owner_attach_point_x, float owner_attach_point_y) = 0;

    virtual void Render() = 0;

    // Xóa hàm UpdatePosition vì nó không còn cần thiết nữa
    // virtual void UpdatePosition(...) = 0; // <<< XÓA DÒNG NÀY

    bool IsActive() const { return isActive; }
    void SetInactive() { isActive = false; }
    void SetDirection(int direction) { nx = direction; }
};