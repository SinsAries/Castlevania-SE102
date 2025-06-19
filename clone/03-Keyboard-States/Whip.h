// Whip.h - PHIÊN BẢN MỚI, GỌN GÀNG HƠN

#pragma once
#include "GameObject.h"
#include "GameIDs.h"

// Forward declaration để Whip biết Simon là ai mà không cần include đầy đủ
class CSimon;

class CWhip : public CGameObject
{
private:
    vector<LPGAMEOBJECT> hit_objects;

public:
    CWhip();
    virtual ~CWhip() {}

    /**
     * @brief Hàm duy nhất điều khiển toàn bộ hành vi của roi trong 1 frame tấn công.
     * @param owner Con trỏ đến Simon, để roi biết vị trí và trạng thái của chủ nhân.
     * @param targets Danh sách các đối tượng có thể bị roi đánh trúng.
     */
    void ProcessAttack(CSimon* owner, const vector<LPGAMEOBJECT>& targets);

    /**
     * @brief Reset roi về trạng thái ban đầu (ẩn, vô hại).
     */
    void Reset();
};