// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>
#include <path/SceneGraph.hpp>
#include <tween/TwVel.hpp>
#include <tween/TwAccY.hpp>

//#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"

#include "Enemy.h"
#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"

namespace pnk
{
    using spTwVel = std::shared_ptr<dang::TwVel>;
    using spTwAccY = std::shared_ptr<dang::TwAccY>;

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
        std::cout << "enemy destructor" << std::endl;
    }

    void Enemy::update(uint32_t dt)
    {
        _on_ground = false;
    }

    dang::CollisionSpriteLayer::eCollisionResponse Enemy::getCollisionResponse(spSprite other)
    {
        return dang::CollisionSpriteLayer::CR_SLIDE;
    }

    void Enemy::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
    }

    dang::BTNodeStatus Enemy::handlePath()
    {
        dang::BTNodeStatus ret{dang::BTNodeStatus::FAILURE};

        if (!_path.empty())
        {
            switch (checkPathProgress())
            {
                case dang::BTNodeStatus::SUCCESS:
                {
                    if (_path_index + 1 == _path.size())
                    {
                        _current_wp = _path[_path_index];
//                        std::cout << "path: enemy "<< _type_num << " reached goal " << _current_wp.lock()->_id << std::endl;

                        // ultimate goal reached. Reset stuff
                        _path.clear();
                        _path_index = 0;
                        _vel.x = 0;
                        ret = dang::BTNodeStatus::SUCCESS;
                    }
                    else
                    {
                        // next waypoint
                        _current_wp = _path[_path_index];
                        _path_index++;
                        startOutToWaypoint();
                        ret = dang::BTNodeStatus::RUNNING;
                    }
                    break;
                }
                case dang::BTNodeStatus::RUNNING:
                {
                    ret = dang::BTNodeStatus::RUNNING;
                    break;
                }
                case dang::BTNodeStatus::FAILURE:
                default:
                {
                    ret = dang::BTNodeStatus::FAILURE;
                    break;
                }
            }

        }
        else    // TODO: remove later
        {
            randomNextWaypoint();
            if (!_path.empty())
            {
                startOutToWaypoint();
                ret = dang::BTNodeStatus::RUNNING;
            }
        }

        return ret;
    }

    dang::BTNodeStatus Enemy::randomNextWaypoint()
    {
        _scene_graph->getRandomNextWaypoint(_current_wp, _path);
        _path_index = 0;
        if (_path.empty())
        {
            // no other waypoint (?). Would be a design error, returning failure
            return dang::BTNodeStatus::FAILURE;
        }
        return dang::BTNodeStatus::SUCCESS;
    }

    dang::BTNodeStatus Enemy::checkPathProgress()
    {
        if (_scene_graph->waypointReached(getHotrectAbs(), _path[_path_index]))
        {
            _vel.x = 0;
            if (_on_ground)
            {
                return dang::BTNodeStatus::SUCCESS;
            }
            else
            {
                return dang::BTNodeStatus::RUNNING;
            }
        }
        else
        {
            spWaypoint w = _path[_path_index].lock();
            if (w)
            {
                // if we missed somehow the waypoint..
                if ((_vel.x < 0 && getHotrectAbs().center().x < w->_pos.x) || (_vel.x > 0 && getHotrectAbs().center().x > w->_pos.x))
                {
                    _vel.x = -_vel.x;
                    _vel.x /= 2;
                }
            }
            return dang::BTNodeStatus::RUNNING;
        }

    }

    void Enemy::startOutToWaypoint()
    {
        wpWaypoint wp = _path[_path_index];
        spWaypoint spwp = wp.lock();
        if (spwp)
        {
            uint32_t conn_type = _scene_graph->getConnectionType(_current_wp, wp);
            switch (conn_type)
            {
                case dang::e_tmx_waypoint_connection::wp_walk:
                {
                    removeTweens(true);
                    _vel.x = spwp->_pos.x - _pos.x < 0 ? -_walkSpeed : _walkSpeed;
                    _current_wp = wp;
                    break;
                }
                case dang::e_tmx_waypoint_connection::wp_jump:
                {
                    removeTweens(true);
                    float vx = spwp->_pos.x - _pos.x < 0 ? -_walkSpeed : _walkSpeed;
                    spTwVel tw = std::make_shared<dang::TwVel>(dang::Vector2F(vx*1.5, -16), dang::Vector2F(vx, 0), 600, &dang::Ease::OutQuad, 1, false );
                    addTween(tw);
                    _vel.x = spwp->_pos.x - _pos.x < 0 ? -_walkSpeed : _walkSpeed;
                    _current_wp = wp;
                    break;
                }
                case dang::e_tmx_waypoint_connection::wp_warp:
                    // TODO
                    break;
                default:
                    break;
            }
        }

    }

}