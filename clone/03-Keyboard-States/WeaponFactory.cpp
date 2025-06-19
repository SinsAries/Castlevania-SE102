#include "WeaponFactory.h"
#include "Dagger.h"
#include "Axe.h"

LPGAMEOBJECT CWeaponFactory::CreateSubWeapon(ESubWeaponType type, float x, float y, int nx)
{
	switch (type)
	{
	case ESubWeaponType::Dagger:
		return new CDagger(x, y, nx); // Truyền vào hướng nx
	case ESubWeaponType::Axe:
		//return new CAxe(x, y, nx);
		// ... các trường hợp khác
	default:
		return nullptr;
	}
}