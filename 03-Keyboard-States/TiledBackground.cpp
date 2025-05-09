#include "TiledBackground.h"

void CTiledBackground::Render()
{
    CGame* game = CGame::GetInstance();
    float cx, cy;
    game->GetCamera()->GetPosition(cx, cy);
    int screenWidth = game->GetBackBufferWidth();
    int screenHeight = game->GetBackBufferHeight();

    int startCol = (int)(cx / tileWidth);
    int endCol = (int)((cx + screenWidth) / tileWidth) + 2;
    int startRow = (int)(cy / tileHeight);
    int endRow = (int)((cy + screenHeight) / tileHeight) + 2;

    for (int row = startRow; row <= endRow; ++row)
    {
        for (int col = startCol; col <= endCol; ++col)
        {
            // Kiểm tra index hợp lệ
            int col2 = col, row2 = row;
            if (col >= mapWidth) col2 = col % mapWidth;
            if (row >= mapHeight) row2 = row % mapHeight;
            if (col < 0) col2 = col % mapWidth + mapWidth;
            if (row < 0) row2 = row % mapHeight + mapHeight;


            int tileIndex = mapData[row2][col2];
            float xPos = x + col * tileWidth;
            float yPos = y + row * tileHeight;
            tiles[tileIndex]->Draw(xPos, yPos);

        }
    }
}