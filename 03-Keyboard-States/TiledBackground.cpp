#include "TiledBackground.h"

void CTiledBackground::Render()
{
    CGame* game = CGame::GetInstance();
    float cx, cy;
    game->GetCamera()->GetPosition(cx, cy);
    int screenWidth = game->GetBackBufferWidth();
    int screenHeight = game->GetBackBufferHeight();

    // Tính toán tile bắt đầu dựa trên vị trí camera
    int startTileX = (int)(cx / tileWidth);
    int startTileY = (int)(cy / tileHeight);

    // Tính số lượng tile cần để phủ màn hình
    int tilesNeededX = (screenWidth / tileWidth) + 2; // +2 để đảm bảo không có khoảng trống
    int tilesNeededY = (screenHeight / tileHeight) + 2;

    // Vẽ các tile cần thiết
    for (int y = 0; y < tilesNeededY; y++) {
        for (int x = 0; x < tilesNeededX; x++) {
            // Vị trí thực tế trong thế giới game
            float worldPosX = (startTileX + x) * tileWidth;
            float worldPosY = (startTileY + y) * tileHeight;

            // Chọn sprite tile phù hợp từ danh sách tiles
            int tileIndex = GetTileIndex(startTileX + x, startTileY + y);
            if (tileIndex >= 0 && tileIndex < (int)tiles.size())
                tiles[tileIndex]->Draw(worldPosX, worldPosY);
        }
    }
}