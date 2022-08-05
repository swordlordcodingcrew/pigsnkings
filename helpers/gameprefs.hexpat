// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game
//
// hexpat definition file to be used with ImHex editor
#pragma endian little

struct Prefs
        {
            u8      version [[color("0000FF")]];                // of the prefs for upgrading
            float   volume_track [[color("000FFF")]];
            float   volume_sfx [[color("00F0FF")]];
            u8      selectedModule [[color("FF0000")]];         // which module is set in the GSHome (where is the candle)
            u8      currentGameSaveSlot [[color("00FF00")]];    // 1-4
        };

Prefs prefs @ 0x00;
