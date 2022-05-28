// (c) 2019-1 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level1SP.hpp"
#include "../GSPlay.h"
#include "../actors/npc/PigBomb.h"
#include "../actors/npc/PigBoss.h"

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
                    .leaf(std::bind(&GSPlay::NTheroInSight, &gsp, std::placeholders::_1, std::placeholders::_2))
//                    .leaf(PigBomb::NTDistanceOK)
                    .leaf(PigBomb::NTThrowBomb)
                    .leaf(HenchPig::NTsetSleepMedium)
                    .leaf(HenchPig::NTdoSleep)
                .end()
            .end()
        .build();

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("Level1SP: after bt 6 (%d)\r\n", mallinfo().uordblks);
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





/*        _bt["loiter_with_crates"] = dang::NTBuilder{}
        .selector()
            .sequence()
                .inverter().leaf(PigCrate::NTWithCrate)
                .leaf(Enemy::NTsetDestinationCrateDepot)
                .leaf(Enemy::NTcheckPathCompleted)
                .leaf(HenchPig::NTNap)
                .leaf(PigCrate::NTPickUpCrate)
            .end()
            .sequence()
                .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1))
                .leaf(PigCrate::NTThrowCrate)
            .end()
            .sequence()
                .leaf(Enemy::NTsetRandNeighbourWaypoint)
                .leaf(Enemy::NTcheckPathCompleted)
            .end()
            .tree(back_to_path_h)
            .tree(back_to_path)
        .end()
    .build();
*/

/*
        DEBUG_PRINT("Level1SP: before bt (%d)\r\n", mallinfo().uordblks);

        // behaviour tree 1 for finding back to the path system
        dang::spNTree back_to_path_h = dang::NTBuilder{}
            .sequence()
                .leaf(Enemy::NTfindNearestWaypointH)
                .leaf(Enemy::NTcheckPathCompleted)
            .end()
        .build();

        DEBUG_PRINT("Level1SP: after bt 1 (%d)\r\n", mallinfo().uordblks);

        // behaviour tree 2 for finding back to the path system
        dang::spNTree back_to_path = dang::NTBuilder{}
            .sequence()
                .leaf(Enemy::NTfindNearestWaypoint)
                .leaf(Enemy::NTcheckPathCompleted)
            .end()
        .build();

        DEBUG_PRINT("Level1SP: after bt 2 (%d)\r\n", mallinfo().uordblks);

        _bt["loiter"] = dang::NTBuilder{}
            .sequence()
                .selector()
                    .sequence()
                        .leaf(Enemy::NTsetRandomPath)
                        .leaf(Enemy::NTcheckPathCompleted)
                    .end()
                    .tree(buildBackToPathH())
                    .tree(buildBackToPath())
                .end()
                .leaf(HenchPig::NTsetSleepMedium)
                .leaf(HenchPig::NTdoSleep)
            .end()
        .build();

        DEBUG_PRINT("Level1SP: after bt 3 (%d)\r\n", mallinfo().uordblks);

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


        DEBUG_PRINT("Level1SP: after bt 4 (%d)\r\n", mallinfo().uordblks);

        _bt["loiter_with_one_crate"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1, std::placeholders::_2))
                    .leaf(PigCrate::NTThrowCrate)
                .end()
                .sequence()
                    .leaf(Enemy::NTsetRandNeighbourWaypoint)
                    .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .tree(back_to_path_h)
                .tree(back_to_path)
            .end()
        .build();


        DEBUG_PRINT("Level1SP: after bt 5 (%d)\r\n", mallinfo().uordblks);

        _bt["wait_crate"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(PigCrate::NTWithCrate)
                    .selector()
                        .sequence()
                            .leaf(std::bind(&GSPlay::NTheroInSight, &gsp, std::placeholders::_1, std::placeholders::_2))
                            .leaf(PigCrate::NTDistanceOK)
                            .leaf(PigCrate::NTThrowCrate)
                            .leaf(HenchPig::NTsetSleepMedium)
                            .leaf(HenchPig::NTdoSleep)
                        .end()
                        .sequence()
                            .leaf(HenchPig::NTsetSleepShort)
                            .leaf(HenchPig::NTdoSleep)
                        .end()
                    .end()
                .end()
                .tree(_bt["loiter"])
            .end()
        .build();


        _bt["wait_bomb"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(PigBomb::NTWithBomb)
                    .selector()
                        .sequence()
                            .leaf(std::bind(&GSPlay::NTheroInSight, &gsp, std::placeholders::_1, std::placeholders::_2))
                            .leaf(PigBomb::NTDistanceOK)
                            .leaf(PigBomb::NTThrowBomb)
                            .leaf(HenchPig::NTsetSleepMedium)
                            .leaf(HenchPig::NTdoSleep)
                        .end()
                        .sequence()
                            .leaf(HenchPig::NTsetSleepShort)
                            .leaf(HenchPig::NTdoSleep)
                        .end()
                    .end()
                .end()
                .tree(_bt["loiter"])
            .end()
        .build();
*/

/*        _bt["wait_for_hero"] = dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1))
                    .leaf(Enemy::NTsetWPNearHero)
                    .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .sequence()
                    .leaf(HenchPig::NTsetSleepShort)
                    .leaf(HenchPig::NTdoSleep)
                .end()
            .end()
        .build();
*/
    }

}