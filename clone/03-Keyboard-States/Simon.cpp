// Simon.cpp
#include "Simon.h"
#include "IdleState.h"
#include "SpriteRendererComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "Animations.h"
#include "PlatformerController.h"
#include "AttackState.h"       // <<< THÊM DÒNG NÀY
#include "WeaponFactory.h"     // <<< THÊM DÒNG NÀY
#include "PlayScene.h"         // <<< THÊM DÒNG NÀY
#include "Item.h"
#include "KnockbackState.h"

CSimon::CSimon(float x, float y) : CGameObject(x, y)
{
    // 1. LẮP RÁP CÁC COMPONENT CẦN THIẾT
    this->AddComponent(new ColliderComponent(16.0f, 30.0f));
	this->AddComponent(new PlatformerController()); // Thêm controller để xử lý di chuyển
    this->AddComponent(new RigidBodyComponent()); // Component vật lý

    // Lấy animation mặc định và tạo renderer
    LPANIMATION initial_animation = CAnimations::GetInstance()->Get(static_cast<int>(AnimationID::SimonIdleRight));
    this->AddComponent(new SpriteRendererComponent(initial_animation));

    // 2. KHỞI TẠO CÁC ĐỐI TƯỢNG MÀ SIMON SỞ HỮU
    this->whip = new CWhip();

    // 3. KHỞI TẠO CÁC GIÁ TRỊ BAN ĐẦU
    this->nx = 1;
    this->isSitting = false;
    this->isAttacking = false;
    this->attackCoolDown = 0;

    // 4. KHỞI TẠO STATE MACHINE
    this->currentState = nullptr; // Gán là null trước khi gọi SetState
    this->SetState(new IdleState());

    this->hearts = 5;
    this->score = 0;
    this->lives = 3;             // <<< THÊM
    this->max_health = 16;       // <<< THÊM
    this->health = max_health;   // <<< THÊM
	this->shotMultipler = 1; // Số lượng vũ khí phụ ném ra mỗi lần

    this->pvpCollisionCooldownTimer = 0;
}

void CSimon::SetKeyMappings(const KeyMappings& mappings)
{
    this->keyMappings = mappings;
}

// Trong Simon.cpp

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    // BƯỚC 1: CẬP NHẬT VẬT LÝ VÀ VỊ TRÍ TRƯỚC
    // Dòng này sẽ gọi RigidBodyComponent, tính toán va chạm và
    // cập nhật ra vị trí cuối cùng của Simon cho frame này.
    CGameObject::Update(dt, coObjects);

    // BƯỚC 2: CẬP NHẬT LOGIC STATE SAU
    // Bây giờ, khi State chạy, nó sẽ đọc được vị trí ĐÃ được cập nhật
    // của Simon, giúp tính toán vị trí roi chính xác.
    if (currentState)
        currentState->Update(this, dt);

    // BƯỚC 3: CẬP NHẬT CÁC TIMER (PHIÊN BẢN AN TOÀN)
    // Dùng if thay cho std::max để tránh mọi vấn đề
    if (subWeaponCoolDownTimer > 0)
    {
        subWeaponCoolDownTimer -= dt;
        if (subWeaponCoolDownTimer < 0)
            subWeaponCoolDownTimer = 0;
    }

    if (attackCoolDown > 0)
    {
        attackCoolDown -= dt;
        if (attackCoolDown < 0)
            attackCoolDown = 0;
    }

    if (pvpCollisionCooldownTimer > 0)
    {
        pvpCollisionCooldownTimer -= dt;
        if (pvpCollisionCooldownTimer < 0)
            pvpCollisionCooldownTimer = 0;
    }
}


// Sửa lại hàm này trong Simon.cpp
void CSimon::Render()
{
    // 1. Yêu cầu state hiện tại chọn và cài đặt animation đúng cho Simon
    if (currentState)
    {
        currentState->Render(this);
    }

    // 2. Gọi hàm Render của lớp cha để vẽ animation đã được state cài đặt
    CGameObject::Render();
	// 3. Vẽ roi nếu có
	if (whip)
	{
		whip->Render(); // Gọi hàm Render của CWhip
	}
}


void CSimon::SetState(ISimonState* state)
{
	if (currentState) delete currentState;
	currentState = state;
	currentState->Enter(this);
}

// THÊM HÀM HỦY ĐỂ TRÁNH RÒ RỈ BỘ NHỚ
CSimon::~CSimon()
{
	delete currentState;
	delete whip; // <<< THÊM DÒNG NÀY: Simon tự dọn dẹp roi khi bị hủy
}

CWhip* CSimon::GetWhip()
{
	return this->whip;
}

void CSimon::TryUseSubWeapon()
{
    // Điều kiện 1: Đang tấn công rồi thì không ném vũ khí
    if (isAttacking) return;

    // Điều kiện 2: Đang trong thời gian chờ (cooldown)
    if (subWeaponCoolDownTimer > 0) return;

    // Điều kiện 3: Không có vũ khí phụ hoặc không đủ hearts
    if (currentSubWeapon == ESubWeaponType::None || hearts < 1) return; // Giả sử tốn 1 heart

    // --- Nếu mọi điều kiện đều thỏa mãn ---
    this->hearts -= 1; // Trừ tim
    this->subWeaponCoolDownTimer = SUB_WEAPON_COOLDOWN_TIME; // Đặt lại cooldown

    // Chuyển sang trạng thái tấn công
    // Trạng thái này sẽ dùng chung animation với đánh roi, nhưng sẽ gọi hàm SpawnSubWeapon()
    SetState(new AttackState(true)); // Thêm cờ `isSubWeaponAttack`
}

// Hàm thực sự tạo ra vũ khí, sẽ được gọi từ AttackState ở frame phù hợp
void CSimon::SpawnSubWeapon()
{
    LPGAMEOBJECT weapon = CWeaponFactory::CreateSubWeapon(currentSubWeapon, this->x, this->y, this->nx);
    if (weapon)
    {
        // Thêm vũ khí vừa tạo vào danh sách đối tượng của scene/grid để được Update và Render
        dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddObject(weapon);
    }
}

void CSimon::OnCollision(LPCOLLISIONEVENT e)
{
    // Thử ép kiểu đối tượng va chạm thành CItem
    CItem* item = dynamic_cast<CItem*>(e->obj);

    // --- NẾU VA CHẠM VỚI MỘT ITEM ---
    if (item != nullptr)
    {
        // Lấy loại item
        ItemType itemType = item->GetType();

        // Xử lý hiệu ứng dựa trên loại item
        switch (itemType)
        {
        case ItemType::LargeHeart:
            this->AddHearts(5);
            break;
        case ItemType::SmallHeart:
            this->AddHearts(1);
            break;
        //case ItemType::WhipUpgrade:
        //    // this->UpgradeWhip(); // Ví dụ
        //    break;
        case ItemType::Dagger:
            this->SetSubWeapon(ESubWeaponType::Dagger);
            break;
        /*case ItemType::MoneyBagRed:
            this->AddScore(100);
            break;*/
            // Thêm các case khác cho các item khác...
        }

        // Sau khi xử lý, yêu cầu item tự hủy
        item->OnCollected();
    }

    CSimon* other_simon = dynamic_cast<CSimon*>(e->obj);
    if (other_simon != nullptr)
    {
        // QUAN TRỌNG: Kiểm tra cooldown của CẢ HAI người chơi.
        // Nếu một trong hai đang trong trạng thái không thể bị va chạm, ta bỏ qua.
        if (this->pvpCollisionCooldownTimer > 0 || other_simon->pvpCollisionCooldownTimer > 0)
        {
            return;
        }

        // Chỉ xử lý va chạm theo chiều ngang
        if (e->nx != 0)
        {
            // TẠI MỘT THỜI ĐIỂM, ÁP DỤNG HIỆU ỨNG CHO CẢ HAI

            // 1. Áp dụng trạng thái văng lùi cho Simon này (this).
            // Hướng văng của nó là e->nx.
            this->SetState(new KnockbackState(e->nx));

            // 2. Đồng thời, áp dụng trạng thái văng lùi cho Simon kia (other_simon).
            // Hướng văng của nó sẽ ngược lại, tức là -e->nx.
            other_simon->SetState(new KnockbackState(-e->nx));
        }
    }

    // --- NẾU VA CHẠM VỚI CÁC ĐỐI TƯỢNG KHÁC ---
    // Ví dụ: va chạm với Enemy
    // CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);
    // if (enemy != nullptr) {
    //     // Xử lý khi bị kẻ địch đụng phải
    // }
}

// --- Viết phần thân cho các hàm nhận hiệu ứng ---
void CSimon::AddHearts(int amount)
{
    this->hearts += amount;
    DebugOut(L"[INFO] Collected hearts! Current hearts: %d\n", this->hearts);
}

void CSimon::SetSubWeapon(ESubWeaponType subWeaponType)
{
    this->currentSubWeapon = subWeaponType;
    DebugOut(L"[INFO] Collected sub-weapon: %d\n", static_cast<int>(subWeaponType));
}

void CSimon::AddScore(int amount)
{
    this->score += amount; // Giả sử Simon có biến score
}