// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game
//
// hexpat definition file to be used with ImHex editor
#pragma endian little

#include <std/mem.pat>

u32 filesize @ 0x04 [[color("FF00FF")]];

struct Gamestate
{
    u8      version [[color("0000FF")]];        // of the gamestate for upgrading
    u8      placeholder1;
    u8      placeholder2;
    u8      placeholder3;
    u32     save_size [[color("C0C0C0")]];      // size of saved gamestate (v2)
    s8      lives [[color("FF00FF")]];
    s8      health [[color("00FF00")]];         // percent
    u8      placeholder4;
    u8      placeholder5;
    u32     score [[color("00A0A0")]];
    u32     high_score [[color("00FFFF")]];
    u16     saved_level [[color("FF0000")]];
    u16     saved_room [[color("FF00FF")]];
    s8      boss_health [[color("FF0000")]];    // percent
    bool    has_cheated [[color("0000FF")]];    // persistent
    bool    invincible [[color("00FFFF")]];     // this is global, since can be set without having loaded a gamestate from a slot
	u8		placeholder6;
};

struct KilledEnemy
{
    u16		killed_enemy; // ID of killed enemy
};

Gamestate gamestate @ 0x00;
KilledEnemy killed_enemies[while (!std::mem::eof())] @$;


