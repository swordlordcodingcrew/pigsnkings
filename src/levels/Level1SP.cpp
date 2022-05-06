// (c) 2019-1 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Level1SP.hpp"
#include "../pnk_globals.h"
#include "../GSPlay.h"
#include "../actors/npc/Enemy.h"
#include "../actors/npc/HenchPig.h"
#include "../actors/npc/PigCrate.h"
#include "../actors/npc/PigBomb.h"
#include "../actors/npc/PigBoss.h"

#include <bt/NTBuilder.h>

#ifdef PNK_DEBUG_PRINT
#ifdef TARGET_32BLIT_HW
#include "32blit.hpp"
#endif

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

        // viewport = 320 x 240 px
        act room0;
        room0._extent = {0, 16, 10, 8};
        /** the starting point related to the save-game-trigger / start of level */
        room0._passage_from[-1] = {0, 5};
        /** when coming from room 1, start at given position */
        room0._passage_from[1] = {8, 1};
        _acts.push_back(room0);

        act room1;
        room1._extent = {9, 16, 11, 8};
        /** the starting point related to the save-game-trigger */
        room1._passage_from[-1] = {0, 1};
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
        /** the starting point related to the save-game-trigger */
        room3._passage_from[-1] = {0, 6};
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
        /** the starting point related to the save-game-trigger */
        room5._passage_from[-1] = {8, 2};
        room5._passage_from[4] = {8, 2};
        room5._passage_from[6] = {0, 6};
        _acts.push_back(room5);

        act room6;
        room6._extent = {0, 8, 10, 8};
        room6._passage_from[5] = {4, 1};
        _acts.push_back(room6);

        act room7;
        room7._extent = {10, 8, 10, 8};
        room7._passage_from[4] = {1, 6};
        room7._passage_from[6] = {1, 6};
        _acts.push_back(room7);

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

        DEBUG_PRINT("Level1SP: after bt 6 (%d)\r\n", mallinfo().uordblks);

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