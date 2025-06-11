#pragma once

enum class TextureID {
	MARIO = 0,
	ENEMY = 10,
	MISC = 20,
	SIMON = 30,
	BACKGROUND = 40,
	FONT = 50,
	BRICK = 100
};

enum class SpriteID {
	BRICK = 20001,
	BACKGROUND = 20002,

	// ID cho các sprite của UI
	UI_Black_Background = 4000,
	UI_Health_Red,
	UI_Health_White,
	UI_Heart,
	// Thêm các sprite cho sub-weapon nếu có...

	// ID cho bộ Font (26 chữ cái + 10 chữ số + ký tự đặc biệt)
	Font_A = 5000,
	Font_B, Font_C, Font_D, Font_E, Font_F, Font_G, Font_H, Font_I, Font_J,
	Font_K, Font_L, Font_M, Font_N, Font_O, Font_P, Font_Q, Font_R, Font_S,
	Font_T, Font_U, Font_V, Font_W, Font_X, Font_Y, Font_Z,
	Font_0 = 5100,
	Font_1, Font_2, Font_3, Font_4, Font_5, Font_6, Font_7, Font_8, Font_9,
	Font_Symbol_Dash = 5200 // Dấu gạch ngang
};

enum class AnimationID {
	// Simon Animations
	SimonIdleRight = 2400,
	SimonIdleLeft = 2401,
	SimonWalkRight = 2500,
	SimonWalkLeft = 2501,
	SimonStandAttackRight = 2600,
	SimonStandAttackLeft = 2601,
	SimonJumpRight = 2700,
	SimonJumpLeft = 2701,
	SimonSitAttackRight = 2800,
	SimonSitAttackLeft = 2801,
	SimonSitRight = 2900,
	SimonSitLeft = 2901,

	// Weapon Animations
	WeaponWhipRight = 600,
	WeaponWhipLeft = 601,
	WeaponKnifeRight = 602,
	WeaponKnifeLeft = 603,

	// Other Animations
	Brick = 10000,
};

enum class SimonStateID {
	Idle,
	WalkRight,
	WalkLeft,
	Jump,
	Sit,
	Attack
};
