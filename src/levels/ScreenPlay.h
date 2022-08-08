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
    class GSPlay;

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

        /** graphs per zones. key equals zone_nr*/
        std::unordered_map<uint16_t, std::vector<dang::spSceneGraph>>  _scene_graphs;

        /** function to find closest graph to the point*/
        dang::spSceneGraph findNearestGraph(const uint16_t zone_nr, const dang::Vector2F& pos);

        /** the behaviour trees in this level */
        std::unordered_map<std::string, dang::spNTree>  _bt;

        /** # of bubble-loops a pig is bubbled. When advancing the levels, the value could be diminished */
        uint8_t _bubble_loops = 6;

    protected:
        /** some common behaviour trees */
        dang::spNTree buildBackToPathH();
        dang::spNTree buildBackToPath();
        dang::spNTree buildLoiter();
        dang::spNTree buildBerserk();
        dang::spNTree buildLoiterWithSingleCrateH(GSPlay& gsp);
        dang::spNTree buildWaitWithCrate(GSPlay& gsp);
        dang::spNTree buildWaitWithBomb(GSPlay& gsp);
        dang::spNTree buildWaitWithBombH(GSPlay& gsp);


    };
}

