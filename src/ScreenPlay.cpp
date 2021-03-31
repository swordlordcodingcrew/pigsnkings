// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "ScreenPlay.h"

namespace pnk
{
    L1SP::L1SP()
    {
        _l_bg_name = "lvl_1_bg";
        _l_obj_name = "lvl_1_obj";;
        _l_mood_name = "lvl_1_mood";
        _l_hud_name = "lvl_1_hud";

        // viewport = 320 x 240 px
        act room1;
        room1._extent = {0, 16, 10, 8};
        room1._starting_position = {0, 5};
        _acts.push_back(room1);

        act room2;
        room2._extent = {9, 16, 11, 8};
        room1._starting_position = {0, 2};
        _acts.push_back(room2);

        act room3;
        room3._extent = {19, 16, 11, 8};
        room1._starting_position = {0, 3};
        _acts.push_back(room3);

    }
}