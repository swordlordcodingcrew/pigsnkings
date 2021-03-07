// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <Vector2T.hpp>
#include <string>
#include <vector>

namespace pnk
{
    class LevelFlow
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
        struct roomflow
        {
            /**
             * center of the room in the tiled-level
             * unit: float
             */
            dang::Vector2F  room_center{0,0};
            u_int32_t       spawn_spr_with_id{0};
            u_int32_t       number_of_spawns{1};
        };

        std::vector<roomflow> _roomflows;

    };

    class L1F : public LevelFlow
    {
    public:
        L1F();
    };

    class L2F : public LevelFlow
    {
    public:
        L2F();
    };

}

