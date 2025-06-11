#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "Game.h"
#include <vector>
#include "debug.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

class CTiledBackground : public CGameObject {
private:
	std::vector<LPSPRITE> tiles;
	int mapWidth;  // Số tile theo chiều ngang của map
	int mapHeight; // Số tile theo chiều dọc của map
	int numTilesX; // Số lượng tile trong tileset theo chiều ngang
	int numTilesY; // Số lượng tile trong tileset theo chiều dọc

public:
	CTiledBackground(float x, float y, LPSPRITE tilesetSprite, int mapWidth, int mapHeight);

	virtual void Render();
	virtual void Update(DWORD dt) {} // Background không cần update
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {} // Background không có bounding box

	// Hàm lấy chỉ số tile dựa trên vị trí trong map
	int GetTileIndex(int mapX, int mapY);

	~CTiledBackground();
};