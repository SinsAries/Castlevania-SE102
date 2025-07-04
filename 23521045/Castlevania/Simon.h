#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "GameIDs.h"

#include "debug.h"
#include "Game.h"
#include "Whip.h"
#include "SimonState.h"

struct KeyMappings {
	int up;
	int down;
	int left;
	int right;
	int jump;
	int attack;
	int subweapon;
};

class ISimonState;
class IdleState;
class WalkState;
class SitState;
class JumpState;
class AttackState;

class CSimon : public CGameObject	
{
	friend class ISimonState;
	friend class IdleState;
	friend class WalkState;
	friend class SitState;
	friend class JumpState;
	friend class AttackState;
public:
	KeyMappings keyMappings;
	ESubWeaponType currentSubWeapon;
	int hearts;
	int score;
	int shotMultipler;
	int lives;    
	int health;      
	int max_health;  
	int pvpCollisionCooldownTimer;
	int subWeaponCoolDownTimer;

	BOOLEAN isSitting;
	BOOLEAN isAttacking;
	ISimonState* currentState;
	int attackCoolDown;
	CWhip* whip;
public:
	// === GAMEPLAY CONSTANTS ===
	static constexpr float WALKING_SPEED = 0.1f;
	static constexpr float JUMP_SPEED_Y = 0.5f;
	static constexpr float GRAVITY = 0.002f;
	static constexpr float GROUND_Y = 160.0f;
	static constexpr int ATTACK_TIME_MS = 300;
	static constexpr int ATTACK_COOLDOWN_MS = 500;
	static constexpr int SUB_WEAPON_COOLDOWN_TIME = 500;
	static constexpr float WORLD_BOUNDARY_RIGHT = 990.0f;
	static constexpr float START_X = 200.0f;
	static constexpr float START_Y = 10.0f;
	static constexpr float SIMON_SITTING_BBOX_WIDTH = 16;
	static constexpr float SIMON_SITTING_BBOX_HEIGHT = 23;
	static constexpr float SIMON_STANDING_BBOX_WIDTH = 16;
	static constexpr float SIMON_STANDING_BBOX_HEIGHT = 30;
	static constexpr float PVP_KNOCKBACK_SPEED_X = 0.8f;
	static constexpr float PVP_KNOCKBACK_SPEED_Y = -0.2f;
	static constexpr int PVP_COLLISION_COOLDOWN_MS = 500;

	// === METHODS ===
	CSimon(float x, float y);
	void TryUseSubWeapon(); 
	void SpawnSubWeapon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	void Render();
	void SetState(ISimonState* state);
	CWhip* GetWhip();
	int GetScore() const { return score; }
	int GetHearts() const { return hearts; }
	int GetLives() const { return lives; }
	int GetHealth() const { return health; }
	int GetMaxHealth() const { return max_health; }
	ESubWeaponType GetCurrentSubWeapon() const { return currentSubWeapon; }
	/*bool IsSitting() const { return isSitting; }
	bool IsAttacking() const { return isAttacking; }*/

	void SetKeyMappings(const KeyMappings& mappings);
	//void AddHealth(int amount);
	void OnCollision(LPCOLLISIONEVENT e) override;
	void AddHearts(int amount);
	void SetSubWeapon(ESubWeaponType subWeaponType);
	void AddScore(int amount);

	~CSimon();
};

