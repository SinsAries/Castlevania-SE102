// GameUtils.h

#pragma once

#include <string>
#include "GameIDs.h" // Rất quan trọng để có enum ItemType

// Khai báo hàm
ItemType ConvertStringToItemType(const std::string& itemStr);