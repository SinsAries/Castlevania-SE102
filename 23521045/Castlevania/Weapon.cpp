#include "Weapon.h"

// Constructor mới
CWeapon::CWeapon(CSimon* owner) : CGameObject()
{
    this->owner = owner;
    this->x = 0; // Vị trí sẽ được cập nhật trong hàm Update
    this->y = 0;
    this->nx = 1;
    this->isActive = true;
}