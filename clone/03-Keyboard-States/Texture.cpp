// Texture.cpp

#include "Texture.h"
#include "debug.h"

// --- HÀM TẠO (CONSTRUCTOR) ---
// Nhiệm vụ: Nhận vào các con trỏ DirectX thô và lưu trữ chúng, đồng thời lấy thông tin width/height
CTexture::CTexture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* rsview)
{
	// 1. Lưu lại các con trỏ DirectX được truyền vào
	this->_tex = tex;
	this->_rsview = rsview;

	// 2. Lấy và lưu lại kích thước của texture để sau này dùng cho tiện
	D3D10_TEXTURE2D_DESC desc;
	this->_tex->GetDesc(&desc); // Gọi hàm GetDesc để lấy thông tin
	this->_width = desc.Width;
	this->_height = desc.Height;

	DebugOut(L"[INFO] CTexture object created with size %d x %d\n", _width, _height);
}


// --- HÀM HỦY (DESTRUCTOR) ---
// Nhiệm vụ: Dọn dẹp, giải phóng các đối tượng DirectX khi đối tượng CTexture bị hủy
CTexture::~CTexture()
{
	// Luôn kiểm tra con trỏ khác NULL trước khi Release để tránh crash
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