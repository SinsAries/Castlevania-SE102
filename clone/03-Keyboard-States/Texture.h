// Texture.h - PHIÊN BẢN HOÀN CHỈNH

#pragma once
#include <d3d10.h>
#include <d3dx10.h>

class CTexture
{
protected:
	// Con trỏ tới tài nguyên texture gốc (dữ liệu hình ảnh)
	ID3D10Texture2D* _tex;

	// Con trỏ tới "view" của texture (dùng để vẽ)
	ID3D10ShaderResourceView* _rsview;

	int _width;
	int _height;

public:
	CTexture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* rsview);
	~CTexture();

	// Hàm này trả về cái "view" để SpriteHandler có thể vẽ
	ID3D10ShaderResourceView* getShaderResourceView() { return this->_rsview; }

	// Hàm này trả về tài nguyên texture gốc để lấy thông tin (width, height)
	ID3D10Texture2D* getTexture2D() { return this->_tex; }

	int getWidth() { return this->_width; }
	int getHeight() { return this->_height; }
};

typedef CTexture* LPTEXTURE;