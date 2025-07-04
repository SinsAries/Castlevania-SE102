#include <Windows.h>

#include "debug.h"
#include "Game.h"
#include "textures.h"

CTextures* CTextures::__instance = NULL;

CTextures::CTextures()
{

}

CTextures* CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

void CTextures::Add(int id, LPCWSTR filePath)
{
	textures[id] = CGame::GetInstance()->LoadTexture(filePath);
}

LPTEXTURE CTextures::Get(int i)
{
    auto it = textures.find(i);

    if (it == textures.end())
    {
        DebugOut(L"[ERROR] Texture with ID %d not found!\n", i);
        return nullptr; // Trả về nullptr một cách tường minh
    }

    // Nếu tìm thấy, trả về texture
    return it->second;
}



