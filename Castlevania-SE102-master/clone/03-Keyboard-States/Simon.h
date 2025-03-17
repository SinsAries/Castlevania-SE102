#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
#include "KeyEventHandler.h"
#include "Game.h"
#include "Weapon.h"

#pragma region STATE_ID

#define SIMON_WALKING_SPEED		0.1f

#define SIMON_JUMP_SPEED_Y		0.5f

#define SIMON_GRAVITY			0.002f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200

#define SIMON_STATE_JUMP			300

#define SIMON_STATE_SIT				600

#define SIMON_STATE_ATTACK			700

#pragma endregion

#pragma region ANIMATION_ID

#define ID_ANI_SIMON_IDLE_RIGHT         2400
#define ID_ANI_SIMON_IDLE_LEFT          2401

#define ID_ANI_SIMON_WALKING_RIGHT      2500
#define ID_ANI_SIMON_WALKING_LEFT       2501

#define ID_ANI_SIMON_STAND_ATTACK_RIGHT 2600
#define ID_ANI_SIMON_STAND_ATTACK_LEFT  2601

#define ID_ANI_SIMON_JUMP_WALK_RIGHT    2700
#define ID_ANI_SIMON_JUMP_WALK_LEFT     2701

#define ID_ANI_SIMON_SIT_ATTACK_RIGHT   2800
#define ID_ANI_SIMON_SIT_ATTACK_LEFT    2801

#define ID_ANI_SIMON_SIT_RIGHT          2900
#define ID_ANI_SIMON_SIT_LEFT           2901

#pragma endregion

#define GROUND_Y 160.0f

class CSimon : public CGameObject, public CKeyEventHandler
{
protected:
	BOOLEAN isSitting;
	BOOLEAN isAttacking;
	int attackTime;
	int currentId;
public:
	friend class CWeapon;
	CSimon(float x, float y) : CGameObject(x, y) {
		isSitting = false;

	}
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
	void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

