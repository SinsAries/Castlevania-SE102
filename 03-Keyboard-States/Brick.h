#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject {
public:
    CBrick(float x, float y) : CGameObject(x, y) {}
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}
    float GetWidth() const override { return BRICK_BBOX_WIDTH; }
    float GetHeight() const override { return BRICK_BBOX_HEIGHT; }
};
