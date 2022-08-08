// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "ScreenPlay.h"

//#include "../pnk_globals.h"
#include "GSPlay.h"
#include "actors/npc/Enemy.h"
#include "actors/npc/HenchPig.h"
#include "actors/npc/PigCrate.h"
#include "actors/npc/PigBomb.h"
#include "actors/npc/PigBoss.h"

#include <path/SceneGraph.hpp>
#include <bt/NTBuilder.h>

#include <cfloat>

namespace pnk
{
    dang::spSceneGraph ScreenPlay::findNearestGraph(const uint16_t zone_nr, const dang::Vector2F& pos)
    {
        float dist = FLT_MAX;
        size_t index{0};

        const std::vector<dang::spSceneGraph>& graphs = _scene_graphs[zone_nr];
        if (!graphs.empty())
        {
            for (size_t i = 0; i < graphs.size(); ++i)
            {
                float newdist = graphs[i]->findNearestWaypointDist(pos);
                if (newdist < dist)
                {
                    dist = newdist;
                    index = i;
                }
            }
            return graphs[index];
        }

        return nullptr;
    }

    dang::spNTree ScreenPlay::buildBackToPathH()
    {
        // behaviour tree 1 for finding back to the path system
        return dang::NTBuilder{}
            .sequence()
                .leaf(Enemy::NTfindNearestWaypointH)
                .leaf(Enemy::NTcheckPathCompleted)
            .end()
        .build();
    }

    dang::spNTree ScreenPlay::buildBackToPath()
    {
        // behaviour tree 2 for finding back to the path system
        return dang::NTBuilder{}
            .sequence()
                .leaf(Enemy::NTfindNearestWaypoint)
                .leaf(Enemy::NTcheckPathCompleted)
            .end()
        .build();
    }

    /**
     * For Henchpigs and children only
     * builds a generic loiter behaviour tree
     * @return built spNTree
     */
    dang::spNTree ScreenPlay::buildLoiter()
    {
        // generic loitering
        return dang::NTBuilder{}
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
    }

    /**
     * For Henchpigs and children only
     * builds a generic berserk behaviour tree
     * @return built spNTree
     */
    dang::spNTree ScreenPlay::buildBerserk()
    {
        return dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(Enemy::NTsetRandNeighbourWaypoint)
                    .leaf(Enemy::NTcheckPathCompleted)
                .end()
                .tree(buildBackToPathH())
                .tree(buildBackToPath())
            .end()
        .build();
    }

    /**
     * For PigCrate and children only
     * builds a behaviour tree where a crated pig loiters and, upon seeing (horizontally) the hero, throws the crate.
     * Before and after the piggy is loitering
     * @return built spNTree
     */
    dang::spNTree ScreenPlay::buildLoiterWithSingleCrateH(GSPlay& gsp)
    {
        return dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1, std::placeholders::_2))
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

    /**
     * For PigCrate and children only
     * builds a behaviour tree where a crated pig waits until the hero is close enough, then the crate is thrown
     * afterwards the piggy is loitering
     * @return built spNTree
     */
    dang::spNTree ScreenPlay::buildWaitWithCrate(GSPlay &gsp)
    {
        return dang::NTBuilder{}
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
                .tree(buildLoiter())
            .end()
        .build();

    }

    /**
     * For PigBomb and children only
     * builds a behaviour tree where a bombed pig waits until the hero is close enough, then the bomb is thrown
     * afterwards the piggy is loitering
     * @return built spNTree
     */
    dang::spNTree ScreenPlay::buildWaitWithBomb(GSPlay &gsp)
    {
        return dang::NTBuilder{}
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
    }

    /**
     * For PigBomb and children only
     * builds a behaviour tree where a bombed pig waits until the hero is horizontally close enough, then the bomb is thrown
     * afterwards the piggy is loitering
     * @return built spNTree
     */
    dang::spNTree ScreenPlay::buildWaitWithBombH(GSPlay &gsp)
    {
        return dang::NTBuilder{}
            .selector()
                .sequence()
                    .leaf(PigBomb::NTWithBomb)
                    .selector()
                        .sequence()
                            .leaf(std::bind(&GSPlay::NTheroInSightH, &gsp, std::placeholders::_1, std::placeholders::_2))
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
    }

}
