#pragma once

class Camera {
public:
    float x, y;           // Vị trí của Camera
    int width, height;    // Kích thước màn hình
    float limitMinX, limitMinY; // Giới hạn trên trái
    float limitMaxX, limitMaxY; // Giới hạn dưới phải

public:
    Camera(int screenWidth, int screenHeight);

    void FollowSimon(float simonX, float simonY); // Theo dõi Simon
    void GetPosition(float& outX, float& outY) const; // Lấy vị trí Camera

    void SetLimits(float minX, float minY, float maxX, float maxY); // Giới hạn Camera
};
