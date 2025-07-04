#include "font.h"
#include "GameIDs.h" // Cần để lấy ID của Font

// Khởi tạo instance của singleton
CFont* CFont::__instance = nullptr;

CFont* CFont::GetInstance()
{
	if (__instance == nullptr) __instance = new CFont();
	return __instance;
}

CFont::CFont()
{
	sprites = CSprites::GetInstance();
}

void CFont::Draw(float x, float y, const std::string& s)
{
	for (size_t i = 0; i < s.length(); i++)
	{
		char character = s[i];
		if (character == ' ') continue;

		int spriteId = -1;
		if (character >= 'A' && character <= 'Z') {
			spriteId = static_cast<int>(SpriteID::Font_A) + (character - 'A');
		}
		else if (character >= '0' && character <= '9') {
			spriteId = static_cast<int>(SpriteID::Font_0) + (character - '0');
		}
		else if (character == '-') {
			spriteId = static_cast<int>(SpriteID::Font_Symbol_Dash);
		}

		if (spriteId != -1) {
			CSprites::GetInstance()->Get(spriteId)->Draw(x + i * 15, y); // Giả sử mỗi ký tự rộng 8 pixel
		}
	}
}