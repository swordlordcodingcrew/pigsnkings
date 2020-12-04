// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

//#define PNK_DEBUG

/**
 * hero consts
 */
const float H_WALK_VEL = 10;
const float H_JUMP_VEL = -16;
const uint32_t H_JUMP_COUNT = 40;
const float BUBBLE_VEL = 20;

/**
 * definition of _type_num of sprites
 */
enum TN
{
    TN_HERO = 10,
    TN_BUBBLE = 50,

    TN_ENEMY1 = 101,
    TN_ENEMY2 = 102,

    TN_HOTRECT = 201,
    TN_DEADLY_HOTRECT = 202
};

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
    ETA_PAUSE = 100

};

