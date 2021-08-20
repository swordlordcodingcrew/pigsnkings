// (c) 2019-1 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level2SP.hpp"
#include "../actors/npc/Enemy.h"
#include "../actors/npc/HenchPig.h"
#include "../GSPlay.h"

#include <bt/NTBuilder.h>

namespace pnk
{
    Level2SP::Level2SP(GSPlay& gsp)
    {
        _l_bg_name = "lvl_2_bg";
        _l_obj_name = "lvl_2_obj";;
        _l_mood_name = "lvl_2_mood";
        _l_hud_name = "lvl_2_hud";
        _l_fg_name = "lvl_2_fg";

        // viewport = 320 x 240 px
        act room0;
        room0._extent = {49, 8, 11, 8}; // global coordinates
        /** the starting point of the level */
        room0._passage_from[-1] = {9, 1}; // local, no global coordinates
        /** when coming from room 1, start at given position */
        room0._passage_from[1] = {0, 1}; // local, no global coordinates
        _acts.push_back(room0);

        act room1;
        room1._extent = {40, 8, 10, 8};
        room1._passage_from[0] = {9, 1};
        room1._passage_from[2] = {0, 1};
        _acts.push_back(room1);

        act room2;
        room2._extent = {30, 8, 10, 8};
        room2._passage_from[1] = {9, 1};
        room2._passage_from[3] = {0, 14};
        _acts.push_back(room2);

        act room3;
        room3._extent = {20, 8, 10, 8};
        room3._passage_from[2] = {10, 14};
        room3._passage_from[4] = {0, 1};
        _acts.push_back(room3);

        act room4;
        room4._extent = {10, 8, 10, 8};
        room4._passage_from[3] = {10, 1};
        room4._passage_from[5] = {0, 14};
        _acts.push_back(room4);

        act room5;
        room5._extent = {0, 0, 10, 16};
        room5._passage_from[4] = {10, 15};
        room5._passage_from[6] = {10, 7};
        _acts.push_back(room5);

        act room6;
        room6._extent = {10, 0, 10, 8};
        room6._passage_from[5] = {0, 7};
        room6._passage_from[7] = {10, 1};
        _acts.push_back(room6);

        act room7;
        room7._extent = {10, 8, 10, 8};
        room7._passage_from[6] = {0, 1};
        room7._passage_from[8] = {10, 7};
        _acts.push_back(room7);

        /** behaviour trees */
        dang::spNTree tr = dang::NTBuilder{}
                .selector()
                .sequence()
                .leaf(Enemy::NTsetRandNeighbourWaypoint)
                .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .sequence()
                .leaf(Enemy::NTfindNearestWaypointH)
                .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .leaf(HenchPig::NTSleep)
                .end()
                .build();

        _bt["loiter"] = tr;

        dang::spNTree tr2 = dang::NTBuilder{}
                .selector()
                .sequence()
                .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1))
                .leaf(Enemy::NTsetWPNearHero)
                .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .sequence()
                .leaf(Enemy::NTsetRandNeighbourWaypoint)
                .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .sequence()
                .leaf(Enemy::NTfindNearestWaypointH)
                .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .leaf(HenchPig::NTSleep)
                .end()
                .build();
        _bt["loiter_towards_hero"] = tr2;

        dang::spNTree tr3 = dang::NTBuilder{}
                .selector()
                .sequence()
                .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1))
                .leaf(Enemy::NTsetWPNearHero)
                .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .leaf(HenchPig::NTNap)
                .end()
                .build();
        _bt["wait_for_hero"] = tr3;

        dang::spNTree tr4 = dang::NTBuilder{}
                .selector()
                .sequence()
                .leaf(Enemy::NTsetRandomPath)
                .leaf(Enemy::NTcheckPathCompleted)
                .leaf(HenchPig::NTSleep)
                .end()
                .sequence()
                .leaf(Enemy::NTfindNearestWaypointH)
                .leaf(Enemy::NTcheckPathCompleted)
                .leaf(HenchPig::NTSleep)
                .end()
                .leaf(HenchPig::NTSleep)
                .end()
                .build();
        _bt["lazy"] = tr4;


    }
}