// GameUtils.cpp

#include "GameUtils.h"
#include <map>
#include "debug.h" // Để có thể báo lỗi nếu tên item không tồn tại

ItemType ConvertStringToItemType(const std::string& itemStr)
{
	// Khởi tạo một bản đồ tĩnh (static) để ánh xạ chuỗi sang enum.
	// Static giúp map này chỉ được tạo 1 lần duy nhất trong suốt chương trình.
	static const std::map<std::string, ItemType> itemMap = {
		{ "SmallHeart", ItemType::SmallHeart },
		{ "LargeHeart", ItemType::LargeHeart },
		{ "Money50", ItemType::Money50 },
		{ "Money100", ItemType::Money100 },
		{ "Money400", ItemType::Money400 },
		{ "Dagger", ItemType::Dagger },
		{ "Axe", ItemType::Axe },
		{ "HolyWater", ItemType::HolyWater },
		{ "Stopwatch", ItemType::Stopwatch },
		{ "Rosary", ItemType::Rosary },
		{ "DoubleShot", ItemType::DoubleShot },
		{ "TripleShot", ItemType::TripleShot },
		{ "OneUp", ItemType::OneUp }
	};

	// Tìm kiếm chuỗi trong map
	auto it = itemMap.find(itemStr);

	// Nếu tìm thấy
	if (it != itemMap.end())
	{
		return it->second; // Trả về giá trị enum tương ứng
	}

	DebugOut(L"[WARNING] Item type not found in map: %hs. Defaulting to SmallHeart.\n", itemStr.c_str());
	return ItemType::SmallHeart; // Mặc định là tim nhỏ
}