#include <fstream>
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
#include "GameUtils.h"
#include "Item.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "Tile.h"
#include "Utils.h"
#include <dinput.h>

using json = nlohmann::json;

extern std::wstring s2ws(const std::string& s);


CPlayScene::CPlayScene(int id, std::string filePath) : CScene(id)
{
	this->sceneFilePath = filePath;
	this->players.clear();
	this->grid = nullptr;
	this->key_handler = nullptr;
	this->board = nullptr;

	this->score = 0;
	this->game_time = 300;
	this->stage_number = 1;
	this->last_tick_count = 0;	
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from: %hs \n", sceneFilePath.c_str());

	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	std::ifstream f(sceneFilePath);
	if (!f) {
		DebugOut(L"[ERROR] Failed to open scene file: %hs\n", sceneFilePath.c_str());
		return;
	}
	json data = json::parse(f);



	std::string textures_path = data["textures_file"];
	std::string assets_path = data["assets_file"];

	// === 1. LOAD TEXTURES ===
	DebugOut(L"[INFO] Loading textures from %hs\n", textures_path.c_str());
	std::map<std::string, TextureID> textureIdMap = {
		{"SIMON", TextureID::SIMON}, {"MISC", TextureID::MISC},
		{"BACKGROUND", TextureID::BACKGROUND}, {"FONT", TextureID::FONT},
		{"BLACK", TextureID::BLACK},
		{"TILESET_1", TextureID::TILESET_1},
	};
	try {
		std::ifstream f_textures(textures_path);
		json data_textures = json::parse(f_textures);
		for (const auto& texData : data_textures) {
			textures->Add(static_cast<int>(textureIdMap[texData["id"]]), s2ws(texData["path"]).c_str());
		}
	}
	catch (json::parse_error& e) {
		DebugOut(L"[ERROR] JSON parse error in %hs: %hs\n", textures_path.c_str(), e.what());
		return;
	}

	DebugOut(L"[INFO] Loading assets from %hs\n", assets_path.c_str());
	std::map<std::string, int> sprite_registry_for_animation;
	try
	{
		std::ifstream f_assets(assets_path);
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
			masterSpriteIdMap["brick_stage0"] = static_cast<int>(SpriteID::BRICK_GROUND_STYLE_1);
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
			//{"WeaponWhipLeft", AnimationID::WeaponWhipLeft}, {"WeaponWhipRight", AnimationID::WeaponWhipRight},
			//{"WeaponDaggerRight", AnimationID::WeaponDaggerRight}, {"WeaponDaggerLeft", AnimationID::WeaponDaggerLeft},
			{"Brick", AnimationID::Brick}, {"GroundCandle", AnimationID::GroundCandle}, {"WallCandle",  AnimationID::WallCandle},
		};

		for (auto& anim_element : data_assets["animations"].items()) {
			std::string animIdStr = anim_element.key();
			json animData = anim_element.value();

			// Kiểm tra xem animation có tồn tại trong map không trước khi tiếp tục
			if (animationIdMap.find(animIdStr) == animationIdMap.end()) {
				continue; // Bỏ qua nếu không có trong map
			}

			int defaultFrameTime = animData["defaultFrameTime"];
			LPANIMATION ani = new CAnimation(defaultFrameTime);

			// Chỉ xử lý nếu có key "frames"
			if (animData.contains("frames"))
			{
				// Vòng lặp mới đọc cấu trúc "frames"
				for (const auto& frameData : animData["frames"]) {
					// Lấy spriteId (bắt buộc)
					std::string spriteIdStr = frameData["spriteId"].get<std::string>();
					int sprite_id_int = sprite_registry_for_animation[spriteIdStr];

					// Lấy các giá trị tùy chọn. Nếu không có trong JSON, dùng giá trị mặc định.
					int frame_time = frameData.value("time", defaultFrameTime);
					int anchor_x = frameData.value("anchorX", 0);
					int anchor_y = frameData.value("anchorY", 0);

					// Gọi hàm Add đã được nâng cấp của CAnimation
					ani->Add(sprite_id_int, frame_time, anchor_x, anchor_y);
				}
			}

			animations->Add(static_cast<int>(animationIdMap[animIdStr]), ani);
		}
	}
	catch (json::parse_error& e) {
		DebugOut(L"[ERROR] JSON parse error in %hs: %hs\n", assets_path.c_str(), e.what());
		return;
	}
	
	DebugOut(L"[INFO] Initializing Grid...\n");
	int map_width = data["map_info"]["width"];
	int map_height = data["map_info"]["height"];
	this->grid = CGrid::GetInstance();
	this->grid->Init(map_width, map_height);
	CGame* game = CGame::GetInstance();
	Camera* cam = game->GetCamera();

	DebugOut(L"[INFO] Loading tile map...\n");
	if (data.contains("tile_map"))
	{
		json tileMapData = data["tile_map"];
		std::string tilesetTexIdStr = tileMapData["tileset_texture_id"];
		int tile_width = tileMapData["tile_width"];
		int tile_height = tileMapData["tile_height"];
		
		LPTEXTURE tileset_texture_wrapper = textures->Get(static_cast<int>(textureIdMap[tilesetTexIdStr]));

		if (tileset_texture_wrapper == nullptr)
		{
			DebugOut(L"[FATAL ERROR] Tileset texture wrapper is NULL for key '%hs'. Check previous logs.\n", tilesetTexIdStr.c_str());
			return;
		}

		ID3D10Texture2D* raw_texture = tileset_texture_wrapper->getTexture2D();

		if (raw_texture == nullptr)
		{
			DebugOut(L"[FATAL ERROR] The raw DirectX texture is NULL inside the wrapper.\n");
			return;
		}

		D3D10_TEXTURE2D_DESC texDesc;
		raw_texture->GetDesc(&texDesc);
		int tileset_cols = texDesc.Width / tile_width;

		json layout = tileMapData["layout"];
		for (size_t i = 0; i < layout.size(); i++) {
			for (size_t j = 0; j < layout[i].size(); j++) {
				int tile_index = layout[i][j].get<int>();
				
				if (tile_index == -1) continue;

				int l = (tile_index % tileset_cols) * tile_width;
				int t = (tile_index / tileset_cols) * tile_height;
				int r = l + tile_width;
				int b = t + tile_height;

				DebugOut(L"%d %d %d %d %d\n", tile_index, l, t, r, b);

				// Truyền con trỏ wrapper vào CTile
				CTile* tile = new CTile((float)j * tile_width, (float)i * tile_height + 85, l, t, r, b, tileset_texture_wrapper);
				tiles.push_back(tile);
			}
			DebugOut(L"\n");
		}
	}

	DebugOut(L"[INFO] Loading game objects...\n");
	CSimon* player1 = new CSimon(100.0f, 10.0f); // Vị trí bắt đầu của P1
	KeyMappings p1_keys = {
		DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT, // up, down, left, right
		DIK_Z,  // jump (ví dụ phím Z)
		DIK_X,  // attack (ví dụ phím X)
		DIK_C   // subweapon (ví dụ phím C)
	};
	player1->SetKeyMappings(p1_keys);
	this->AddObject(player1);
	this->players.push_back(player1);


	// Tạo người chơi 2 (điều khiển bằng WASD)
	CSimon* player2 = new CSimon(150.0f, 10.0f); // Vị trí bắt đầu của P2
	KeyMappings p2_keys = {
		DIK_W, DIK_S, DIK_A, DIK_D, // up, down, left, right
		DIK_G, // jump
		DIK_H, // attack
		DIK_J  // subweapon
	};
	player2->SetKeyMappings(p2_keys);
	this->AddObject(player2);
	this->players.push_back(player2);

	for (const auto& objData : data["objects"])
	{
		std::string type = objData["type"];
		LPGAMEOBJECT obj = nullptr;

		if (type == "Candle") {
			auto pos = objData["position"];
			std::string itemTypeStr = objData["itemType"];
			ItemType itemType = StringToItemType(itemTypeStr);

			//obj = new CCandle(pos["x"].get<float>(), pos["y"].get<float>(), itemType);

			this->AddObject(obj, true); // Thêm vào danh sách tĩnh và Grid
		}
		else if (type == "Bricks")
		{
			auto props = objData["properties"];
			float startX = props["startX"].get<float>();
			float startY = props["startY"].get<float>();
			int count = props["count"].get<int>();
			float cellWidth = props["cellWidth"].get<float>();
			for (int i = 0; i < count; i++)
			{
				float brickX = startX + i * cellWidth;

				LPGAMEOBJECT brick = new CBrick(brickX, startY);

				this->AddObject(brick, true);
			}
			continue;
		}
	}

	board = new CInfoBoard(this->GetPlayer(0), this);
	DebugOut(L"[INFO] Scene loaded successfully.\n");
}

void CPlayScene::AddObject(LPGAMEOBJECT obj, bool is_static)
{
	if (is_static) {
		static_objects.push_back(obj);
	}
	else {
		dynamic_objects.push_back(obj);
	}
}

// Trong CPlayScene.cpp

void CPlayScene::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	BYTE* keyStates = game->GetKeyStates();
	for (CSimon* p : players)
	{
		// Ủy quyền xử lý input cho State của từng người chơi
		if (p && p->currentState)
		{
			p->currentState->HandleInput(p, keyStates);
		}
	}

	// --- Giai đoạn 1: Cập nhật các đối tượng ---

	// Tạo một danh sách TẤT CẢ các đối tượng có thể va chạm
	vector<LPGAMEOBJECT> collidable_objects;
	collidable_objects.insert(collidable_objects.end(), static_objects.begin(), static_objects.end());
	collidable_objects.insert(collidable_objects.end(), dynamic_objects.begin(), dynamic_objects.end());
	for (size_t i = 0; i < dynamic_objects.size(); i++)
	{
		dynamic_objects[i]->Update(dt, &collidable_objects);
	}

	for (size_t i = 0; i < static_objects.size(); i++)
	{
		static_objects[i]->Update(dt, nullptr);
	}

	CSimon* main_player = GetPlayer(0);
	if (main_player) {
		float cx, cy;
		main_player->GetPosition(cx, cy);
		CGame::GetInstance()->GetCamera()->FollowSimon(cx, cy);
	}

	if (game_time > 0)
	{
		// Lấy thời gian hệ thống hiện tại
		DWORD now = GetTickCount64();

		// Nếu đây là lần đầu tiên chạy, ta cần khởi tạo mốc thời gian
		if (last_tick_count == 0)
		{
			last_tick_count = now;
		}
		// Kiểm tra xem đã đủ 1000ms trôi qua kể từ mốc cuối chưa
		else if (now - last_tick_count >= 1000)
		{
			game_time--;
			last_tick_count = now; // Cập nhật lại mốc thời gian mới
		}
	}
	dynamic_objects.erase(
		std::remove_if(dynamic_objects.begin(), dynamic_objects.end(), [](LPGAMEOBJECT obj) {
			if (obj->IsDeleted()) {
				delete obj;
				return true;
			}
			return false;
			}),
		dynamic_objects.end()
	);

	static_objects.erase(
		std::remove_if(static_objects.begin(), static_objects.end(), [](LPGAMEOBJECT obj) {
			if (obj->IsDeleted()) {
				delete obj;
				return true;
			}
			return false;
			}),
		static_objects.end()
	);
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

	for (auto& tile : tiles)
	{
		tile->Render();
	}

	// Vẽ các vật thể tĩnh (gạch, nến...)
	for (auto& obj : static_objects)
	{
		obj->Render();
	}

	// Vẽ các vật thể động (simon, enemy, item...)
	for (auto& obj : dynamic_objects)
	{
		obj->Render();
	}

	if (board)
		board->Render();

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}

void CPlayScene::Unload()
{
	// Xóa các object động
	for (auto& obj : dynamic_objects)
	{
		delete obj;
	}
	dynamic_objects.clear();

	// Xóa các object tĩnh
	for (auto& obj : static_objects)
	{
		delete obj;
	}
	static_objects.clear();

	// Xóa sạch các con trỏ trong Grid, nhưng không xóa Grid Singleton
	if (grid != nullptr)
	{
		grid->Clear();
	}

	if (board) {
		delete board;
		board = nullptr;
	}

	players.clear();

	DebugOut(L"[INFO] Scene unloaded.\n");
}