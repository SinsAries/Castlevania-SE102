// Texture.cpp

#include "Texture.h"
#include "debug.h"
CTexture::CTexture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* rsview)
{
	this->_tex = tex;
	this->_rsview = rsview;
	D3D10_TEXTURE2D_DESC desc;
	this->_tex->GetDesc(&desc); // Gọi hàm GetDesc để lấy thông tin
	this->_width = desc.Width;
	this->_height = desc.Height;

	DebugOut(L"[INFO] CTexture object created with size %d x %d\n", _width, _height);
}

CTexture::~CTexture()
{
	if (this->_rsview)
	{
		this->_rsview->Release();
		this->_rsview = NULL;
	}

	if (this->_tex)
	{
		this->_tex->Release();
		this->_tex = NULL;
	}

	DebugOut(L"[INFO] CTexture object destroyed and resources released.\n");
}