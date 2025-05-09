#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX -100

class CGameObject
{
protected:
    float x;
    float y;

    float vx;
    float vy;

    int nx;

    int state;

    bool isDeleted = 0;

    friend class Quad;

public:
    friend class Quad;
    void SetPosition(float x, float y) { this->x = x, this->y = y; }
    void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
    void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
    void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
    void SetDirection(int direction) { nx = direction; }
    virtual void SetState(int state) { this->state = state; } // Make this method virtual
    int GetState() { return this->state; }

    void RenderBoundingBox();

    CGameObject();
    CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }
    CGameObject(float x, float y, int nx) :CGameObject(x, y) { this->nx = nx; }

    virtual float GetWidth() const = 0;
    virtual float GetHeight() const = 0;

    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) const {
        l = x;
        t = y;
        r = x + GetWidth();
        b = y + GetHeight();
    }
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
    virtual void Render() = 0;
    ~CGameObject();
    //
    // Collision ON or OFF ? This can change depending on object's state. For example: die
    //
    virtual int IsCollidable() { return 0; };

    // When no collision has been detected (triggered by CCollision::Process)
    virtual void OnNoCollision(DWORD dt) {};

    // When collision with an object has been detected (triggered by CCollision::Process)
    virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};

    // Is this object blocking other object? If YES, collision framework will automatically push the other object
    virtual int IsBlocking() { return 1; }

    // Is this object collide with other object at certain direction ( like ColorBox )
    virtual int IsDirectionColliable(float nx, float ny) { return 1; }

    bool IsDeleted() {
        return isDeleted;
    }
};


typedef CGameObject* LPGAMEOBJECT;