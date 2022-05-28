// (c) 2019-1 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level2SP.hpp"

namespace pnk
{
    Level2SP::Level2SP(GSPlay& gsp)
    {
        _l_bg_name = "lvl_2_bg";
        _l_obj_name = "lvl_2_obj";;
        _l_mood_name = "lvl_2_mood";
        _l_hud_name = "lvl_2_hud";
        _l_fg_name = "lvl_2_fg";

        /** behaviour trees */
        _bt["loiter"] = buildLoiter();


/*        dang::spNTree tr = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(Enemy::NTsetRandNeighbourWaypoint)
                    .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .sequence()
                    .leaf(Enemy::NTfindNearestWaypointH)
                    .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .leaf(HenchPig::NTsetSleepMedium)
                .leaf(HenchPig::NTdoSleep)
            .end()
        .build();

        _bt["loiter"] = tr;
*//*
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
*/

    }
}