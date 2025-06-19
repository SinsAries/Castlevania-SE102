#include "Sprite.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;

	// Set the sprite�s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	sprite.TexCoord.x = this->left / (float)tex->getWidth();
	sprite.TexCoord.y = this->top / (float)tex->getHeight();

	int spriteWidth = (this->right - this->left + 1);
	int spriteHeight = (this->bottom - this->top + 1);

	sprite.TexSize.x = spriteWidth / (float)tex->getWidth();
	sprite.TexSize.y = spriteHeight / (float)tex->getHeight();

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);
}

void CSprite::Draw(float x, float y, int nx)

{

	CGame* g = CGame::GetInstance();

	float cx, cy;

	g->GetCamera()->GetPosition(cx, cy);



	cx = (FLOAT)floor(cx);

	cy = (FLOAT)floor(cy);



	// 1. Lấy kích thước của sprite

	int spriteWidth = this->right - this->left + 1;

	int spriteHeight = this->bottom - this->top + 1;



	// 2. Tính toán tọa độ vẽ góc trên-trái (drawX, drawY) trong World Space

	float drawX, drawY;



	// Tọa độ Y để vẽ góc trên luôn là y - height/2

	drawY = y - spriteHeight / 2.0f;



	// Tọa độ X để vẽ góc trên phụ thuộc vào hướng lật

	if (nx > 0)

	{

		// Nếu không lật (quay phải), điểm bắt đầu vẽ là bên trái của tâm

		drawX = x - spriteWidth / 2.0f;

	}

	else

	{

		// Nếu bị lật (quay trái), sprite được vẽ ngược từ phải sang trái.

		// Để tâm vẫn ở đúng vị trí x, điểm bắt đầu vẽ phải là bên phải của tâm.

		drawX = x + spriteWidth / 2.0f;

	}



	D3DXMATRIX matTranslation;

	// SỬA Ở ĐÂY: Thêm floor() cho cả 2 tham số tọa độ
	D3DXMatrixTranslation(&matTranslation,
		floor(x - cx),
		floor(g->GetBackBufferHeight() - (y - cy)),
		0.1f);



	this->sprite.matWorld = (this->matScaling * matTranslation);



	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);	
}


