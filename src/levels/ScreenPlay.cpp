// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "ScreenPlay.h"

#include <path/SceneGraph.hpp>

#include <cfloat>

namespace pnk
{
    size_t ScreenPlay::findNearestGraph(const dang::Vector2F& pos)
    {
        float dist = FLT_MAX;
        size_t index{0};

        for (auto room : _acts)
        {
            if (room._extent_pixels.contains(pos) && !room._scene_graphs.empty())
            {
                for (size_t i = 0; i < room._scene_graphs.size(); ++i)
                {
                    float newdist = room._scene_graphs[i]->findNearestWaypointDist(pos);
                    if (newdist < dist)
                    {
                        dist = newdist;
                        index = i;
                    }
                }
                // room found, break the for loop
                break;
            }
        }

        return index;
    }

}
