// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level4SP.hpp"

namespace pnk
{
    Level4SP::Level4SP(GSPlay &gsp)
    {
        _l_bg_name = "lvl_4_bg";
        _l_obj_name = "lvl_4_obj";;
        _l_mood_name = "lvl_4_mood";
        _l_hud_name = "lvl_4_hud";
        _l_fg_name = "lvl_4_fg";

        // generic loitering
        _bt["loiter"] = buildLoiter();
        _bt["berserk"] = buildBerserk();

    }
}