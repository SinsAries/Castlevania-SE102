#pragma once
#include <string>
#include "Sprites.h"

class CFont
{
	static CFont* __instance;
	CSprites* sprites;

public:
	CFont();
	void Draw(float x, float y, const std::string& s);

	static CFont* GetInstance();
};