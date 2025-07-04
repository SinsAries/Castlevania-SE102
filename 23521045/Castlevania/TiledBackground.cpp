#include "TiledBackground.h"

CTiledBackground::CTiledBackground(float x, float y, LPSPRITE tilesetSprite, int mapWidth, int mapHeight) : CGameObject(x, y)
{
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;
	this->numTilesX = 0;
	this->numTilesY = 0;

	if (tilesetSprite == nullptr) {
		DebugOut(L"[ERROR] tilesetSprite is null for CTiledBackground\n");
		return;
	}

	// <<< SỬA LỖI CHÍNH TẠI ĐÂY >>>
	// Lấy texture từ sprite tileset được truyền vào, không hard-code nữa
	LPTEXTURE texture = tilesetSprite->GetTexture();
	if (texture == nullptr) {
		DebugOut(L"[ERROR] Texture from tilesetSprite is null\n");
		return;
	}

	// Dùng các hàm Get...() để an toàn hơn
	int tilesetLeft = tilesetSprite->GetLeft();
	int tilesetTop = tilesetSprite->GetTop();
	int tilesetWidth = tilesetSprite->GetWidth();
	int tilesetHeight = tilesetSprite->GetHeight();

	// Tính số lượng tile có thể cắt ra từ tileset
	this->numTilesX = tilesetWidth / TILE_WIDTH;
	this->numTilesY = tilesetHeight / TILE_HEIGHT;

	// Tạo các tile 32x32 từ sprite tileset
	for (int y = 0; y < numTilesY; y++) {
		for (int x = 0; x < numTilesX; x++) {
			int tileLeft = tilesetLeft + x * TILE_WIDTH;
			int tileTop = tilesetTop + y * TILE_HEIGHT;
			int tileRight = tileLeft + TILE_WIDTH;
			int tileBottom = tileTop + TILE_HEIGHT;

			LPSPRITE tileSprite = new CSprite(-1, tileLeft, tileTop, tileRight, tileBottom, texture);
			tiles.push_back(tileSprite);
		}
	}
}

int CTiledBackground::GetTileIndex(int mapX, int mapY) {
	if (numTilesX == 0 || numTilesY == 0) return -1;

	// Áp dụng quy tắc lặp lại (wrap around)
	int tileX = mapX % numTilesX;
	int tileY = mapY % numTilesY;

	if (tileX < 0) tileX += numTilesX;
	if (tileY < 0) tileY += numTilesY;

	return tileY * numTilesX + tileX;
}

void CTiledBackground::Render()
{
	CGame* game = CGame::GetInstance();
	float cam_x, cam_y;
	game->GetCamera()->GetPosition(cam_x, cam_y);

	int screenWidth = game->GetBackBufferWidth();
	int screenHeight = game->GetBackBufferHeight();

	// Tính toán tile bắt đầu vẽ trên màn hình
	int startTileX = (int)(cam_x / TILE_WIDTH);
	int startTileY = (int)(cam_y / TILE_HEIGHT);

	// Tính số lượng tile cần vẽ để lấp đầy màn hình (+2 để trừ hao khi cuộn)
	int tilesToDrawX = (screenWidth / TILE_WIDTH) + 2;
	int tilesToDrawY = (screenHeight / TILE_HEIGHT) + 2;

	// Vẽ các tile cần thiết
	for (int y = 0; y < tilesToDrawY; y++) {
		for (int x = 0; x < tilesToDrawX; x++) {

			int mapX = startTileX + x;
			int mapY = startTileY + y;

			// Vị trí vẽ trên thế giới game
			float world_x = (float)mapX * TILE_WIDTH;
			float world_y = (float)mapY * TILE_HEIGHT;

			int tileIndex = GetTileIndex(mapX, mapY);
			if (tileIndex != -1 && tileIndex < (int)tiles.size())
				tiles[tileIndex]->Draw(world_x, world_y);
		}
	}
}

CTiledBackground::~CTiledBackground() {
	for (auto sprite : tiles) {
		delete sprite;
	}
	tiles.clear();
}