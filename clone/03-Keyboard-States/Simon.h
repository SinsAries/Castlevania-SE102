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
public:
	// === GAMEPLAY CONSTANTS ===
	static constexpr float WALKING_SPEED = 0.1f;
	static constexpr float JUMP_SPEED_Y = 0.5f;
	static constexpr float GRAVITY = 0.002f;
	static constexpr float GROUND_Y = 160.0f;
	static constexpr int ATTACK_TIME_MS = 900;
	static constexpr int ATTACK_COOLDOWN_MS = 150;
	static constexpr float WORLD_BOUNDARY_RIGHT = 990.0f;
	static constexpr float START_X = 200.0f;
	static constexpr float START_Y = 10.0f;

	// === METHODS ===
	CSimon(float x, float y);
	void Update(DWORD dt);
	void Render();
	void SetState(ISimonState* state);
	/*bool IsSitting() const { return isSitting; }
	bool IsAttacking() const { return isAttacking; }*/

	// === KEYBOARD EVENTS ===
	void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

