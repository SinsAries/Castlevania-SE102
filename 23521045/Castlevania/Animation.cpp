#include "Animation.h"
#include "debug.h"

void CAnimation::Add(int spriteId, DWORD time, int anchorX, int anchorY)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t, anchorX, anchorY);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int nx)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			// Dùng >= để an toàn hơn trong trường hợp frames rỗng
			if (currentFrame >= frames.size()) currentFrame = 0;
		}
	}

	if (currentFrame == frames.size() - 1) {
		isFinished = true;
	}

	if (currentFrame >= 0 && currentFrame < frames.size())
	{

		frames[currentFrame]->GetSprite()->Draw(x, y, nx);
	}
}

float CAnimation::GetCurrentFrameWidth()
{
	if (currentFrame >= 0 && currentFrame < frames.size())
	{
		return (float)frames[currentFrame]->GetSprite()->GetWidth();
	}
	return 0;
}

float CAnimation::GetCurrentFrameHeight()
{
	if (currentFrame >= 0 && currentFrame < frames.size())
	{
		return (float)frames[currentFrame]->GetSprite()->GetHeight();
	}
	return 0;
}

void CAnimation::GetCurrentFrameAnchorPoint(int& x, int& y)
{
	if (currentFrame == -1)
	{
		x = 0;
		y = 0;
		return;
	}

	if (currentFrame >= 0 && currentFrame < frames.size())
	{
		x = frames[currentFrame]->GetAnchorX();
		y = frames[currentFrame]->GetAnchorY();
	}
	else
	{
		x = 0;
		y = 0;
	}
}

void CAnimation::Reset()
{
	currentFrame = -1;
	lastFrameTime = -1;
	isFinished = false;
}

int CAnimation::GetCurrentFrame()
{
	return currentFrame;
}

void CAnimation::SetCurrentFrame(int frameIndex)
{
	if (frameIndex < 0 || frameIndex >= frames.size())
	{
		return;
	}
	this->currentFrame = frameIndex;
	this->lastFrameTime = GetTickCount64();
}

void CAnimation::Add(LPSPRITE sprite, DWORD time)
{
	int t = time;
	if (time == 0)
	{
		t = this->defaultTime;
	}
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t, 0, 0);

	frames.push_back(frame);
}