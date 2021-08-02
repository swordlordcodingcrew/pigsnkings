// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include <RectT.hpp>
#include <Vector2T.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace pnk
{
    class ScreenPlay
    {
    public:

        /**
         * tmx layer ids
         * bg: background (i.e. tile layer)
         * mood: deco sprites
         * obj: object (i.e. sprites and whatnot)
         * hud: human user interface stuff
         * fg: foreground deco sprites
         */
        std::string         _l_bg_name;
        std::string         _l_obj_name;
        std::string         _l_mood_name;
        std::string         _l_hud_name;
        std::string         _l_fg_name;

        struct act
        {
            /** size of room in tiles */
            dang::RectU         _extent{0, 0, 0, 0};

            /** size of room in pixels */
            dang::RectF         _extent_pixels{0, 0, 0, 0};

            /** scenegraphs in the room. There can be more than one */
            std::vector<dang::spSceneGraph>        _scene_graphs;

            /**
             * passages to other rooms
             * the hash key is the room number to go, the value is the position in the room
             * unit: relative tile coord to the room. Meaning {0,0} is the top left corner of the room
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

    class L2SP : public ScreenPlay
    {
    public:
        L2SP();
    };

}

