// (c) 2019-1 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level2SP.hpp"
#include "GSPlay.h"
#include "actors/npc/PigBoss.h"

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

        /** behaviour trees */
        _bt["loiter"] = buildLoiter();
        _bt["berserk"] = buildBerserk();
        _bt["basic_cannon"] = buildBasicCannon();


        _bt["boss"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(PigBoss::NTHit)
                    .leaf(PigBoss::NTRecover)
                .end()
                .sequence()
                    .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1, std::placeholders::_2))
                    .leaf(PigBoss::NTRunToPOI)
                    .leaf(Enemy::NTcheckPathCompleted)
                    .leaf(PigBoss::NTLurk)
                .end()
            .end()
        .build();

    }
}