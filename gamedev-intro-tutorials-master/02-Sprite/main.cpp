/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 02 - SPRITE AND ANIMATION

	This sample illustrates how to:

		1/ Render a sprite (within a sprite sheet)
		2/ How to manage sprites/animations in a game
		3/ Enhance CGameObject with sprite animation
================================================================ */

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "debug.h"
#include "Game.h"
#include "Textures.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Animation.h"
#include "Animations.h"


#include "Mario.h"
#include "Simon.h"


#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"02 - Sprite animation"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255,0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_SIMON 30

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc_transparent.png"
#define TEXTURE_PATH_ENEMIES TEXTURES_DIR "\\enemies.png"
#define TEXTURE_PATH_SIMON TEXTURES_DIR "\\simon.png"

CMario *mario;
#define MARIO_START_X 10.0f
#define MARIO_START_Y 50.0f
#define MARIO_START_VX 0.1f

CBrick *brick;

CSimon* simon;
CSimon* simon2;
#define SIMON_START_X 10.0f
#define SIMON_START_Y 130.0f
#define SIMON_START_VX 0.1f

vector<LPGAMEOBJECT> objects;

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

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	//textures->Add(ID_ENEMY_TEXTURE, TEXTURE_PATH_ENEMIES, D3DCOLOR_XRGB(156, 219, 239));
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_SIMON, TEXTURE_PATH_SIMON);


	CSprites * sprites = CSprites::GetInstance();
	
	LPTEXTURE texSimon = textures->Get(ID_TEX_SIMON);

	sprites->Add(10031, 1519, 21, 1534, 52, texSimon);
	sprites->Add(10032, 1536, 21, 1551, 52, texSimon);
	sprites->Add(10033, 1553, 21, 1568, 52, texSimon);

	sprites->Add(10041, 29, 21, 44, 52, texSimon);
	sprites->Add(10042, 46, 21, 61, 52, texSimon);
	sprites->Add(10043, 63, 21, 78, 52, texSimon);

	CAnimations* animations = CAnimations::GetInstance();
	LPANIMATION aniSimon;

	aniSimon = new CAnimation(100);
	aniSimon->Add(10031);
	aniSimon->Add(10032);
	aniSimon->Add(10033);
	animations->Add(540, aniSimon);

	aniSimon = new CAnimation(100);
	aniSimon->Add(10041);
	aniSimon->Add(10042);
	aniSimon->Add(10043);
	animations->Add(541, aniSimon);

	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	sprites->Add(10101, 246, 154, 259, 181, texMario);
	sprites->Add(10102, 275, 154, 290, 181, texMario);
	sprites->Add(10103, 304, 154, 321, 181, texMario);

	sprites->Add(10111, 186, 154, 200, 181, texMario);
	sprites->Add(10112, 155, 154, 171, 181, texMario);
	sprites->Add(10113, 125, 154, 141, 181, texMario);

	LPANIMATION ani;
	
	ani = new CAnimation(100);
	ani->Add(10101);
	ani->Add(10102);
	ani->Add(10103);
	animations->Add(2830, ani);
	
	ani = new CAnimation(100);
	ani->Add(10111);
	ani->Add(10112);
	ani->Add(10113);
	animations->Add(2831, ani);
	
	
	brick = new CBrick(100.0f, 100.0f);
	for (int i = 0; i < 10; i++) {
		simon = new CSimon(SIMON_START_X, SIMON_START_Y, 0.1f);
		mario = new CMario(MARIO_START_X, MARIO_START_Y + 32 * (i / 2), MARIO_START_VX);
		if (i % 2)
			objects.push_back(simon);
		else
			objects.push_back(mario);
	}
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	for (auto& object : objects) {
		object->Update(dt);
	}
}

void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		// clear the background 
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		// Use Alpha blending for transparent sprites
		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		//brick->Render();
		for (auto& object : objects) {
			object->Render();
		}

		// Uncomment this line to see how to draw a porttion of a texture  
		//g->Draw(10, 10, texMisc, 300, 117, 316, 133);


		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
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
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
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
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

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

	CGame *game = CGame::GetInstance();
	game->Init(hWnd);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}