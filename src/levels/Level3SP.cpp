// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level3SP.hpp"
#include "GSPlay.h"
#include "actors/npc/PigCrate.h"
#include "actors/npc/PigBomb.h"

#include <bt/NTBuilder.h>

#ifdef PNK_DEBUG_PRINT
#ifdef TARGET_32BLIT_HW
#include "32blit.hpp"
#endif

#include <malloc.h>
#endif

namespace pnk
{
    Level3SP::Level3SP(GSPlay& gsp)
    {
        _l_bg_name = "lvl_3_bg";
        _l_obj_name = "lvl_3_obj";;
        _l_mood_name = "lvl_3_mood";
        _l_hud_name = "lvl_3_hud";
        _l_fg_name = "lvl_3_fg";

        // generic loitering
        _bt["loiter"] = buildLoiter();
        _bt["berserk"] = buildBerserk();

        _bt["wait_for_hero"] = dang::NTBuilder{}
            .selector()
                .sequence()
//                    .leaf(HenchPig::NTsetSleepMedium)
//                    .leaf(HenchPig::NTdoSleep)
                    .leaf(std::bind(&GSPlay::NTheroInSight, &gsp, std::placeholders::_1, std::placeholders::_2))
                    .leaf(Enemy::NTsetWPNearHero)
                    .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .sequence()
                    .leaf(HenchPig::NTsetSleepShort)
                    .leaf(HenchPig::NTdoSleep)
                .end()
            .end()
        .build();

        _bt["wait_with_bombs"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .inverter().leaf(PigBomb::NTWithBomb)
                    .selector()
                        .sequence()
                            .leaf(Enemy::NTsetDestinationBombDepot)
                            .leaf(Enemy::NTcheckPathCompleted)
                            .leaf(PigBomb::NTPickUpBomb)
                            .leaf(HenchPig::NTsetSleepShort)
                            .leaf(HenchPig::NTdoSleep)
                            .leaf(Enemy::NTsetDestinationPOI)
                            .leaf(Enemy::NTcheckPathCompleted)
                        .end()
                        .tree(buildBackToPathH())
                        .tree(buildBackToPath())
                    .end()
                .end()
                .selector()
                    .sequence()
                        .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1, std::placeholders::_2))
                        .leaf(PigBomb::NTThrowBomb)
                    .end()
                    .sequence()
                        .leaf(HenchPig::NTsetSleepShort)
                        .leaf(HenchPig::NTdoSleep)
                    .end()
                .end()
            .end()
        .build();

/*        _bt["wait_with_crates"] = dang::NTBuilder{}
                .selector()
                .sequence()
                .inverter().leaf(PigCrate::NTWithCrate)
                .selector()
                .sequence()
                .leaf(Enemy::NTsetDestinationCrateDepot)
                .leaf(Enemy::NTcheckPathCompleted)
                .leaf(PigBomb::NTPickUpBomb)
                .leaf(Enemy::NTsetDestinationPOI)
                .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .tree(back_to_path_h)
                .tree(back_to_path)
                .end()
                .end()
                .sequence()
                .leaf(std::bind(&GSPlay::NTheroInSight, &gsp, std::placeholders::_1, std::placeholders::_2))
//                    .leaf(PigBomb::NTDistanceOK)
                .leaf(PigBomb::NTThrowBomb)
                .leaf(HenchPig::NTsetSleepMedium)
                .leaf(HenchPig::NTdoSleep)
                .end()
                .end()
                .build();

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("Level3SP: after bt 6 (%d)\r\n", mallinfo().uordblks);
#endif
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

*/
    }

}