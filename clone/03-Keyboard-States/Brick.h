#pragma once
#include "GameObject.h"

class CBrick : public CGameObject
{
public:
	CBrick(float x, float y);
	virtual ~CBrick() {}

	// Gạch thường không cần Update hay Render riêng, CGameObject sẽ lo
};