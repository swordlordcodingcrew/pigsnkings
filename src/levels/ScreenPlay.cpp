// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "ScreenPlay.h"

//#include "../pnk_globals.h"
//#include "../GSPlay.h"
#include "../actors/npc/Enemy.h"
#include "../actors/npc/HenchPig.h"
#include "../actors/npc/PigCrate.h"
#include "../actors/npc/PigBomb.h"
#include "../actors/npc/PigBoss.h"

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

    dang::spNTree ScreenPlay::getBackToPathH()
    {
        // behaviour tree 1 for finding back to the path system
        return dang::NTBuilder{}
            .sequence()
                .leaf(Enemy::NTfindNearestWaypointH)
                .leaf(Enemy::NTcheckPathCompleted)
            .end()
        .build();
    }

    dang::spNTree ScreenPlay::getBackToPath()
    {
        // behaviour tree 2 for finding back to the path system
        return dang::NTBuilder{}
            .sequence()
                .leaf(Enemy::NTfindNearestWaypoint)
                .leaf(Enemy::NTcheckPathCompleted)
            .end()
        .build();
    }

}
