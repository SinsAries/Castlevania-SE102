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
    void GetBoundingBox(float& l, float& t, float& r, float& b) {
        l = x;
        t = y;
        r = x + BRICK_BBOX_WIDTH;
        b = y + BRICK_BBOX_HEIGHT;
    }
};
