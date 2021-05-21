// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>
#include <path/SceneGraph.hpp>
#include <tween/TwVel.hpp>

//#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"

#include "Enemy.h"
#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"

namespace pnk
{
    using spTwVel = std::shared_ptr<dang::TwVel>;

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
/*        if (_bubbled)
        {
            return dang::CollisionSpriteLayer::CR_NONE;
        }

        if (other->_type_num == SpriteFactory::TN_KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }
*/
        return dang::CollisionSpriteLayer::CR_SLIDE;
    }

    void Enemy::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
    }

    void Enemy::handlePath()
    {

/*        if (_path.empty())  // no goal
        {
            if (_scene_graph->waypointReached(getHotrectAbs(), _current_wp))    // sprite is at current waypoint
            {
                // TODO: ask Behaviour tree what to do
                randomNextWaypoint();
            }
            else if (!_current_wp.expired())    // sprite goes back to current waypoint
            {
                checkPathProgress();
            }
            else    // no goal, no current waypoint - in limbo
            {
                // try to get back
                spWaypoint wp = _scene_graph->getNearestWaypoint(_pos);

                if (wp)
                {
                    _current_wp = wp;
                    // hopefully no jump required
                    _vel.x = wp->_pos.x - _pos.x < 0 ? -_walkSpeed : _walkSpeed;
                }
            }

        }
*/
        if (!_path.empty())
        {
            if (_scene_graph->waypointReached(getHotrectAbs(), _path[_path_index]))
            {
                if (_path_index + 1 == _path.size())
                {
                    // ultimate goal reached. Reset stuff
                    std::cout << "path: reached goal " << _type_num << std::endl;
                    _current_wp = _path[_path_index];

                    _path.clear();
                    _path_index = 0;
                    _vel.x = 0;
                }
                else
                {
                    _current_wp = _path[_path_index];
                    _path_index++;
                    startOutToWaypoint();
                }

            }
        }
        else
        {
            randomNextWaypoint();
            if (!_path.empty())
            {
                startOutToWaypoint();
            }
        }
    }

    void Enemy::randomNextWaypoint()
    {
        _scene_graph->getRandomNextWaypoint(_current_wp, _path);
        _path_index = 0;
        if (_path.empty())
        {
            // something went wrong. Waiting..
            _vel.x = 0;
        }
    }

    void Enemy::checkPathProgress()
    {

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
                    _vel.x = spwp->_pos.x - _pos.x < 0 ? -_walkSpeed : _walkSpeed;
                    _current_wp = wp;
                    break;

                }
                case dang::e_tmx_waypoint_connection::wp_jump:
                {
                    float vx = spwp->_pos.x - _pos.x < 0 ? -_walkSpeed : _walkSpeed;
                    spTwVel twv = std::make_shared<dang::TwVel>(dang::Vector2F(vx*2, -12), dang::Vector2F(vx, -6), 600, &dang::Ease::InQuad, 1, false );
                    addTween(twv);
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