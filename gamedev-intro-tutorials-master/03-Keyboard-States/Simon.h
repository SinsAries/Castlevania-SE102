#pragma once
#include "GameObject.h"
#include "KeyEventHandler.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define SIMON_WALKING_SPEED		0.1f
#define SIMON_RUNNING_SPEED		0.2f

#define SIMON_ACCEL_WALK_X		0.0005f
#define SIMON_ACCEL_RUN_X		0.0007f

#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_RUN_SPEED_Y	0.6f

#define SIMON_GRAVITY			0.0f

#define SIMON_STATE_IDLE			1000
#define SIMON_STATE_WALKING_RIGHT	1100
#define SIMON_STATE_WALKING_LEFT	1200

#define SIMON_STATE_JUMP			1300
#define SIMON_STATE_RELEASE_JUMP    1301

#define SIMON_STATE_RUNNING_RIGHT	1400
#define SIMON_STATE_RUNNING_LEFT	1500

#define SIMON_STATE_SIT				1600
#define SIMON_STATE_SIT_RELEASE		1601

#pragma region ANIMATION_ID

#define ID_ANI_SIMON_IDLE_RIGHT 1400
#define ID_ANI_SIMON_IDLE_LEFT 1401

#define ID_ANI_SIMON_WALKING_RIGHT 1500
#define ID_ANI_SIMON_WALKING_LEFT 1501

#define ID_ANI_SIMON_RUNNING_RIGHT 1600
#define ID_ANI_SIMON_RUNNING_LEFT 1601

#define ID_ANI_SIMON_JUMP_WALK_RIGHT 1700
#define ID_ANI_SIMON_JUMP_WALK_LEFT 1701

#define ID_ANI_SIMON_JUMP_RUN_RIGHT 1800
#define ID_ANI_SIMON_JUMP_RUN_LEFT 1801

#define ID_ANI_SIMON_SIT_RIGHT 1900
#define ID_ANI_SIMON_SIT_LEFT 1901

#define ID_ANI_SIMON_BRACE_RIGHT 2000
#define ID_ANI_SIMON_BRACE_LEFT 2001

#pragma endregion

#define GROUND_Y 160.0f

#define SIMON_SIT_HEIGHT_ADJUST 4.0f

class CSimon : public CGameObject, public CKeyEventHandler
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
public:
	CSimon(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
	}
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};