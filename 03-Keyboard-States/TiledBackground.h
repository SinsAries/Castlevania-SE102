#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Game.h"
#include "Textures.h"
#include <vector>

class CTiledBackground : public CGameObject {
private:
    std::vector<CSprite*> tiles; // Danh sách các sprite tile
    int tileWidth;
    int tileHeight;
    int mapWidth; // Chiều rộng bản đồ
    int mapHeight; // Chiều cao bản đồ
    int** mapData; // Dữ liệu bản đồ từ JSON

public:
    CTiledBackground(float x, float y, vector<int> ids, int** mapData, int tile_size, int mapHeight, int mapWidth) : CGameObject(x, y) {
        this->tileHeight = tile_size;
        this->tileWidth = tile_size;
        this->mapData = mapData;
        this->mapWidth = mapWidth;
        this->mapHeight = mapHeight;
        for (int id : ids) {
            CSprite* sprite = CSprites::GetInstance()->Get(id);
            tiles.push_back(sprite);
        }
    }
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}

    ~CTiledBackground() {
        tiles.clear();
    }


    void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
        left = x;
        top = y;
        right = x;
        bottom = y;
    }
};