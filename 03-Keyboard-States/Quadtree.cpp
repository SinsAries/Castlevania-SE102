#include "Quadtree.h"
#include "Game.h"
#include "debug.h"

Quad::Quad(int _level, Point _topL, Point _botR)
    : level(_level), topLeft(_topL), botRight(_botR),
    topLeftTree(nullptr), topRightTree(nullptr),
    botLeftTree(nullptr), botRightTree(nullptr) {
}

Quad::~Quad() {
    delete topLeftTree;
    delete topRightTree;
    delete botLeftTree;
    delete botRightTree;

    for (auto* obj : objs) {
        delete obj;
    }
}

void Quad::split() {
    int midX = (topLeft.x + botRight.x) / 2;
    int midY = (topLeft.y + botRight.y) / 2;

    topLeftTree = new Quad(level + 1, topLeft, Point(midX, midY));
    topRightTree = new Quad(level + 1, Point(midX, topLeft.y), Point(botRight.x, midY));
    botLeftTree = new Quad(level + 1, Point(topLeft.x, midY), Point(midX, botRight.y));
    botRightTree = new Quad(level + 1, Point(midX, midY), botRight);
}

std::vector<int> Quad::getIndices(CGameObject* obj) const {
    std::vector<int> indices;
    int midX = (topLeft.x + botRight.x) / 2;
    int midY = (topLeft.y + botRight.y) / 2;

    bool top = obj->y < midY;
    bool bot = obj->y > midY;
    bool left = obj->x < midX;
    bool right = obj->x > midX;

    if (top && left) indices.push_back(0); // top-left
    if (top && right) indices.push_back(1); // top-right
    if (bot && left) indices.push_back(2); // bot-left
    if (bot && right) indices.push_back(3); // bot-right

    return indices;
}

void Quad::insert(CGameObject* obj) {
    if (obj == nullptr) return;
    if (obj->x < topLeft.x || obj->x > botRight.x || obj->y < topLeft.y || obj->y > botRight.y) {
        return; // Object is outside this quadrant
    }

    if (topLeftTree != nullptr) {
        std::vector<int> indices = getIndices(obj);
        for (int index : indices) {
            switch (index) {
            case 0: topLeftTree->insert(obj); break;
            case 1: topRightTree->insert(obj); break;
            case 2: botLeftTree->insert(obj); break;
            case 3: botRightTree->insert(obj); break;
            }
        }
        return;
    }

    objs.push_back(obj);

    if (objs.size() > MAX_OBJECTS && level < MAX_LEVELS) {
        split();
        for (auto* obj : objs) {
            std::vector<int> indices = getIndices(obj);
            for (int index : indices) {
                switch (index) {
                case 0: topLeftTree->insert(obj); break;
                case 1: topRightTree->insert(obj); break;
                case 2: botLeftTree->insert(obj); break;
                case 3: botRightTree->insert(obj); break;
                }
            }
        }
        objs.clear();
    }
}

void Quad::retrieve(const Point& viewTopLeft, const Point& viewBotRight, std::vector<CGameObject*>& foundObjects) {
    // Check if the quad's area intersects with the view area
    if (topLeft.x > viewBotRight.x || botRight.x < viewTopLeft.x ||
        topLeft.y > viewBotRight.y || botRight.y < viewTopLeft.y) {
        return;
    }

    for (auto* obj : objs) {
        if (obj->x >= viewTopLeft.x && obj->x <= viewBotRight.x &&
            obj->y >= viewTopLeft.y && obj->y <= viewBotRight.y) {
            foundObjects.push_back(obj);
        }
    }

    if (topLeftTree == nullptr) {
        return;
    }

    if (topLeftTree != nullptr) topLeftTree->retrieve(viewTopLeft, viewBotRight, foundObjects);
    if (topRightTree != nullptr) topRightTree->retrieve(viewTopLeft, viewBotRight, foundObjects);
    if (botLeftTree != nullptr) botLeftTree->retrieve(viewTopLeft, viewBotRight, foundObjects);
    if (botRightTree != nullptr) botRightTree->retrieve(viewTopLeft, viewBotRight, foundObjects);
}

void Quad::Update(DWORD dt) {
    // Update logic if needed
}

void Quad::Render() {
    CGame* game = CGame::GetInstance();
    float cx, cy;
    game->GetCamera()->GetPosition(cx, cy);
    int screenWidth = game->GetBackBufferWidth();
    int screenHeight = game->GetBackBufferHeight();

    std::vector<CGameObject*> res;
    retrieve(Point(max(0.0f, cx - 50), max(0.0f, cy - 50)),
        Point(cx + screenWidth + 50, 50 + cy + screenHeight), res);
    int cnt = 0;
    for (auto* obj : res) {
        obj->Render();
        cnt++;
    }
    DebugOut(L"[INFO] The Number Of Current Objects Rendered: %d\n", cnt);
}