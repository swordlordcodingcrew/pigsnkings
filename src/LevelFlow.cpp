// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "LevelFlow.h"

namespace pnk
{
    L1F::L1F()
    {
        _l_bg_name = "lvl_1_bg";
        _l_obj_name = "lvl_1_obj";;
        _l_mood_name = "lvl_1_mood";
        _l_hud_name = "lvl_1_hud";

        // viewport = 320 x 240 px
        roomflow room1 = {dang::Vector2F{320.0 / 2, 960 - 130}, 26, 2};
//        roomflow room2 = {dang::Vector2F{320.0 + 320.0 / 2, 960 - 130}, 27, 2};

        _roomflows.push_back(room1);
//        _roomflows.push_back(room2);

    }
}