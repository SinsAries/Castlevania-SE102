#include "WeaponFactory.h"
#include "Dagger.h"

LPGAMEOBJECT CWeaponFactory::CreateSubWeapon(ESubWeaponType type, float x, float y, int nx)
{
	switch (type)
	{
	case ESubWeaponType::Dagger:
		return new CDagger(x, y, nx); 
	default:
		return nullptr;
	}
}