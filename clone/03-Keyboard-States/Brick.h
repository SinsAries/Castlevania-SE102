#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "GameIDs.h"

class CBrick : public CGameObject {
public:
	// === GAMEPLAY CONSTANTS ===
	static constexpr int BRICK_WIDTH = 16;
	static constexpr float BRICK_X = 0.0f;
	static constexpr float BRICK_Y = 180.0f;
	static constexpr int BRICK_BBOX_WIDTH = 16;
	static constexpr int BRICK_BBOX_HEIGHT = 16;

	// === METHODS ===
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}
	float GetWidth() const override { return BRICK_BBOX_WIDTH; }
	float GetHeight() const override { return BRICK_BBOX_HEIGHT; }
};