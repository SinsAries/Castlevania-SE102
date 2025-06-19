		#include "AttackState.h"
		#include "IdleState.h"
		#include "SitState.h"      // Thêm để xử lý khi đang ngồi
		#include "Simon.h"
		#include "Animations.h"
		#include "Whip.h"
		#include "RigidBodyComponent.h"
		#include "SpriteRendererComponent.h"
		#include "Game.h"
		#include "PlayScene.h"
		#include "Utils.h"

		// --- Hàm khởi tạo mới ---
		AttackState::AttackState(bool isSubWeapon)
		{
			this->isSubWeaponAttack = isSubWeapon;
			this->hasSpawnedWeapon = false; // Luôn khởi tạo là false
			this->attackTime = CSimon::ATTACK_TIME_MS;
		}

		void AttackState::Enter(CSimon* simon)
		{
			simon->isAttacking = true;

			// Reset lại animation tấn công để nó chạy từ đầu
			int aniId = GetAttackAnimationId(simon);
			if (CAnimations::GetInstance()->Get(aniId))
			{
				CAnimations::GetInstance()->Get(aniId)->Reset();
			}

			// Chỉ dừng di chuyển khi đang trên mặt đất
			auto rbody = simon->GetComponent<RigidBodyComponent>();
			if (rbody && rbody->IsGrounded())
			{
				rbody->SetSpeed(0, rbody->GetVy()); // Chỉ set vx=0, giữ nguyên vy để xử lý trọng lực
			}
		}

		void AttackState::HandleInput(CSimon* simon, BYTE* states)
		{
			// Khi đang tấn công, Simon không nhận input di chuyển
		}

		// --- Hàm Update được nâng cấp ---
		void AttackState::Update(CSimon* simon, DWORD dt)
		{
			// Luôn giảm thời gian tấn công
			this->attackTime -= dt;

			// === PHÂN LUỒNG LOGIC TẤN CÔNG ===
			if (this->isSubWeaponAttack)
			{
				// --- Logic ném vũ khí phụ ---
				if (!this->hasSpawnedWeapon)
				{
					auto renderer = simon->GetComponent<SpriteRendererComponent>();
					// Chỉ ném vũ khí khi animation đạt đến frame nhất định (ví dụ frame 1)
					if (renderer && renderer->GetAnimation()->GetCurrentFrame() == 1)
					{
						simon->SpawnSubWeapon();
						this->hasSpawnedWeapon = true; // Đánh dấu đã ném để không ném lại
					}
				}
			}
			else
			{
				// --- Logic đánh bằng roi (như cũ) ---
				CWhip* whip = simon->GetWhip();
				CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
				if (whip && scene)
				{
					// Lấy danh sách các đối tượng có thể bị roi đánh trúng
					const auto& static_targets = scene->GetStaticObjects();
					whip->ProcessAttack(simon, static_targets);
					// (Bạn có thể cần thêm cả các đối tượng động như Enemy vào đây)
				}
			}


			// Khi hết thời gian tấn công, chuyển về trạng thái Idle hoặc Sit
			if (this->attackTime <= 0)
			{
				simon->isAttacking = false;

				// === THÊM ĐOẠN NÀY ĐỂ FIX LỖI ROI ===
				// Nếu đây là một đòn đánh roi (không phải sub-weapon),
				// thì phải reset roi khi kết thúc.
				if (!this->isSubWeaponAttack)
				{
					if (simon->GetWhip())
					{
						simon->GetWhip()->Reset();
					}
				}

				simon->attackCoolDown = CSimon::ATTACK_COOLDOWN_MS;
				// === KẾT THÚC PHẦN THÊM ===

				if (simon->isSitting)
				{
					simon->SetState(new SitState());
				}
				else
				{
					simon->SetState(new IdleState());
				}
			}
		}

		// --- Hàm Render không đổi ---
		void AttackState::Render(CSimon* simon)
		{
			auto simon_renderer = simon->GetComponent<SpriteRendererComponent>();
			if (simon_renderer)
			{
				int aniId = GetAttackAnimationId(simon);
				simon_renderer->SetAnimation(CAnimations::GetInstance()->Get(aniId));
			}
		}


		// --- Hàm tiện ích GetAttackAnimationId ---
		// Bạn có thể đã có hàm này ở đâu đó, nếu chưa thì đây là một ví dụ
		int AttackState::GetAttackAnimationId(CSimon* simon)
		{
			if (simon->isSitting)
			{
				return (simon->getNx() > 0) ?
					static_cast<int>(AnimationID::SimonSitAttackRight) :
					static_cast<int>(AnimationID::SimonSitAttackLeft);
			}
			else
			{
				return (simon->getNx() > 0) ?
					static_cast<int>(AnimationID::SimonStandAttackRight) :
					static_cast<int>(AnimationID::SimonStandAttackLeft);
			}
		}