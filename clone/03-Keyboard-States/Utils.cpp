#include "Utils.h"

void DebugOut(wchar_t* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring* w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}

ItemType StringToItemType(const std::string& str)
{
	// Tạo một bảng tra cứu tĩnh để không phải tạo lại mỗi lần gọi hàm
	static std::map<std::string, ItemType> const item_table = {
		{"SmallHeart", ItemType::SmallHeart},
		{"LargeHeart", ItemType::LargeHeart},
		{"Money50", ItemType::Money50},
		{"Money100", ItemType::Money100},
		{"Money400", ItemType::Money400},
		{"Dagger", ItemType::Dagger},
		{"Axe", ItemType::Axe},
		{"HolyWater", ItemType::HolyWater},
		{"Stopwatch", ItemType::Stopwatch},
		{"Rosary", ItemType::Rosary},
		{"DoubleShot", ItemType::DoubleShot},
		{"TripleShot", ItemType::TripleShot},
		{"OneUp", ItemType::OneUp}
		// Thêm các item khác vào đây nếu có
	};

	// Tìm chuỗi trong bảng tra cứu
	auto it = item_table.find(str);

	// Nếu tìm thấy, trả về giá trị enum tương ứng
	if (it != item_table.end())
	{
		return it->second;
	}

	// Nếu không tìm thấy chuỗi trong bảng (ví dụ: gõ sai trong JSON),
	// trả về một giá trị mặc định để tránh game bị crash.
	DebugOut(L"[WARNING] Item type not found in map: %hs. Defaulting to SmallHeart.\n", str.c_str());
	return ItemType::LargeHeart;
}

int GetAttackAnimationId(CSimon* simon)
{
	if (simon->isSitting) {
		return (simon->getNx() > 0)
			? static_cast<int>(AnimationID::SimonSitAttackRight)
			: static_cast<int>(AnimationID::SimonSitAttackLeft);
	}
	else {
		return (simon->getNx() > 0)
			? static_cast<int>(AnimationID::SimonStandAttackRight)
			: static_cast<int>(AnimationID::SimonStandAttackLeft);
	}
}

const std::map<ItemType, AnimationID> ITEM_ANIMATION_MAP = {
	{ ItemType::LargeHeart, AnimationID::ItemLargeHeartFloat },
	{ ItemType::SmallHeart, AnimationID::ItemSmallHeartFloat },
	{ ItemType::Rosary,     AnimationID::ItemRosarySpin },
	{ ItemType::Dagger,     AnimationID::ItemDagger}
	/*{ ItemType::MoneyBagRed, AnimationID::ItemMoneyBagRed },
	{ ItemType::MoneyBagPurple, AnimationID::ItemMoneyBagPurple },
	{ ItemType::MoneyBagYellow, AnimationID::ItemMoneyBagYellow }*/
	// Thêm tất cả các cặp ItemType và AnimationID tương ứng vào đây
};