// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

//#define PNK_DEBUG
//#define PNK_SND_DEBUG

//#define PNK_DEBUG_PRINT

#ifdef PNK_DEBUG_PRINT
    #ifdef TARGET_32BLIT_HW
        #define DEBUG_PRINT(...) blit::debug(__VA_ARGS__)
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
    ETG_NEW_THROWN_BOMB = 21,
    ETG_NEW_FIRED_CANNON = 22,
    ETG_NEW_POOF = 23,
    ETG_REWARD_HIT = 30,
    ETG_CRATE_EXPLODES = 50,
    ETG_BOMB_EXPLODES = 51,
    ETG_CANNONBALL_EXPLODES = 52,
    ETG_KING_HIT = 60,
    ETG_CHANGE_ROOM = 101,
    ETG_WARP_ROOM = 102,
    ETA_PAUSE = 201
};

