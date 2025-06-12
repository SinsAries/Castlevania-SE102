#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "GameIDs.h"

#include "debug.h"
#include "KeyEventHandler.h"
#include "Game.h"
#include "Weapon.h"
#include "SimonState.h"

class ISimonState;
class IdleState;
class WalkState;
class SitState;
class JumpState;
class AttackState;
class CWeapon;

class CSimon : public CGameObject, public CKeyEventHandler
{
	friend class ISimonState;
	friend class IdleState;
	friend class WalkState;
	friend class SitState;
	friend class JumpState;
	friend class AttackState;
	friend class CWeapon;
public:
	BOOLEAN isSitting;
	BOOLEAN isAttacking;
	ISimonState* currentState;
	int attackCoolDown;
	BOOLEAN isOnPlatform = false;
public:
	// === GAMEPLAY CONSTANTS ===
	static constexpr float WALKING_SPEED = 0.1f;
	static constexpr float JUMP_SPEED_Y = 0.5f;
	static constexpr float GRAVITY = 0.002f;
	static constexpr float GROUND_Y = 300.0f;
	static constexpr int ATTACK_TIME_MS = 900;
	static constexpr int ATTACK_COOLDOWN_MS = 150;
	static constexpr float WORLD_BOUNDARY_RIGHT = 700.0f;
	static constexpr float SIMON_BBOX_WIDTH = 16.0f;
	static constexpr float SIMON_BBOX_HEIGHT = 21.0f;
	static constexpr float SIMON_GRAVITY = 0.002f;

	// === METHODS ===
	CSimon(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render();
	void SetState(ISimonState* state);
	void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void OnNoCollision(DWORD dt) override;
	int IsCollidable()
	{
		return 1;
	}
	float GetWidth() const override { return SIMON_BBOX_WIDTH; }
	float GetHeight() const override { return SIMON_BBOX_HEIGHT; }
};

