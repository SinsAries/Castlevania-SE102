#include "RigidBodyComponent.h"
#include "ColliderComponent.h" // Sẽ cần để lấy kích thước bounding box
#include "Game.h"             // Cần để lấy danh sách đối tượng va chạm (từ Grid/Quadtree)
#include "Candle.h"

#define GRAVITY 0.002f

RigidBodyComponent::RigidBodyComponent()
{
    // Khởi tạo vận tốc ban đầu
    this->vx = 0.0f;
    this->vy = 0.0f;
}

// Sửa lại hàm SweptAABBEx
LPCOLLISIONEVENT RigidBodyComponent::SweptAABBEx(LPGAMEOBJECT coO, DWORD dt)
{
    float sl, st, sr, sb;
    float ml, mt, mr, mb;
    float t, nx, ny;

    coO->GetBoundingBox(sl, st, sr, sb);

    // Lấy vận tốc của đối tượng kia một cách an toàn
    float svx = 0, svy = 0;
    RigidBodyComponent* other_rbody = coO->GetComponent<RigidBodyComponent>();
    if (other_rbody != nullptr) {
        other_rbody->GetSpeed(svx, svy);
    }
    // Nếu other_rbody là nullptr (vật thể tĩnh), svx và svy sẽ mặc định là 0.

    float sdx = svx * dt;
    float sdy = svy * dt;

    owner->GetBoundingBox(ml, mt, mr, mb);

    float m_dx = this->vx * dt;
    float m_dy = this->vy * dt;

    float rdx = m_dx - sdx;
    float rdy = m_dy - sdy;

    CGame::SweptAABB(
        ml, mt, mr, mb,
        rdx, rdy,
        sl, st, sr, sb,
        t, nx, ny
    );

    CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
    return e;
}

void RigidBodyComponent::CalcPotentialCollisions(
    const vector<LPGAMEOBJECT>& coObjects, // Dùng tham chiếu hằng
    vector<LPCOLLISIONEVENT>& coEvents,
    DWORD dt)
{
    // Sửa luôn cách truy cập từ -> thành .
    for (UINT i = 0; i < coObjects.size(); i++)
    {
        LPGAMEOBJECT other_obj = coObjects.at(i);

        // Bỏ qua các đối tượng đã bị xóa
        if (other_obj->IsDeleted()) continue;

        LPCOLLISIONEVENT e = SweptAABBEx(coObjects.at(i), dt);

        if (e->t > 0 && e->t <= 1.0f)
            coEvents.push_back(e);
        else
            delete e;
    }

    std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void RigidBodyComponent::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty, 
	float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix>=0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy>=0) coEventsResult.push_back(coEvents[min_iy]);
}

void RigidBodyComponent::Update(DWORD dt)
{
    // Hàm này chỉ tồn tại để RigidBodyComponent không còn là lớp trừu tượng.
    // Toàn bộ logic vật lý đã được xử lý trong hàm Update(dt, coObjects).
    // Vì vậy, chúng ta có thể để nó trống.
}

// Hàm Update MỚI và HOÀN CHỈNH
// Trong RigidBodyComponent.cpp

void RigidBodyComponent::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (!owner || owner->IsDeleted()) return;

    this->grounded = false;

    vector<LPCOLLISIONEVENT> coEvents;
    vector<LPCOLLISIONEVENT> coEventsResult;
    coEvents.clear();

    if (coObjects != nullptr)
    {
        // Bỏ bộ lọc cũ ở đây, ta muốn xét va chạm với tất cả mọi thứ
        CalcPotentialCollisions(*coObjects, coEvents, dt);
    }

    if (coEvents.size() == 0)
    {
        float x, y;
        owner->GetPosition(x, y);
        owner->SetPosition(x + vx * dt, y + vy * dt);
    }
    else
    {
        float min_tx, min_ty, nx = 0, ny = 0;
        float rdx = 0, rdy = 0;

        // TẠO MỘT DANH SÁCH VA CHẠM VỚI VẬT CẢN CỨNG (NON-TRIGGER)
        vector<LPCOLLISIONEVENT> solid_coEvents;
        for (LPCOLLISIONEVENT e : coEvents)
        {
            ColliderComponent* otherCollider = e->obj->GetComponent<ColliderComponent>();
            if (otherCollider && !otherCollider->IsTrigger())
            {
                solid_coEvents.push_back(e);
            }
        }

        // LỌC VA CHẠM CHỈ DỰA TRÊN CÁC VẬT CẢN CỨNG
        FilterCollision(solid_coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

        // Di chuyển đến vị trí va chạm gần nhất với VẬT CẢN CỨNG
        float x, y;
        owner->GetPosition(x, y);
        owner->SetPosition(x + min_tx * (vx * dt) + nx * 0.4f, y + min_ty * (vy * dt) + ny * 0.4f);

        // Cập nhật lại vận tốc chỉ dựa trên va chạm với VẬT CẢN CỨNG
        if (nx != 0) vx = 0;
        if (ny != 0)
        {
            vy = 0;
            if (ny < 0) this->grounded = true;
        }

        // GỌI ONCOLLISION CHO TẤT CẢ CÁC VA CHẠM (CẢ TRIGGER VÀ VẬT CẢN)
        // Điều này đảm bảo Simon vẫn "biết" mình đã đi qua item để nhặt
        for (UINT i = 0; i < coEvents.size(); i++)
        {
            LPCOLLISIONEVENT e = coEvents[i];
            // Để tối ưu, chỉ xử lý logic cho các va chạm thực sự xảy ra trong khoảng thời gian di chuyển
            if (e->t >= 0 && e->t <= min_tx) // min_tx là thời gian di chuyển thực tế
            {
                owner->OnCollision(e);
            }
        }
    }

    // Dọn dẹp bộ nhớ cho tất cả các sự kiện va chạm đã tạo
    for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}