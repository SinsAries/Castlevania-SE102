#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"

using namespace std;

class CAnimation
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
	bool isFinished;
public:
	CAnimation(int defaultTime) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }

	// Sửa đổi hàm Add để nhận anchor points, với giá trị mặc định là 0
	void Add(int spriteId, DWORD time = 0, int anchorX = 0, int anchorY = 0); // <<< SỬA ĐỔI

	// Thêm tham số nx để xử lý lật sprite
	void Render(float x, float y, int nx = 1); // <<< SỬA ĐỔI

	void Reset();
	int GetCurrentFrame();
	float GetCurrentFrameWidth();
	float GetCurrentFrameHeight();

	// --- PHẦN THÊM MỚI ---
	// Hàm quan trọng để lấy ra điểm gắn của frame hiện tại
	void GetCurrentFrameAnchorPoint(int& x, int& y);
	void Add(LPSPRITE sprite, DWORD time = 0);
	bool IsFinished() const { return isFinished; }
	// ----------------------

	void SetCurrentFrame(int frameIndex);
};

typedef CAnimation* LPANIMATION;