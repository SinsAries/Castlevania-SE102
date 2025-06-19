// GameIDs.h — central registry of resource IDs
//
// NOTE
// -----
// * Keep each enum monotonically increasing so we can quickly locate free ranges.
// * SpriteID & AnimationID share the same numerical space in the original code
//   base, so avoid accidental overlaps.
// * When adding **new** art assets reserve a block of at least +10 IDs for future
//   variations (palette‑swap, destroy frames, etc.).
//
// Ranges we have carved out so far
//   0‑99      : internal / debug
//   100‑299   : textures (TextureID)
//   500‑799   : weapon animations
//   20000‑20999 : static sprites (brick, candle, environment …)
//   21000‑21999 : item sprites (hearts, money, sub‑weapon icons …)
//   2400‑2999  : Simon animations
//   10000‑10499: static environment animations (brick/candle break, etc.)
//   >11000     : enemy animations (future)
// -----------------------------------------------------------------------------

#pragma once

// ===== TEXTURE SHEETS ========================================================
//  Each logical texture (loaded once) gets an ID so that SpriteManager can map
//  (textureID, rect) ➜ LPDIRECT3DTEXTURE9
// -----------------------------------------------------------------------------

enum class TextureID {
    MARIO = 0,
    ENEMY = 10,
    MISC = 20,
    SIMON = 30,
    BACKGROUND = 40,
    FONT = 50,
    ITEM = 60,   // NEW: sheet that hosts hearts / money / sub‑weapons
    ENVIRONMENT = 70,    // NEW: bricks, candles, etc.
    BLACK = 80,
    TILESET_1 = 90
};

// ===== STATIC SPRITES ========================================================
//  One draw call (single frame). Different states (idle vs. broken) get their
//  own SpriteID so they can be swapped in Animation definitions.
// -----------------------------------------------------------------------------

enum class SpriteID {
    // 20000 — Environment ------------------------------------------------------
    BRICK_SOFT_IDLE = 20000,
    BRICK_SOFT_BROKEN = 20001,
    BRICK_HARD = 20002,
    BRICK_GROUND_STYLE_1 = 20003,

    CANDLE_GROUND_IDLE = 20010,
    CANDLE_GROUND_BROKEN = 20011,
    CANDLE_WALL_IDLE = 20012,
    CANDLE_WALL_BROKEN = 20013,

    BACKGROUND_TILE = 20020,

    // 21000 — Item pickups ----------------------------------------------------
    ITEM_SMALL_HEART = 21000,
    ITEM_LARGE_HEART = 21001,
    ITEM_MONEY_50 = 21002,
    ITEM_MONEY_100 = 21003,
    ITEM_MONEY_400 = 21004,

    ITEM_DAGGER_ICON = 21010,
    ITEM_AXE_ICON = 21011,
    ITEM_HOLYWATER_ICON = 21012,
    ITEM_STOPWATCH_ICON = 21013,
    ITEM_ROSARY = 21014,
    ITEM_DOUBLE_SHOT = 21015,
    ITEM_TRIPLE_SHOT = 21016,

    // 4000 — UI ---------------------------------------------------------------
    UI_Black_Background = 4000,
    UI_Health_Red = 4001,
    UI_Health_White = 4002,
    UI_Heart = 4003,

    // 5000 — Bitmap font (kept from original) ---------------------------------
    Font_A = 5000, Font_B, Font_C, Font_D, Font_E, Font_F, Font_G, Font_H,
    Font_I, Font_J, Font_K, Font_L, Font_M, Font_N, Font_O, Font_P, Font_Q,
    Font_R, Font_S, Font_T, Font_U, Font_V, Font_W, Font_X, Font_Y, Font_Z,
    Font_0 = 5100, Font_1, Font_2, Font_3, Font_4, Font_5, Font_6, Font_7,
    Font_8, Font_9,
    Font_Symbol_Dash = 5200
};

// ===== ANIMATIONS ============================================================
//  Multi‑frame sequences referencing SpriteID(s) defined above.
//  Keep IDs grouped by category to stay organised.
// -----------------------------------------------------------------------------

enum class AnimationID {
    // Simon ------------------------------------------------------
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

    // Weapon (whip, knife, etc.) --------------------------------
    WeaponWhipRight = 600,
    WeaponWhipLeft = 601,
    WeaponKnifeRight = 602,
    WeaponKnifeLeft = 603,
	WeaponDaggerRight = 604,
	WeaponDaggerLeft = 605,

    // Environment break / idle ---------------------------------
    BrickSoftIdle = 10000,
    BrickSoftBreak = 10001,

    GroundCandle = 10010,
    WallCandle = 10011,

    // Items flicker / spin -------------------------------------
    ItemLargeHeartFloat = 10100,
    ItemSmallHeartFloat = 10101,
    ItemMoneyBagRed = 10102,
	ItemMoneyBagPurple = 10103,
	ItemMoneyBagYellow = 10104,
    ItemRosarySpin = 10103,
	ItemDagger = 10110,

    // Placeholder for future enemies ---------------------------
    EnemyZombieWalk = 11000,
    EnemyZombieDie = 11001,

    EnemyPantherRun = 11100,
    EnemyPantherJump = 11101,

    EnemyBatFly = 11200,
    EnemyBatDie = 11201,

    BossBatPhase1 = 12000,
    BossBatPhase2 = 12001,

    Brick = 10000,
};

// ===== ITEM TYPES ============================================================
//  Logical categories for gameplay logic. These **do not** overlap numerical
//  ID space with SpriteID/AnimationID because they are not used for rendering.
// -----------------------------------------------------------------------------

enum class ItemType {
    SmallHeart,
    LargeHeart,
    Money50,
    Money100,
    Money400,
    Dagger,
    Axe,
    HolyWater,
    Stopwatch,
    Rosary,
    DoubleShot,
    TripleShot,
    OneUp
};

// ===== SIMON STATE MACHINE ===================================================

enum class SimonStateID {
    Idle,
    WalkRight,
    WalkLeft,
    Jump,
    Sit,
    Attack
};

// ===== SUB-WEAPON TYPES =====================================================
enum class ESubWeaponType
{
    None,
    Dagger,
    Axe,
    HolyWater,
    Stopwatch
};