// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

//#define PNK_DEBUG


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

