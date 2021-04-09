// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <RectT.hpp>
#include <Vector2T.hpp>

#include <string>
#include <vector>
#include <unordered_map>

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

        struct act
        {
            /** size of room in tiles */
            dang::RectU         _extent{0, 0, 0, 0};

            /**
             * passages to other rooms
             * the hash key is the room number, the value is the position in the room
             * unit: relative tile coord. Meaning {0,0} is the top left corner of the room
             * */
            std::unordered_map<int32_t,dang::Vector2U>  _passage_from;
        };

        std::vector<act> _acts;

    };

    class L1SP : public ScreenPlay
    {
    public:
        L1SP();
    };

}

