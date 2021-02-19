// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "LevelFlow.h"

namespace pnk
{
    Level1Flow::Level1Flow()
    {
        _is_bg_ids.push_back("");


        _is_bg_id = "tiles_bg_png";
        _is_obj_id = "pnk_sprites_png";
        _l_bg_id = "lvl1_bg";
        _l_obj_id = "lvl1_obj";

        // viewport = 320 x 240 px
        roomflow room1 = {dang::Vector2F{320.0 / 2, 960 - 130}, 26, 2};
        roomflow room2 = {dang::Vector2F{320.0 + 320.0 / 2, 960 - 130}, 27, 2};

        _roomflows.push_back(room1);
        _roomflows.push_back(room2);
    }
}