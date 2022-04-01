// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "src/pigsnkings.hpp"
#include "Enemy.h"
#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/levels/ScreenPlay.h"

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <path/SceneGraph.hpp>
#include <tween/TwVel.hpp>
#include <tween/TwAccY.hpp>
#include <tween/TwVelY.hpp>
#include <path/Waypoint.hpp>
#include <TmxExtruder.hpp>
#include <iostream>

namespace pnk
{
    extern PigsnKings _pnk;


    Enemy::Enemy() : dang::CollisionSprite()
    {
        _gravity = PigsnKings::_gravity;
    }

    Enemy::Enemy(const dang::tmx_spriteobject* so, const dang::spImagesheet& is) : dang::CollisionSprite(so, is)
    {
        _gravity = PigsnKings::_gravity;
    }

    void Enemy::init()
    {
        _hotrect = {10, 16, 12, 16};

        setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3, 4, 5}, 600, &dang::Ease::Linear, -1)));

        setVel({0,0});
    }

    Enemy::~Enemy()
    {
        _path.clear();

#ifdef PNK_DEBUG_PRINT
        std::cout << "enemy destructor" << std::endl;
#endif
    }


    void Enemy::initSceneGraph(const spScreenPlay &sp)
    {
        for (auto room : sp->_acts)
        {
            if (room._extent_pixels.contains(local2Global(_pos)) && !room._scene_graphs.empty())
//            if (room._extent_pixels.contains(_pos_g) && !room._scene_graphs.empty())
            {
                size_t ind = sp->findNearestGraph(getHotrectG().center());
                _scene_graph = room._scene_graphs[ind];

                // first clear any remains of the last path
                resetPathVars();

                const dang::Waypoint* wp = _scene_graph->findNearestWaypoint(getHotrectG().center());
                if (_scene_graph->waypointReached(getHotrectG(), wp))
                {
                    _current_wp = wp;
#ifdef PNK_DEBUG_WAYPOINTS
                    DEBUG_PRINT("(spr id %u): initSceneGraph: graph found. Set waypoint with id %u \n", _id, wp->_id);
#endif
                }
                else
                {
                    _path.push_back(wp);
#ifdef PNK_DEBUG_WAYPOINTS
                    DEBUG_PRINT("(spr id %u): initSceneGraph: graph found. Nearest waypoint = %u \n", _id, wp->_id);
#endif
                    startOutToWaypoint();
                }
                break;
            }
        }

    }


    dang::BTNode::Status Enemy::checkPathCompleted()
    {
        dang::BTNode::Status ret{dang::BTNode::Status::FAILURE};

        if (!_path.empty() )
        {
            ret = checkWaypointReached();
            if (ret == dang::BTNode::Status::SUCCESS)
            {
                if (_path_index + 1 == _path.size())
                {
                    _current_wp = _path[_path_index];

                    // ultimate goal reached. Reset stuff
                    resetPathVars();
                    ret = dang::BTNode::Status::SUCCESS;
                }
                else
                {
                    // next waypoint
                    _current_wp = _path[_path_index];
                    _path_index++;
                    startOutToWaypoint();
                    ret = dang::BTNode::Status::RUNNING;
                }
            }
        }
        return ret;
    }

    dang::BTNode::Status Enemy::setWPHNearHero()
    {
        if (_current_wp == nullptr)
        {
            // no current waypoint set -> finding a path is not possible
            return dang::BTNode::Status::FAILURE;
        }

        // first clear any remains of the last path
        resetPathVars();

        float dist = _nTreeState->_payload["aaLoSH"];
        _nTreeState->_payload.erase("aaLoSH");
        _scene_graph->getNearestNeighbourHPath(_current_wp, dist, _path);
        if (_path.empty())
        {
            // no other waypoint (?). Would be a design error, returning failure
            return dang::BTNode::Status::FAILURE;
        }

#ifdef PNK_DEBUG_WAYPOINTS
        DEBUG_PRINT("(spr id %u): setWPHNearHero: nearest waypoint = %u \n", _id, _path[0]->_id);
#endif

        startOutToWaypoint();
        return dang::BTNode::Status::SUCCESS;
    }

    dang::BTNode::Status Enemy::setRandPath()
    {
        if (_current_wp == nullptr)
        {
            // no current waypoint set -> finding a path is not possible
            return dang::BTNode::Status::FAILURE;
        }

        // first clear any remains of the last path
        resetPathVars();

        _scene_graph->getRandomPath(_current_wp, _path);

        // the path includes also the first (i.e. current) wp
        if (_path.size() < 2)
        {
            // no other waypoint (?). Would be a design error, returning failure
            return dang::BTNode::Status::FAILURE;
        }
        _path_index++;

#ifdef PNK_DEBUG_WAYPOINTS
        DEBUG_PRINT("(spr id %u): setRandPath: set random path towards wp  %u \n", _id, _path[_path.size()-1]->_id);
#endif

        startOutToWaypoint();
        return dang::BTNode::Status::SUCCESS;
    }


    dang::BTNode::Status Enemy::setRandNeighbourWaypoint()
    {
        if (_current_wp == nullptr)
        {
            // no current waypoint set -> finding a neighbour is not possible
            return dang::BTNode::Status::FAILURE;
        }

        // first clear any remains of the last path
        resetPathVars();

        _scene_graph->getRandomNeighbourPath(_current_wp, _path);
        if (_path.empty())
        {
            // no other waypoint (?). Would be a design error, returning failure
            return dang::BTNode::Status::FAILURE;
        }

        startOutToWaypoint();
        return dang::BTNode::Status::SUCCESS;
    }

    dang::BTNode::Status Enemy::setDestinationWaypointByType(uint32_t wp_type)
    {
        if (_current_wp == nullptr)
        {
            // no current waypoint set -> finding a neighbour is not possible
            return dang::BTNode::Status::FAILURE;
        }

        // first clear any remains of the last path
        resetPathVars();

        dang::Waypoint* start = const_cast<dang::Waypoint*>(_current_wp);
        const dang::Waypoint* dest = _scene_graph->getWaypointWithType(wp_type);

        if (dest != start)
        {
            if (_scene_graph->getPath(start, dest, _path))
            {
                _path_index = 0;
                if (!_path.empty())
                {
                    startOutToWaypoint();
                    return dang::BTNode::Status::SUCCESS;
                }
            }
        }
        return dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status Enemy::checkWaypointReached()
    {
        if (_scene_graph->waypointReached(getHotrectG(), _path[_path_index]))
        {
            _vel.x = 0;
            if (_on_ground)     // waypoint in hotrect and on ground -> waypoint reached
            {
                _max_time_to_wp = 0;
                _time_elapsed_to_wp = 0;
#ifdef PNK_DEBUG_WAYPOINTS
                DEBUG_PRINT("(spr id %u): waypoint %u reached\n", _id, _path[_path_index]->_id);
#endif
                return dang::BTNode::Status::SUCCESS;
            }
            else
            {
                return dang::BTNode::Status::RUNNING;
            }
        }
        else
        {
            if (blit::now() - _time_elapsed_to_wp > _max_time_to_wp)
            {
#ifdef PNK_DEBUG_WAYPOINTS
                DEBUG_PRINT("(spr id %u): checkWaypointReached: time is up\n", _id);
#endif
                // waypoint was not reached in time
                return dang::BTNode::Status::FAILURE;
            }

            // if the sprite went too far, turn around and go slower
            const dang::Waypoint* w = _path.at(_path_index);
            if ((_vel.x < 0 && getHotrectG().center().x < w->_pos.x) || (_vel.x > 0 && getHotrectG().center().x > w->_pos.x))
            {
                _vel.x = -_vel.x;
                _transform = _vel.x > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;
                _vel.x /= 2;
            }

            return dang::BTNode::Status::RUNNING;

        }

    }

    void Enemy::startOutToWaypoint()
    {
        const dang::Waypoint* wp = _path.at(_path_index);

#ifdef PNK_DEBUG_WAYPOINTS
        DEBUG_PRINT("(spr id %u): start out from wp %u to wp %u\n", _id, (_current_wp == nullptr ? 0 : _current_wp->_id), wp->_id);
#endif

        removeTween(_tw_short_jump, true);
        removeTween(_tw_long_horiz_jump, true);

        uint32_t conn_type = _scene_graph->getConnectionType(_current_wp, wp);
        switch (conn_type)
        {
            case dang::e_tmx_waypoint_connection::wpc_invalid:
            case dang::e_tmx_waypoint_connection::wpc_walk:
            {
                _vel.x = wp->_pos.x - _co_pos.x < 0 ? -_walkSpeed : _walkSpeed;
//                _vel.x = wp->_pos.x - _pos_g.x < 0 ? -_walkSpeed : _walkSpeed;
                _max_time_to_wp = (std::fabs(wp->_pos.x - getHotrectG().center().x) + 32) * 100 / _walkSpeed;
                _time_elapsed_to_wp = blit::now();
                break;
            }
            case dang::e_tmx_waypoint_connection::wpc_jump:
            {
                dang::Vector2F v{0,0};
                dang::Vector2F v_end{0,0};
                if (getHotrectG().center().y - wp->_pos.y > 10)     // safety buffer of 10 units for the check if higher / lower
                {
                    // the waypoint is higher than the hero
                    v.y = -15 - (0.4f * _walkSpeed);
                }
                else
                {
                    // equal or lower
                    v.y = -5 + (0.4f * _walkSpeed);
                }

                if ((wp->_pos.x - getHotrectG().center().x) * (wp->_pos.x - getHotrectG().center().x) > 1600)  // long horizontal distance
                {
                    if (wp->_pos.x - _co_pos.x < 0)
//                    if (wp->_pos.x - _pos_g.x < 0)
                    {
                        v.x = -16;
                        v_end.x = -_walkSpeed;
                    }
                    else
                    {
                        v.x = 16;
                        v_end.x = _walkSpeed;
                    }
                    _vel.x = v.x;
                    _max_time_to_wp = 3000;
                    _time_elapsed_to_wp = blit::now();
                    _tw_long_horiz_jump = std::make_shared<dang::TwVel>(v, v_end, 600, &dang::Ease::Linear, 1, false );
                    addTween(_tw_long_horiz_jump);
                }
                else
                {
                    // short jump
                    _max_time_to_wp = 2000;
                    _time_elapsed_to_wp = blit::now();
                    _vel.x = wp->_pos.x - _co_pos.x < 0 ? -3 : 3;
//                    _vel.x = wp->_pos.x - _pos_g.x < 0 ? -3 : 3;
                    _tw_short_jump = std::make_shared<dang::TwVelY>(v.y, 0.0f, 600, &dang::Ease::Linear, 1, false );
                    addTween(_tw_short_jump);
                }
                break;
            }
            case dang::e_tmx_waypoint_connection::wpc_warp:
                // TODO
                break;
            default:
                break;
        }
    }

    /**
     * this function is used if the npc is lost (i.e. bubbled and freed). This happens if the _max_time_to_wp is used up
     * @param only_horizontally
     * @return BT Status
     */
    dang::BTNode::Status Enemy::findNearestWaypoint(bool only_horizontally)
    {
        if (_scene_graph == nullptr)
        {
            return dang::BTNode::Status::FAILURE;
        }

        resetPathVars();

        const dang::Waypoint* w;
        if (only_horizontally)
        {
            w = _scene_graph->findNearestWaypointH(getHotrectG());
        }
        else
        {
            w = _scene_graph->findNearestWaypoint(getHotrectG().center());
        }

        if (w != nullptr)
        {
            _path.push_back(w);
            _path_index = 0;
            startOutToWaypoint();
            return dang::BTNode::Status::SUCCESS;
        }

#ifdef PNK_DEBUG_WAYPOINTS
        DEBUG_PRINT("(spr id %u): findNearestWaypoint: no waypoint found\n", _id);
#endif

        return dang::BTNode::Status::FAILURE;
    }

    void Enemy::resetPathVars()
    {
        _path.clear();
        _path_index = 0;
        _vel.x = 0;
        _max_time_to_wp = 0;
        _time_elapsed_to_wp = 0;
    }


    /** static BT hooks */

    dang::BTNode::Status Enemy::NTcheckPathCompleted(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->checkPathCompleted() : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTsetRandNeighbourWaypoint(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->setRandNeighbourWaypoint() : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTcheckWaypointReached(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->checkWaypointReached() : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTsetDestinationBombDepot(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->setDestinationWaypointByType(WPT_BOMBDEPOT) : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTsetDestinationCrateDepot(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->setDestinationWaypointByType(WPT_CRATEDEPOT) : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTsetDestinationPOI(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->setDestinationWaypointByType(WPT_POI) : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTfindNearestWaypoint(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->findNearestWaypoint(false) : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTfindNearestWaypointH(std::shared_ptr<Sprite> s)
    {
//        std:: cout << "find nearest waypoint H" << std::endl;
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->findNearestWaypoint(true) : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTsetRandomPath(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->setRandPath() : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTsetWPNearHero(dang::spSprite s)
    {
        std::shared_ptr<Enemy> spr = std::static_pointer_cast<Enemy>(s);
        return (spr ? spr->setWPHNearHero() : dang::BTNode::Status::FAILURE);
    }

}