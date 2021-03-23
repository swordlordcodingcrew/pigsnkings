// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <RectT.hpp>
#include <Vector2T.hpp>

#include <string>
#include <vector>

namespace pnk
{
    class ScreenPlay
    {
    public:

        /**
         * tmx ids
         * is = imagesheet
         * bg: background (i.e. tile layer)
         * obj: object (i.e. sprites and whatnot)
         */
        std::string         _l_bg_name;
        std::string         _l_obj_name;
        std::string         _l_mood_name;
        std::string         _l_hud_name;

        // TODO -> simple screenplay
        struct act
        {
            /** center of the room in the tiled-level; unit: float */
            dang::Vector2F      _room_center{0,0};

            /** size of room in tiles */
            dang::RectU         _room_extent{0, 0, 0, 0};

        };

        std::vector<act> _acts;

    };

    class L1SP : public ScreenPlay
    {
    public:
        L1SP();
    };

}

