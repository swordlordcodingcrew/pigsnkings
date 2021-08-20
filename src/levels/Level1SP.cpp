// (c) 2019-1 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level1SP.hpp"
#include "../actors/npc/Enemy.h"
#include "../actors/npc/HenchPig.h"
#include "../GSPlay.h"

#include <bt/NTBuilder.h>

namespace pnk
{
    Level1SP::Level1SP(GSPlay& gsp)
    {
        _l_bg_name = "lvl_1_bg";
        _l_obj_name = "lvl_1_obj";;
        _l_mood_name = "lvl_1_mood";
        _l_hud_name = "lvl_1_hud";
        _l_fg_name = "lvl_1_fg";

        // viewport = 320 x 240 px
        act room0;
        room0._extent = {0, 16, 10, 8};
        /** the starting point of the level */
        room0._passage_from[-1] = {0, 5};
        /** when coming from room 1, start at given position */
        room0._passage_from[1] = {8, 1};
        _acts.push_back(room0);

        act room1;
        room1._extent = {9, 16, 11, 8};
        room1._passage_from[0] = {0, 1};
        room1._passage_from[2] = {10, 2};
        _acts.push_back(room1);

        act room2;
        room2._extent = {19, 16, 11, 8};
        room2._passage_from[1] = {1, 2};
        room2._passage_from[3] = {10, 1};
        _acts.push_back(room2);

        act room3;
        room3._extent = {20, 8, 10, 8};
        room3._passage_from[2] = {0, 6};
        room3._passage_from[4] = {0, 1};
        _acts.push_back(room3);

        act room4;
        room4._extent = {9, 0, 21, 8};
        room4._passage_from[3] = {19, 2};
        room4._passage_from[5] = {0, 2};
        room4._passage_from[7] = {6, 6};
        _acts.push_back(room4);

        act room5;
        room5._extent = {0, 0, 10, 8};
        room5._passage_from[4] = {8, 2};
        room5._passage_from[6] = {0, 6};
        _acts.push_back(room5);

        act room6;
        room6._extent = {0, 8, 10, 8};
        room6._passage_from[5] = {4, 1};
        _acts.push_back(room6);

        act room7;
        room7._extent = {10, 8, 10, 8};
        room7._passage_from[4] = {1, 7};
        _acts.push_back(room7);

        // behaviour tree for finding back to the path system
        dang::spNTree back_to_path_h = dang::NTBuilder{}
            .sequence()
                .leaf(Enemy::NTfindNearestWaypointH)
                .leaf(Enemy::NTcheckPathCompleted)
            .end()
        .build();

        dang::spNTree back_to_path = dang::NTBuilder{}
            .sequence()
                .leaf(Enemy::NTfindNearestWaypoint)
                .leaf(Enemy::NTcheckPathCompleted)
            .end()
        .build();

        // generic loitering
        _bt["loiter"] = dang::NTBuilder{}
            .sequence()
                .selector()
                    .sequence()
                        .leaf(Enemy::NTsetRandomPath)
                        .leaf(Enemy::NTcheckPathCompleted)
                    .end()
                    .tree(back_to_path_h)
                    .tree(back_to_path)
                .end()
                .leaf(HenchPig::NTSleep)
            .end()
        .build();

        _bt["berserk"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(Enemy::NTsetRandNeighbourWaypoint)
                    .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .tree(back_to_path_h)
                .tree(back_to_path)
            .end()
        .build();



        _bt["loiter_towards_hero"] = dang::NTBuilder{}
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


        _bt["wait_for_hero"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1))
                    .leaf(Enemy::NTsetWPNearHero)
                    .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .leaf(HenchPig::NTNap)
            .end()
        .build();

    }

}