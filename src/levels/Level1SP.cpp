// (c) 2019-1 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level1SP.hpp"
#include "GSPlay.h"
#include "actors/npc/PigBomb.h"
#include "actors/npc/PigBoss.h"

#include <bt/NTBuilder.h>

#ifdef PNK_DEBUG_PRINT
#ifdef TARGET_32BLIT_HW
#include "32blit.hpp"
#endif
#endif

#ifdef PNK_DEBUG_COMMON
#include <malloc.h>
#endif

namespace pnk
{
    Level1SP::Level1SP(GSPlay& gsp)
    {
        _l_bg_name = "lvl_1_bg";
        _l_obj_name = "lvl_1_obj";;
        _l_mood_name = "lvl_1_mood";
        _l_hud_name = "lvl_1_hud";
        _l_fg_name = "lvl_1_fg";

        // generic loitering
        _bt["loiter"] = buildLoiter();
        _bt["berserk"] = buildBerserk();
        _bt["loiter_with_one_crate"] = buildLoiterWithSingleCrateH(gsp);
        _bt["wait_crate"] = buildWaitWithCrate(gsp);
        _bt["wait_bomb"] = buildWaitWithBomb(gsp);
        _bt["basic_cannon"] = buildBasicCannon();

        _bt["wait_with_bombs"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .inverter().leaf(PigBomb::NTWithBomb)
                    .selector()
                        .sequence()
                            .leaf(Enemy::NTsetDestinationBombDepot)
                            .leaf(Enemy::NTcheckPathCompleted)
                            .leaf(HenchPig::NTsetSleepLong)
                            .leaf(HenchPig::NTdoSleep)
                            .leaf(PigBomb::NTPickUpBomb)
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
                    .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1, std::placeholders::_2))
//                    .leaf(PigBomb::NTDistanceOK)
                    .leaf(PigBomb::NTThrowBomb)
                    .leaf(HenchPig::NTsetSleepMedium)
                    .leaf(HenchPig::NTdoSleep)
                .end()
            .end()
        .build();

        _bt["boss"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(PigBoss::NTHit)
                    .leaf(PigBoss::NTRecover)
                .end()
                .sequence()
                    .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1, std::placeholders::_2))
                    .leaf(PigBoss::NTRun)
                    .leaf(Enemy::NTcheckPathCompleted)
                    .leaf(PigBoss::NTLurk)
                .end()
            .end()
        .build();


    }

}