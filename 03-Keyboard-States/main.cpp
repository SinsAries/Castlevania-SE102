#include <windows.h>

#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Animation.h"
#include "Animations.h"
#include "Sprite.h"
#include "Sprites.h"

#include "Mario.h"
#include "Weapon.h"
#include "Simon.h"
#include "Brick.h"
#include "Quadtree.h"

#include "TiledBackground.h"

#include "SampleKeyEventHandler.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"03 - Keyboard and Mario states"
#define WINDOW_ICON_PATH L"mario.ico"


#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_SIMON 30
#define ID_TEX_BACKGROUND 40
#define ID_TEX_BRICK 50

#define ID_SPRITE_BRICK 20001
#define ID_SPRITE_BACKGROUND 20002

#define TEXTURES_DIR L"textures"

#define MARIO_START_X 200.0f
#define MARIO_START_Y 10.0f

#define SIMON_START_X 200.0f
#define SIMON_START_Y 10.0f

#define BRICK_X 0.0f
#define BRICK_Y GROUND_Y + 20.0f
#define NUM_BRICKS 100

CMario* mario = NULL;
CWeapon* weapon = NULL;
CSimon* simon = NULL;
CSimon* simon2 = NULL;

CTiledBackground* background = NULL;

CSampleKeyHandler* keyHandler;

vector<LPGAMEOBJECT> objects;
Quad* root = new Quad(0, Point(0, 0), Point(1000, 1000));


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

LPCWSTR StringToLPCWSTR(const std::string& str)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	wchar_t* wide_str = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide_str, size);
	return wide_str;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	std::ifstream file("./resource/resource.json");


	// Kiểm tra xem file có mở thành công không
	if (!file.is_open()) {
		std::cout << "Không thể mở file resource.json!" << std::endl;
		return;
	}

	json data;
	file >> data;

	file.close();
	std::ifstream file2("./resource/Scene1/map.json");

	if (!file2.is_open()) {
		std::cout << "Không thể mở file map.json!" << std::endl;
		return;
	}

	json mapData;
	file2 >> mapData;

	file2.close();
	CTextures* textures = CTextures::GetInstance();

	//textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	for (int i = 0; i < data["textures"].size(); i++) {
		textures->Add(data["textures"][i][0], StringToLPCWSTR(data["textures"][i][1].get<string>()));
	}


	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	{
		// Trong LoadResources()

		LPTEXTURE texBackground = textures->Get(ID_TEX_BACKGROUND); // Hoặc texture riêng cho background
		auto tiles = mapData["tiles"];
		vector<int> ids;
		for (int i = 0; i < tiles.size(); i++) {
			sprites->Add(tiles[i][0], tiles[i][1], tiles[i][2],
				tiles[i][3], tiles[i][4], texBackground);
			ids.push_back(tiles[i][0]);
		}
		int height = mapData["map"].size(); // số dòng
		int width = mapData["map"][0].size(); // số cột


		// Cấp phát động int** map
		int** mapArray = new int* [height];
		for (int i = 0; i < height; i++) {
			mapArray[i] = new int[width];
			for (int j = 0; j < width; j++) {
				mapArray[i][j] = mapData["map"].at(i).at(j).get<int>();

			}
		}

		CTiledBackground* background = new CTiledBackground(0, 0, ids, mapArray, 32, height, width);
		objects.push_back(background);
	}

	{
		LPTEXTURE texSimon = textures->Get(ID_TEX_SIMON);
		auto tmp = data["sprites"];
		for (int i = 0; i < tmp.size(); i++)
		{
			sprites->Add(tmp[i][0], tmp[i][2], tmp[i][3], tmp[i][4], tmp[i][5], texSimon);
		}

		tmp = data["animation"];
		for (int i = 0; i < tmp.size(); i++)
		{
			LPANIMATION ani = new CAnimation(tmp[i][1]);
			for (int j = 0; j < tmp[i][2].size(); j++)
			{
				ani->Add(tmp[i][2][j]);
			}
			animations->Add(tmp[i][0], ani);
		}
		simon = new CSimon(SIMON_START_X, SIMON_START_Y);
		CGame::GetInstance()->InitKeyboard(simon);
		objects.push_back(simon);
	}

	{
		// Load objects
		auto items = data["items"]; 
		for (int i = 0; i < items.size(); i++)
		{
			LPTEXTURE texItem = textures->Get(items[i][1]);
			sprites->Add(items[i][0], items[i][2], items[i][3], items[i][4], items[i][5], texItem);
		}

		// Add brick
		LPANIMATION ani;
		ani = new CAnimation(100);
		ani->Add(ID_SPRITE_BRICK);
		animations->Add(ID_ANI_BRICK, ani);

		/*for (int i = 0; i < 40; i++)
		{
			CBrick* b = new CBrick(BRICK_X + i * BRICK_WIDTH, BRICK_Y - 50);
			objects.push_back(b);
		}
		for (int i = 41; i < 100; i++)
		{
			CBrick* b = new CBrick(BRICK_X + (i - 1) * BRICK_WIDTH, BRICK_Y + 100);
			objects.push_back(b);
		}*/

		auto bricksMap = mapData["brick"];
		for (int i = 0; i < bricksMap.size(); i++)
		{
			float x1 = bricksMap[i][0], x2 = bricksMap[i][2];
			float y1 = bricksMap[i][1], y2 = bricksMap[i][3];
			float brickWidth = bricksMap[i][4];
			float brickHeight = bricksMap[i][5];
			for (int i = x1; i <= x2; i += brickWidth)
			{
				for (int j = y1; j <= y2; j += brickHeight)
				{
					CBrick* brick = new CBrick(i, j);
					objects.push_back(brick);
				}
			}
		}
	}
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{

	vector<LPGAMEOBJECT> coObjects;

	for (auto obj : objects)
		coObjects.push_back(obj);

	for (int i = 0; i < (int)objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// Lấy vị trí của Simon
	float simonX, simonY;
	simon->GetPosition(simonX, simonY);

	CGame::GetInstance()->GetCamera()->FollowSimon(simonX, simonY);
}

void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

	for (int i = 0; i < (int)objects.size(); i++)
	{
		objects[i]->Render();
	}

	root->Render();

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			CGame::GetInstance()->ProcessKeyboard();
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetDebugWindow(hWnd);

	CGame* game = CGame::GetInstance();
	game->Init(hWnd, hInstance);



	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	LoadResources();

	Run();

	return 0;
}