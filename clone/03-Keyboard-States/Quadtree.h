#pragma once
#include <vector>
#include "Sprite.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Game.h"
#include "Textures.h"

struct Point {
    float x;
    float y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

class Quad {
private:
    Point topLeft;
    Point botRight;
    Quad* topLeftTree;
    Quad* topRightTree;
    Quad* botLeftTree;
    Quad* botRightTree;

    const int MAX_OBJECTS = 4;
    const int MAX_LEVELS = 5;
    int level;
public:
    std::vector<CGameObject*> objs;
    Quad(int _level, Point _topL, Point _botR);
    ~Quad();
    void split();
    std::vector<int> getIndices(CGameObject* obj) const;
    void insert(CGameObject* obj);
    void retrieve(const Point& viewTopLeft, const Point& viewBotRight, std::vector<CGameObject*>& foundObjects);
    void Update(DWORD dt);
    void Render();
};