// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <Vector2T.h>
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
         * l = layer
         * bg: background (i.e. tile layer)
         * obj: object (i.e. sprites and whatnot)
         */
        std::string         _is_bg_id;
        std::string         _is_obj_id;
        std::string         _l_bg_id;
        std::string         _l_obj_id;


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

    class Level1Flow : public LevelFlow
    {
    public:
        Level1Flow();
    };


}

