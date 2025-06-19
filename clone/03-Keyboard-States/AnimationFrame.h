#pragma once

#include "Sprite.h"

class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

	// --- PHẦN THÊM MỚI ---
	int anchorX; // Tọa độ X của điểm gắn so với gốc của sprite (thường là góc trên-trái)
	int anchorY; // Tọa độ Y của điểm gắn
	// ----------------------

public:
	// Sửa đổi constructor để nhận thêm anchor points
	CAnimationFrame(LPSPRITE sprite, int time, int anchorX, int anchorY) {
		this->sprite = sprite;
		this->time = time;
		this->anchorX = anchorX; // <<< THÊM MỚI
		this->anchorY = anchorY; // <<< THÊM MỚI
	}

	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }

	// --- PHẦN THÊM MỚI ---
	// Thêm hàm để lấy ra tọa độ điểm gắn
	int GetAnchorX() { return anchorX; }
	int GetAnchorY() { return anchorY; }
	// ----------------------
};

typedef CAnimationFrame* LPANIMATION_FRAME;