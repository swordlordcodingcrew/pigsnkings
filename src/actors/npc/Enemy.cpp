// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>
#include <path/SceneGraph.hpp>
#include <tween/TwVel.hpp>
#include <tween/TwAccY.hpp>
#include <tween/TwVelY.hpp>
#include <path/Waypoint.hpp>

#include "src/pigsnkings.hpp"
#include "Enemy.h"
#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"

namespace pnk
{
    using spTwVel = std::shared_ptr<dang::TwVel>;
    using spTwVelY = std::shared_ptr<dang::TwVelY>;

    extern PigsnKings _pnk;

    Enemy::Enemy() : dang::CollisionSprite()
    {
        _gravity = PigsnKings::_gravity;
    }

    Enemy::Enemy(const dang::tmx_spriteobject* so, spImagesheet is) : dang::CollisionSprite(so, is)
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

        std::cout << "enemy destructor" << std::endl;
    }

    dang::BTNode::Status Enemy::checkPathCompleted()
    {
        dang::BTNode::Status ret{dang::BTNode::Status::FAILURE};

        if (!_path.empty())
        {
            ret = checkWaypointReached();
            if (ret == dang::BTNode::Status::SUCCESS)
            {
                if (_path_index + 1 == _path.size())
                {
                    _current_wp = _path[_path_index];
//                        std::cout << "path: enemy " << int8_t(_type_num) << " reached goal " << _current_wp.lock()->_id << std::endl;

                    // ultimate goal reached. Reset stuff
                    _path.clear();
                    _path_index = 0;
                    _vel.x = 0;
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

    dang::BTNode::Status Enemy::setRandNeighbourWaypoint()
    {
        _scene_graph->getRandomNextWaypoint(_current_wp, _path);
        _path_index = 0;
        if (_path.empty())
        {
            // no other waypoint (?). Would be a design error, returning failure
            return dang::BTNode::Status::FAILURE;
        }

        startOutToWaypoint();
        return dang::BTNode::Status::SUCCESS;
    }

    dang::BTNode::Status Enemy::setDestinationWaypointByDepot(uint32_t depot_type)
    {
        dang::spWaypoint start = _current_wp.lock();
        dang::spWaypoint dest = _scene_graph->getWaypointWithType(depot_type);

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
        if (_scene_graph->waypointReached(getHotrectAbs(), _path[_path_index]))
        {
            _vel.x = 0;
            if (_on_ground)
            {
                _max_time_to_wp = 0;
                _time_elapsed_to_wp = 0;
//                std::cout << "waypoint reached" << std::endl;
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
//                std::cout << "checkWaypointReached: time is up!" << std::endl;
                return dang::BTNode::Status::FAILURE;
            }

            dang::spWaypoint w = _path[_path_index].lock();
            if (w)
            {
                // if the sprite went too far, turn around and go slower
                if ((_vel.x < 0 && getHotrectAbs().center().x < w->_pos.x) || (_vel.x > 0 && getHotrectAbs().center().x > w->_pos.x))
                {
                    _vel.x = -_vel.x;
                    _transform = _vel.x > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;
                    _vel.x /= 2;
                }

                return dang::BTNode::Status::RUNNING;
            }

            return dang::BTNode::Status::FAILURE;

        }

    }

    void Enemy::startOutToWaypoint()
    {
        dang::wpWaypoint wp = _path[_path_index];
        dang::spWaypoint spwp = wp.lock();

        if (spwp)
        {
            uint32_t conn_type = _scene_graph->getConnectionType(_current_wp, wp);
            switch (conn_type)
            {
                case dang::e_tmx_waypoint_connection::wpc_invalid:
                case dang::e_tmx_waypoint_connection::wpc_walk:
                {
                    _walkSpeed = 2;
                    removeTweens(true);
                    _vel.x = spwp->_pos.x - _pos.x < 0 ? -_walkSpeed : _walkSpeed;
                    _max_time_to_wp = (std::fabs(spwp->_pos.x - getHotrectAbs().center().x) + 32) * 100 / _walkSpeed;
                    _time_elapsed_to_wp = blit::now();
                    _current_wp = wp;
                    break;
                }
                case dang::e_tmx_waypoint_connection::wpc_jump:
                {
                    removeTweens(true);
                    dang::Vector2F v{0,0}, v_end{0,0};
                    if (spwp->_pos.y < getHotrectAbs().center().y)
                    {
                        // the waypoint is higher than the hero
                        v.y = -16;
                    }
                    else
                    {
                        // equal or lower
                        v.y = -6;
                    }

                    if ((spwp->_pos.x - getHotrectAbs().center().x) * (spwp->_pos.x - getHotrectAbs().center().x) > 1600)
                    {
                        // long horizontal distance
                        if (spwp->_pos.x - _pos.x < 0)
                        {
                            v.x = -16;
                            v_end.x = -2;
                        }
                        else
                        {
                            v.x = 16;
                            v_end.x = 2;
                        }
                        _max_time_to_wp = 3000;
                        _time_elapsed_to_wp = blit::now();
                        spTwVel tw = std::make_shared<dang::TwVel>(v,v_end, 600, &dang::Ease::OutQuad, 1, false );
                        addTween(tw);
                    }
                    else
                    {
                        _max_time_to_wp = 2000;
                        _time_elapsed_to_wp = blit::now();
                        _walkSpeed = 2;
                        _vel.x = spwp->_pos.x - _pos.x < 0 ? -_walkSpeed : _walkSpeed;
                        spTwVelY tw = std::make_shared<dang::TwVelY>(v.y, 0.0f, 600, &dang::Ease::OutQuad, 1, false );
                        addTween(tw);
                    }
                    _current_wp = wp;
                    break;
                }
                case dang::e_tmx_waypoint_connection::wpc_warp:
                    // TODO
                    break;
                default:
                    break;
            }
        }

    }

    dang::BTNode::Status Enemy::findNearestWaypoint(bool only_horizontally)
    {
//        std::cout << "find nearest waypoint" << std::endl;

        // first clear any remainy of the last path
        _path.clear();
        _path_index = 0;
        _vel.x = 0;


        dang::spWaypoint w;
        if (only_horizontally)
        {
            w = _scene_graph->findNearestWaypointH(getHotrectAbs());
        }
        else
        {
            w = _scene_graph->findNearestWaypoint(getHotrectAbs().center());
        }

        if (w)
        {
            _path.push_back(w);
            _path_index = 0;
//            _current_wp = _path[_path_index];
            startOutToWaypoint();
            return dang::BTNode::Status::SUCCESS;
        }

        return dang::BTNode::Status::FAILURE;
    }

    /** static BT hooks */

    dang::BTNode::Status Enemy::NTcheckPathCompleted(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::dynamic_pointer_cast<Enemy>(s);
        return (spr ? spr->checkPathCompleted() : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTsetRandNeighbourWaypoint(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::dynamic_pointer_cast<Enemy>(s);
        return (spr ? spr->setRandNeighbourWaypoint() : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTcheckWaypointReached(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::dynamic_pointer_cast<Enemy>(s);
        return (spr ? spr->checkWaypointReached() : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTsetDestinationBombDepot(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::dynamic_pointer_cast<Enemy>(s);
        return (spr ? spr->setDestinationWaypointByDepot(dang::wp_bombdepot) : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTsetDestinationCrateDepot(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::dynamic_pointer_cast<Enemy>(s);
        return (spr ? spr->setDestinationWaypointByDepot(dang::wp_cratedepot) : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTfindNearestWaypoint(std::shared_ptr<Sprite> s)
    {
        std::shared_ptr<Enemy> spr = std::dynamic_pointer_cast<Enemy>(s);
        return (spr ? spr->findNearestWaypoint(false) : dang::BTNode::Status::FAILURE);
    }

    dang::BTNode::Status Enemy::NTfindNearestWaypointH(std::shared_ptr<Sprite> s)
    {
//        std:: cout << "find nearest waypoint H" << std::endl;
        std::shared_ptr<Enemy> spr = std::dynamic_pointer_cast<Enemy>(s);
        return (spr ? spr->findNearestWaypoint(true) : dang::BTNode::Status::FAILURE);
    }

}