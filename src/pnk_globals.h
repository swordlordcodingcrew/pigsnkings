// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <cstdint>
#include <libs/32blit-sdk/32blit/graphics/surface.hpp>
#include <libs/32blit-sdk/32blit/engine/input.hpp>

//#define PNK_DEBUG
//#define PNK_SND_DEBUG
//#define PNK_DEBUG_PRINT


#ifdef PNK_DEBUG_PRINT
    #ifdef TARGET_32BLIT_HW
        #define DEBUG_PRINT(...) blit::debugf(__VA_ARGS__)
    #else
        #define DEBUG_PRINT(...) std::printf(__VA_ARGS__)
    #endif
#else
    #define DEBUG_PRINT(...)
#endif

/**
 * Definition of event filters
 */
enum EF
{
    EF_SYSTEM = 0x0001,
    EF_GAME = 0x0002,
    EF_APPL = 0x0004
};

/**
 * definition of event types
 * ETG: game events
 * ETA: application events
 */
enum ET
{
    ETG_NEW_BUBBLE = 10,
    ETG_REMOVE_SPRITE = 11,
    ETG_NEW_THROWN_CRATE = 20,
    ETG_NEW_DROP_CRATE = 21,
    ETG_NEW_THROWN_BOMB = 22,
    ETG_NEW_DROP_BOMB = 23,
    ETG_NEW_FIRED_CANNON = 24,
    ETG_NEW_POOF = 25,
    ETG_REWARD_HIT = 30,
    ETG_CRATE_EXPLODES = 50,
    ETG_BOMB_EXPLODES = 51,
    ETG_CANNONBALL_EXPLODES = 52,
    ETG_KING_HIT = 60,
    ETG_BOSS_HIT = 70,
    ETG_BOSS_DIES = 71,
    ETG_CHANGE_ROOM = 101,
    ETG_WARP_ROOM = 102,
    ETG_CHANGE_LEVEL = 103,
    ETG_START_BOSSBATTLE= 104,
    ETG_SAVEPOINT_TRIGGERED= 105,
    ETG_GAME_OVER = 200,
    ETG_GAME_ENDED = 201,
    ETA_PAUSE = 202
};

/**
 * definition of sprite types
 * ST: sprite type
 */
enum SpriteType
{
    ST_UNDEFINED = 0,

    // 1 - 9 king and associated stuff to king
    ST_KING = 1,
    ST_BUBBLE = 5,

    // 10 - 39 enemies
    ST_ENEMIES = 10,
    ST_PIG_NORMAL = 11,
    ST_NORMAL_PIG_HIVE = 12,
    ST_PIG_BETTER = 13,
    ST_PIG_CRATE = 14,
    ST_PIG_BOMB = 15,
    ST_PIG_CANNON = 16,
    ST_CANNON = 17,
    ST_FLYING_CANNONBALL = 18,
    ST_FLYING_CRATE = 19,
    ST_FLYING_BOMB = 20,
    ST_PIG_BOSS = 30,
    ST_EXPLOSION = 38,
    ST_ENEMIES_END = 39,

    // 40 - 49 hotrects
    ST_HOTRECT = 40,
    ST_HOTRECT_PLATFORM = 41,

    // 50 - 69 coins and rewards in general
    ST_REWARDS = 50,
    ST_COIN_SILVER = 51,
    ST_COIN_GOLD = 52,
    ST_GEM_BLUE = 53,
    ST_GEM_GREEN = 54,
    ST_GEM_RED = 55,
    ST_POTION_BLUE = 56,
    ST_POTION_RED = 57,
    ST_POTION_GREEN = 58,
    ST_PIG_REWARD = 59,
    ST_REWARDS_END = 69,

    // 70 - 79 triggers
    ST_TRIGGERS = 70,
    ST_ROOM_TRIGGER = 71,
    ST_WARP_ROOM_TRIGGER = 72,
    ST_LEVEL_TRIGGER = 73,
    ST_BOSS_BATTLE_TRIGGER = 74,
    ST_SAVEPOINT_TRIGGER = 75,
    ST_TRIGGERS_END = 79,

    // 80 - 89 Mood stuff
    ST_PIG_POOF = 81,
    ST_CANNON_MUZZLE = 82
};


/**
 * definition of waypoint depot types
 */
enum e_waypoint_type
{
    WPT_NONE = 0x0,
    WPT_BOMBDEPOT = 0x1,
    WPT_CRATEDEPOT = 0x2,
    WPT_POI = 0x4
};


/**
 * game consts
 */

static inline const float H_WALK_VEL{7};           // hero walk velocity
static inline const float H_JUMP_VEL{-16};         // hero jump velocity
static inline const uint32_t H_JUMP_COUNT{30};     // max jump cyclies
static inline const float BUBBLE_VEL{20};          // bubble horizontal velocity
static inline const float BUBBLE_VEL_UP{-1.5};     // bubble vertical velocity

static inline const float E_WALK_VEL{2};           // enemy walk velocity

static inline const float CRATE_VEL{20};           // crate throwing velocity
static inline const float CRATE_DROP_VEL{5};       // crate dropping velocity

static inline const float BOMB_VEL{24};           // bomb throwing velocity
static inline const float BOMB_DROP_VEL{5};       // bomb dropping velocity

static inline const uint8_t FIRST_GAME_SAVE_SLOT{1}; // number of first game save slot available
static inline const uint8_t LAST_GAME_SAVE_SLOT{4}; // number of last game save slots available

static inline const blit::Pen FADE_COL{0,0,0};  // fading colour
static inline const uint8_t FADE_STEP{8};           // fade step (fading ist from 0 to 255)

/** damage params */
static inline const uint8_t DAMAGE_PIG_NORMAL{30};         // when touching a normal pig
static inline const uint8_t DAMAGE_PIG_BOMB{35};           // when touching a pig with a bomb
static inline const uint8_t DAMAGE_PIG_CRATE{35};          // when touching a pig with a crate
static inline const uint8_t DAMAGE_FLYING_BOMB{20};        // when hit with a flying bomb
static inline const uint8_t DAMAGE_FLYING_CRATE{20};       // when hit with a flying crate
static inline const uint8_t DAMAGE_FLYING_CANNONBALL{40};  // when hit with a flying cannonball
static inline const uint8_t DAMAGE_CANNON{40};             // when touching a cannon
static inline const uint8_t DAMAGE_EXPLOSION{50};          // when hit with an explosion
static inline const uint8_t DAMAGE_FROM_PIGBOSS{41};            // when toucing the pig boss
static inline const uint8_t DAMAGE_TO_PIGBOSS{34};            // when hitting the pig boss

// hero params
static inline const uint8_t HERO_MAX_LIVES{3};            // max lives hero can gain
static inline const uint8_t HERO_MAX_HEALTH{100};            // max health hero can gain

// boss params
static inline const uint8_t BOSS_MAX_HEALTH{100};            // max health boss can gain

// reward gain params
static inline const uint8_t REWARD_POINT_COIN_SILVER{10};
static inline const uint8_t REWARD_POINT_COIN_GOLD{50};
static inline const uint8_t REWARD_POINT_GEM_BLUE{30};
static inline const uint8_t REWARD_POINT_GEM_GREEN{60};
static inline const uint8_t REWARD_POINT_GEM_RED{100};
static inline const uint8_t REWARD_POINT_POTION_BLUE{1};
static inline const uint8_t REWARD_POINT_POTION_RED{5};
static inline const uint8_t REWARD_POINT_POTION_GREEN{20};
static inline const uint8_t REWARD_POINT_PIG_REWARD{100};

/** boss battle params */
static inline const uint32_t BOSS_RECOVER_TIME{3000};           // in ms. Duration of recovery

/** button settings */
static inline const blit::Button BTN_OK{blit::Button::X};
static inline const blit::Button BTN_BACK{blit::Button::Y};
static inline const blit::Button BTN_CANCEL{blit::Button::Y};
static inline const blit::Button BTN_EXIT{blit::Button::MENU};
static inline const blit::Button BTN_JUMP{blit::Button::A};
static inline const blit::Button BTN_BUBBLE{blit::Button::B};
