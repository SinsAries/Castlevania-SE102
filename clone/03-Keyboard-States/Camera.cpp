#include "Camera.h"
#include <algorithm>

template <typename T>
T Clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


Camera::Camera(int screenWidth, int screenHeight)
    : width(screenWidth), height(screenHeight) {
    x = 0;
    y = 0;
}

void Camera::FollowSimon(float simonX, float simonY)
{
    this->x = simonX - width / 2;
    this->y = simonY - height / 2;

    // Giới hạn Camera theo min/max đã đặt
    if (this->x < limitMinX) this->x = limitMinX;
    if (this->y < limitMinY) this->y = limitMinY;
    if (this->x > limitMaxX - width) this->x = limitMaxX - width;
    if (this->y > limitMaxY - height) this->y = limitMaxY - height;
}


void Camera::GetPosition(float& outX, float& outY) const {
    outX = x;
    outY = y;
}

void Camera::SetLimits(float minX, float minY, float maxX, float maxY) {
    limitMinX = minX;
    limitMinY = minY;
    limitMaxX = maxX;
    limitMaxY = maxY;
}