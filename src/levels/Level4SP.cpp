// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level4SP.hpp"
#include "GSPlay.h"
#include "actors/npc/PigCrate.h"

#include <bt/NTBuilder.h>

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
        _bt["basic_cannon"] = buildBasicCannon();
        _bt["wait_bomb"] = buildWaitWithBomb(gsp);
        _bt["loiter_one_bomb_h"] = buildLoiterWithSingleBombH(gsp);

        _bt["wait_with_crates"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .inverter().leaf(PigCrate::NTWithCrate)
                    .selector()
                        .sequence()
                            .leaf(Enemy::NTsetDestinationCrateDepot)
                            .leaf(Enemy::NTcheckPathCompleted)
                            .leaf(PigCrate::NTPickUpCrate)
                            .leaf(Enemy::NTsetDestinationPOI)
                            .leaf(Enemy::NTcheckPathCompleted)
                            .leaf(HenchPig::NTsetSleepShort)
                            .leaf(HenchPig::NTdoSleep)
                        .end()
                        .tree(buildBackToPathH())
                        .tree(buildBackToPath())
                    .end()
                .end()
                .sequence()
                    .leaf(std::bind(&GSPlay::NTheroInSight, &gsp, std::placeholders::_1, std::placeholders::_2))
//                    .leaf(PigBomb::NTDistanceOK)
                    .leaf(PigCrate::NTThrowCrate)
                    .leaf(HenchPig::NTsetSleepShort)
                    .leaf(HenchPig::NTdoSleep)
                .end()
            .end()
        .build();

        _bt["loiter_with_one_crate_dist"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1, std::placeholders::_2))
                    .leaf(PigCrate::NTDistanceOK)
                    .leaf(PigCrate::NTThrowCrate)
                    .end()
                .sequence()
                    .leaf(Enemy::NTsetRandNeighbourWaypoint)
                    .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .tree(buildBackToPathH())
                .tree(buildBackToPath())
            .end()
        .build();

    }
}