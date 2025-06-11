#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(static_cast<int>(AnimationID::Brick))->Render(x,y);
}