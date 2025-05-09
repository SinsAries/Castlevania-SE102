#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
#include "KeyEventHandler.h"
#include "Game.h"
#include "Weapon.h"
#include "SimonState.h"

class IdleState;
class WalkState;
class SitState;
class JumpState;
class AttackState;

#pragma region STATE_ID

#define SIMON_WALKING_SPEED		0.1f

#define SIMON_JUMP_SPEED_Y		0.5f

#define SIMON_GRAVITY			0.001f

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

#define SIMON_BBOX_WIDTH 16
#define SIMON_BBOX_HEIGHT 21


#pragma endregion

#define GROUND_Y 160.0f

class CSimon : public CGameObject, public CKeyEventHandler
{
	friend class ISimonState;
	friend class IdleState;
	friend class WalkState;
	friend class SitState;
	friend class JumpState;
	friend class AttackState;
	friend class CWeapon;
protected:
	BOOLEAN isSitting;
	BOOLEAN isAttacking;
	ISimonState* currentState;
	int attackCoolDown;
	BOOLEAN isOnPlatform = 1;
public:
	CSimon(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render();
	void SetState(ISimonState* state);
	void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	int IsCollidable()
	{
		return 1;
	}
};

