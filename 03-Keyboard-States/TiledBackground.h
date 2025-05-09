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
    std::vector<CSprite*> tiles;
    int tileWidth;
    int tileHeight;
    int numTilesX;  // Số lượng tile theo chiều ngang
    int numTilesY;  // Số lượng tile theo chiều dọc
    int mapWidth;   // Số tile theo chiều ngang của map
    int mapHeight;  // Số tile theo chiều dọc của map

public:
    CTiledBackground(float x, float y, CSprite* originalSprite, int mapWidth, int mapHeight) : CGameObject(x, y) {
        this->tileWidth = 32;
        this->tileHeight = 32;
        this->mapWidth = mapWidth;
        this->mapHeight = mapHeight;

        // Tính số lượng tile từ sprite gốc
        int spriteWidth = originalSprite->right - originalSprite->left + 1;
        int spriteHeight = originalSprite->bottom - originalSprite->top + 1;
        numTilesX = spriteWidth / tileWidth;
        numTilesY = spriteHeight / tileHeight;

        // Tạo các tile 32x32 từ sprite gốc
        LPTEXTURE texture = CTextures::GetInstance()->Get(40);
        int left = originalSprite->left;
        int top = originalSprite->top;

        for (int y = 0; y < numTilesY; y++) {
            for (int x = 0; x < numTilesX; x++) {
                int tileLeft = left + x * tileWidth;
                int tileTop = top + y * tileHeight;
                int tileRight = tileLeft + tileWidth - 1;
                int tileBottom = tileTop + tileHeight - 1;

                // Tạo sprite mới cho tile
                CSprite* tileSprite = new CSprite(
                    -1,  // ID tạm thời
                    tileLeft, tileTop, tileRight, tileBottom,
                    texture
                );

                tiles.push_back(tileSprite);
            }
        }
    }

    // Hàm lấy chỉ số tile dựa trên vị trí trong map
    int GetTileIndex(int mapX, int mapY) {
        // Áp dụng quy tắc lặp lại (wrap around) nếu cần
        mapX = mapX % numTilesX;
        mapY = mapY % numTilesY;

        // Xử lý giá trị âm
        if (mapX < 0) mapX += numTilesX;
        if (mapY < 0) mapY += numTilesY;

        return mapY * numTilesX + mapX;
    }

    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}

    ~CTiledBackground() {
        // Clean up tile sprites
        for (auto sprite : tiles) {
            delete sprite;
        }
        tiles.clear();
    }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x;
		top = y;
        right = x;
        bottom = y;
	}
};