#include "Animation.h"
#include "debug.h"

void CAnimation::Add(int spriteId, DWORD time, int anchorX, int anchorY) // <<< SỬA ĐỔI
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	// Truyền anchorX, anchorY vào constructor của CAnimationFrame
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t, anchorX, anchorY); // <<< SỬA ĐỔI
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

	// Thêm một lớp kiểm tra an toàn cuối cùng trước khi truy cập
	if (currentFrame >= 0 && currentFrame < frames.size())
	{
		/*DebugOut(L"frame w: %d\n", frames[currentFrame]->GetSprite()->GetWidth());
		DebugOut(L"frame h: %d\n", frames[currentFrame]->GetSprite()->GetHeight());
		DebugOut(L"frame x, y: %f, %f, %d", x, y, nx);*/

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
	// Thêm lớp bảo vệ ngay từ đầu. Nếu animation chưa bắt đầu (frame = -1),
	// trả về (0,0) và thoát ngay lập tức.
	if (currentFrame == -1)
	{
		x = 0;
		y = 0;
		return;
	}

	// Lớp bảo vệ thứ hai để đảm bảo currentFrame không bị lỗi ngoài dự tính
	if (currentFrame >= 0 && currentFrame < frames.size())
	{
		x = frames[currentFrame]->GetAnchorX();
		y = frames[currentFrame]->GetAnchorY();
	}
	else
	{
		// Trường hợp dự phòng nếu currentFrame có giá trị không hợp lệ
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
	// Kiểm tra an toàn để đảm bảo index không nằm ngoài giới hạn
	if (frameIndex < 0 || frameIndex >= frames.size())
	{
		return;
	}
	this->currentFrame = frameIndex;
	this->lastFrameTime = GetTickCount64();
}

void CAnimation::Add(LPSPRITE sprite, DWORD time)
{
	// Nếu người dùng không truyền vào "time", thì dùng defaultFrameTime của animation
	int t = time;
	if (time == 0)
	{
		t = this->defaultTime;
	}

	// Tạo một CAnimationFrame mới trực tiếp từ con trỏ LPSPRITE
	// và các thông số khác (anchor point mặc định là 0, 0)
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t, 0, 0);

	// Thêm frame mới này vào danh sách các frame của animation
	frames.push_back(frame);
}