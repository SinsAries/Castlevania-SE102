#include <fstream>
#include <iostream>

#include "json.hpp"
#include "PlayScene.h"
#include "Game.h"
#include "debug.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "Brick.h"
#include "KeyEventHandler.h"
#include "infoBoard.h"

using json = nlohmann::json;

extern std::wstring s2ws(const std::string& s);

CPlayScene::CPlayScene(int id, std::string filePath) : CScene(id)
{
	this->sceneFilePath = filePath + "/map.json";
	this->player = nullptr;
	this->background = nullptr;
	this->quadtree = nullptr;
	this->key_handler = nullptr;
	this->board = nullptr;

	this->score = 0;
	this->game_time = 400;
	this->stage_number = 1;
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from: %hs \n", sceneFilePath.c_str());

	std::ifstream file("./resource/resource.json");


	// Kiểm tra xem file có mở thành công không
	if (!file) {
		DebugOut(L"[ERROR] Failed to open scene file: %hs\n");
		return;
	}
	json data = json::parse(file);

	std::ifstream file2(sceneFilePath);


	// Kiểm tra xem file có mở thành công không
	if (!file2) {
		DebugOut(L"[ERROR] Failed to open file: %hs\n");
		return;
	}
	json mapData = json::parse(file2);

	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 0; i < data["textures"].size(); i++) {
		textures->Add(data["textures"][i][0], StringToLPCWSTR(data["textures"][i][1].get<string>()));
	}
	auto mapTexture = mapData["texture"];
	textures->Add(mapTexture[0], StringToLPCWSTR(mapTexture[1].get<string>()));

	{
		// Trong LoadResources()

		LPTEXTURE texBackground = textures->Get(static_cast<unsigned int>(TextureID::BACKGROUND)); // Hoặc texture riêng cho background
		auto tiles = mapData["tiles"];
		vector<int> ids;
		for (int i = 0; i < tiles.size(); i++) {
			sprites->Add(tiles[i][0], tiles[i][1], tiles[i][2],
				tiles[i][3], tiles[i][4], texBackground);
			ids.push_back(tiles[i][0]);
		}
		int height = mapData["map"].size(); 
		int width = mapData["map"][0].size(); // số cột


		// Cấp phát động int** map
		int** mapArray = new int* [height];
		for (int i = 0; i < height; i++) {
			mapArray[i] = new int[width];
			for (int j = 0; j < width; j++) {
				mapArray[i][j] = mapData["map"].at(i).at(j).get<int>();
			}
		}

		this->background = new CTiledBackground(0, 0, ids, mapArray, 32, height, width);
		this->objects.push_back(this->background);
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
		ani->Add(100);
		animations->Add(100, ani);


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

	std::map<std::string, TextureID> textureIdMap = {
		{"SIMON", TextureID::SIMON}, {"MISC", TextureID::MISC},
		{"BACKGROUND", TextureID::BACKGROUND}, {"FONT", TextureID::FONT}
	};

	// === 2. LOAD ASSETS (SPRITES & ANIMATIONS) ===
	DebugOut(L"[INFO] Loading assets from\n");
	std::map<std::string, int> sprite_registry_for_animation;
	std::ifstream f_assets("./resource/assets.json");
	if (!f_assets.is_open()) {
		DebugOut(L"[ERROR] Failed to open asset.json\n");
		return;
	}
	json data_assets = json::parse(f_assets);

	static std::map<std::string, int> masterSpriteIdMap;
	if (masterSpriteIdMap.empty()) {
		masterSpriteIdMap["ui_black"] = static_cast<int>(SpriteID::UI_Black_Background);
		masterSpriteIdMap["ui_health_red"] = static_cast<int>(SpriteID::UI_Health_Red);
		masterSpriteIdMap["ui_health_white"] = static_cast<int>(SpriteID::UI_Health_White);
		masterSpriteIdMap["ui_heart"] = static_cast<int>(SpriteID::UI_Heart);
		for (int i = 0; i < 26; i++) masterSpriteIdMap["font_" + std::string(1, 'A' + i)] = static_cast<int>(SpriteID::Font_A) + i;
		for (int i = 0; i < 10; i++) masterSpriteIdMap["font_" + std::string(1, '0' + i)] = static_cast<int>(SpriteID::Font_0) + i;
		masterSpriteIdMap["font_symbol_-"] = static_cast<int>(SpriteID::Font_Symbol_Dash);
	}

	for (auto& sprite_element : data_assets["sprites"].items()) {
		std::string spriteIdStr = sprite_element.key();
		json spriteData = sprite_element.value();

		int sprite_id_int;
		if (masterSpriteIdMap.count(spriteIdStr)) {
			sprite_id_int = masterSpriteIdMap[spriteIdStr];
		}
		else {
			static int dynamic_id = 7000;
			sprite_id_int = dynamic_id++;
		}

		std::string texIdStr = spriteData["textureId"];
		LPTEXTURE tex = textures->Get(static_cast<int>(textureIdMap[texIdStr]));
		json rect = spriteData["rect"];
		sprites->Add(sprite_id_int, rect[0].get<int>(), rect[1].get<int>(), rect[2].get<int>(), rect[3].get<int>(), tex);

		sprite_registry_for_animation[spriteIdStr] = sprite_id_int;
	}

	std::map<std::string, AnimationID> animationIdMap = {
		{"SimonWalkRight", AnimationID::SimonWalkRight}, {"SimonWalkLeft", AnimationID::SimonWalkLeft},
		{"SimonIdleRight", AnimationID::SimonIdleRight}, {"SimonIdleLeft", AnimationID::SimonIdleLeft},
		{"SimonSitRight", AnimationID::SimonSitRight}, {"SimonSitLeft", AnimationID::SimonSitLeft},
		{"SimonStandAttackRight", AnimationID::SimonStandAttackRight}, {"SimonStandAttackLeft", AnimationID::SimonStandAttackLeft},
		{"SimonSitAttackRight", AnimationID::SimonSitAttackRight}, {"SimonSitAttackLeft", AnimationID::SimonSitAttackLeft},
		{"WeaponWhipLeft", AnimationID::WeaponWhipLeft}, {"WeaponWhipRight", AnimationID::WeaponWhipRight},
		{"WeaponKnifeLeft", AnimationID::WeaponKnifeLeft}, {"WeaponKnifeRight", AnimationID::WeaponKnifeRight},
		{"Brick", AnimationID::Brick}
	};
	for (auto& anim_element : data_assets["animations"].items()) {
		std::string animIdStr = anim_element.key();
		json animData = anim_element.value();
		int frameTime = animData["defaultFrameTime"];
		LPANIMATION ani = new CAnimation(frameTime);
		for (const auto& spriteIdJson : animData["spriteIds"]) {
			std::string spriteIdStr = spriteIdJson.get<std::string>();
			ani->Add(sprite_registry_for_animation[spriteIdStr]);
		}
		animations->Add(static_cast<int>(animationIdMap[animIdStr]), ani);
	}

	//// === 3. LOAD SCENE OBJECTS ===
	//DebugOut(L"[INFO] Loading scene objects...\n");
	this->quadtree = new Quad(0, Point(0, 0), Point(1000, 1000));
	
	this->player = new CSimon(mapData["simon_start"][0], mapData["simon_start"][1]);
	this->objects.push_back(this->player);
	this->key_handler = this->player;
	board = new CInfoBoard();

	DebugOut(L"[INFO] Scene loaded successfully.\n");

}

void CPlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (auto obj : objects)
		coObjects.push_back(obj);

	for (int i = 0; i < (int)objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	if (player) {
		float cx, cy;
		player->GetPosition(cx, cy);
		CGame::GetInstance()->GetCamera()->FollowSimon(cx, cy);
	}

	if (player && board)
	{
		// Tạm thời dùng giá trị giả định, bạn sẽ cần các hàm Get... từ Simon
		board->Update(this->score, this->game_time, this->stage_number, 16, 16, 3);
	}
}

void CPlayScene::Render()
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

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}

	if (quadtree) {
		quadtree->Render();
	}

	if (board)
		board->Render();

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}

void CPlayScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();

	if (quadtree) {
		delete quadtree;
		quadtree = nullptr;
	}

	if (board) {
		delete board;
		board = nullptr;
	}

	player = nullptr;
	background = nullptr;

	DebugOut(L"[INFO] Scene unloaded.\n");
}