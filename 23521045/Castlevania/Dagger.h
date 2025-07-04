#pragma once
#include "SubWeapon.h"


class CDagger : public CSubWeapon
{
	constexpr static float DAGGER_SPEED = 0.3f;
public:
	CDagger(float x, float y, int nx);
	virtual ~CDagger() {}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

