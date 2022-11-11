// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/pigsnkings.hpp"
#include "PigBoss.h"

#include <tween/TwAnim.hpp>
#include <tween/TwNull.hpp>
#include <tween/TwVel.hpp>
#include <tween/TwVelY.hpp>
#include <Imagesheet.hpp>
#include <TmxExtruder.hpp>
#include <bt/NTreeState.h>
#include <path/SceneGraph.hpp>

#include <cassert>

namespace pnk
{
    extern PigsnKings _pnk;

    PigBoss::PigBoss() : pnk::Enemy()
    {
        _hotrect = {10, 10, 12, 22};
        _walkSpeed = _loiter_speed;     // king is on steroids
    }

    PigBoss::PigBoss(const dang::tmx_spriteobject* so, dang::spImagesheet is) : pnk::Enemy(so, is)
    {
        _hotrect = {10, 10, 12, 22};
        _walkSpeed = _loiter_speed;     // king is on steroids
    }

    void PigBoss::initBT(dang::spNTreeState nts)
    {
        setNTreeState(nts);
        onEnterSleeping();
    }

    PigBoss::~PigBoss()
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("PigBoss destructor\n");
#endif
    }

    // TODO: move into the onDying nextstate etc structure
    void PigBoss::die()
    {
        _nextState = DEAD;
    }

    void PigBoss::update(uint32_t dt)
    {
        this->dang::FullColSpr::update(dt);

        _on_ground = false;

        if(_currentState != _nextState)
        {
            // If you need some logic here, have it decided by a BehaviourTree
            switch (_nextState)
            {
                case SLEEPING:
                    onEnterSleeping();
                    break;
                case LOITERING:
                    onEnterLoitering();
                    break;
                case HIDING:
                    onEnterHiding();
                    break;
                case DEAD:
                    onEnterDead();
                    break;
                case REMOVE_SELF:
                    removeSelf();
                    break;
            }
        }
    }

    uint8_t  PigBoss::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = static_cast<const dang::ColSpr*>(other);

        /** run into the king */
        if (cs_other->typeNum() == ST_KING)
        {
            if (_currentState == DEAD || _currentState == HIDING || _hit)
            {
                return dang::CR_NONE;
            }
            else
            {
                return dang::CR_BOUNCE;
            }
        }
        /** hit a platform hotrect */
        else if (cs_other->typeNum() == ST_HOTRECT)
        {
            return dang::CR_SLIDE;
        }

        return dang::CR_NONE;

    }

    void PigBoss::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_KING && _currentState == LOITERING)
        {
            const dang::Vector2F& o_normal = static_cast<dang::ColSpr*>(mf.me.get()) == this ? mf.normalOther : mf.normalMe;

            if (o_normal.y > 0)
            {
                if (!_hit)
                {
                    std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_BOSS_HIT));
                    e->_pos = getCSPosition();
                    pnk::_pnk._dispatcher.queueEvent(std::move(e));
                    _hit = true;
                    _walkSpeed = _hiding_speed;
                    setVelX(getVel().x > 0 ? _walkSpeed : -_walkSpeed);
                }
            }
            else
            {
                tellTheKingWeHitHim();
            }
        }
        else if (sprOther->typeNum() == ST_HOTRECT)
        {
            const dang::Vector2F& normal = static_cast<dang::ColSpr*>(mf.me.get()) == this ? mf.normalMe : mf.normalOther;

            if (normal.x != 0)
            {
                setVelX(0);
            }

            if (normal.y > 0)
            {
                _on_ground = true;
                setVelY(0);
            }
        }

    }

    void PigBoss::prepareChangeState(e_state wishedState)
    {
        _nextState = wishedState;
    }

    bool PigBoss::onEnterSleeping()
    {
        assert(_anim_m_sleeping != nullptr);
        setAnimation(_anim_m_sleeping);

        setVel({0,0});

        _currentState = SLEEPING;

        return true;
    }

    void PigBoss::endSleep()
    {
        prepareChangeState(LOITERING);
    }

    bool PigBoss::onEnterLoitering()
    {
        _currentState = LOITERING;
        return true;
    }

    void PigBoss::endLoitering()
    {
        prepareChangeState(SLEEPING);
    }

    bool PigBoss::onEnterHiding()
    {
        _currentState = HIDING;
        dang::spTwNull nullTw = std::make_shared<dang::TwNull>(BOSS_RECOVER_TIME, dang::Ease::Linear, 1);
        nullTw->setFinishedCallback(std::bind(&PigBoss::endHiding, this));
        addTween(nullTw);

        setAnimation(_anim_m_recovering);
        return true;
    }

    void PigBoss::endHiding()
    {
        _walkSpeed = _loiter_speed;
        _hit = false;
        prepareChangeState(SLEEPING);
    }

    bool PigBoss::onEnterDead()
    {
        printf("enter dead\n");

        setAnimation(_anim_m_die);
        removeTweens(true);
        _nTreeState.reset();
        setVelX(0);

        _currentState = DEAD;

        return true;
    }

    void PigBoss::tellTheKingWeHitHim()
    {
        pnk::_pnk._dispatcher.queueEvent(PnkEvent::createGE(ETG_KING_HIT, ST_PIG_BOSS));
    }

    void PigBoss::removeSelf()
    {
        markRemove();
    }

    void PigBoss::bubble()
    {
        // do nothing, only non royal enemies bubble
    }

    void PigBoss::endBubble()
    {
        // do nothing, only non royal enemies bubble
    }

    bool PigBoss::isBubbled()
    {
        return false;
    }

/*    void PigBoss::startOutToWaypoint()
    {
        Enemy::startOutToWaypoint();
        removeAnimation();
        setAnimation(_anim_m_running);
        setTransform(getVel().x > 0 ? blit::HORIZONTAL : blit::NONE);
    }
*/
    void PigBoss::startOutToWaypoint()
    {
        const dang::Waypoint* wp = _path.at(_path_index);

#ifdef PNK_DEBUG_WAYPOINTS
        DEBUG_PRINT("(spr id %u): start out from wp %u to wp %u\n", id(), (_current_wp == nullptr ? 0 : _current_wp->_id), wp->_id);
#endif

        removeTween(_tw_short_jump, true);
        removeTween(_tw_long_horiz_jump, true);

        uint32_t conn_type = _scene_graph->getConnectionType(_current_wp, wp);
        switch (conn_type)
        {
            case dang::e_tmx_waypoint_connection::wpc_invalid:
            case dang::e_tmx_waypoint_connection::wpc_walk:
            {
                setVelX(wp->_pos.x - _co_pos.x < 0 ? -_walkSpeed : _walkSpeed);
                _max_time_to_wp = (std::fabs(wp->_pos.x - getHotrectG().center().x) + 32) * 100 / _walkSpeed;
                _time_elapsed_to_wp = 0;
                break;
            }
            case dang::e_tmx_waypoint_connection::wpc_jump:
            {
                dang::Vector2F v{0,0};
                dang::Vector2F v_end{0,0};
                dang::Vector2F hrc = getHotrectG().center();
                if (hrc.y - wp->_pos.y > 10)     // safety buffer of 10 units for the check if higher / lower
                {
                    // the waypoint is higher than the hero
                    v.y = (wp->_pos.y - hrc.y)/2 - (0.4f * _walkSpeed);
                }
                else
                {
                    // equal or lower
                    v.y = -5.5 + (static_cast<double>(0.25f * _walkSpeed));
                }

                if ((wp->_pos.x - hrc.x) * (wp->_pos.x - hrc.x) > 1600)  // long horizontal distance
                {
                    if (wp->_pos.x - _co_pos.x < 0)
                    {
                        v.x = -(_walkSpeed + 7);
                        v_end.x = -_walkSpeed;
                    }
                    else
                    {
                        v.x = (_walkSpeed + 7);
                        v_end.x = _walkSpeed;
                    }
                    setVelX(v.x);
                    _max_time_to_wp = 2000;
                    _time_elapsed_to_wp = 0;
                    _tw_long_horiz_jump = std::make_shared<dang::TwVel>(v, v_end, 650-_walkSpeed*20, &dang::Ease::Linear, 1, false );
                    addTween(_tw_long_horiz_jump);
                }
                else
                {
                    // short jump
                    _max_time_to_wp = 1000;

                    _time_elapsed_to_wp = 0;
                    setVelX(wp->_pos.x - _co_pos.x < 0 ? -_walkSpeed : _walkSpeed);
                    _tw_short_jump = std::make_shared<dang::TwVelY>(v.y, 0.0f, 400/_walkSpeed, &dang::Ease::Linear, 1, false );
                    addTween(_tw_short_jump);
                }
                break;
            }
            case dang::e_tmx_waypoint_connection::wpc_warp:
                break;
            default:
                break;
        }
        removeAnimation();
        setAnimation(_anim_m_running);
        setTransform(getVel().x > 0 ? blit::HORIZONTAL : blit::NONE);

    }

    dang::BTNode::Status PigBoss::NTLurk(dang::FullColSpr& s, uint32_t dt)
    {
        PigBoss& spr = static_cast<PigBoss&>(s);

        if (spr._currentState != SLEEPING && spr._nextState != SLEEPING)
        {
            spr.prepareChangeState(SLEEPING);
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr._currentState == SLEEPING && spr._nextState == SLEEPING)
        {
            return dang::BTNode::Status::SUCCESS;
        }

        return dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status PigBoss::NTRun(dang::FullColSpr& s, uint32_t dt)
    {
        PigBoss& spr = static_cast<PigBoss&>(s);

        if (spr._currentState != LOITERING && spr._nextState != LOITERING)
        {
            dang::BTNode::Status ret1 = spr.setRandNeighbourWaypoint();
            if (ret1 == dang::BTNode::Status::FAILURE)
            {
                return dang::BTNode::Status::FAILURE;
            }
            spr.prepareChangeState(LOITERING);
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr._currentState == LOITERING && spr._nextState == LOITERING)
        {
            return dang::BTNode::Status::SUCCESS;
        }

        return dang::BTNode::Status::FAILURE;

    }

    dang::BTNode::Status PigBoss::NTRunToPOI(dang::FullColSpr& s, uint32_t dt)
    {
        PigBoss& spr = static_cast<PigBoss&>(s);

        if (spr._currentState != LOITERING && spr._nextState != LOITERING)
        {
            if (spr._nTreeState->_payload.count("aaLoSH") == 0)
            {
                return dang::BTNode::Status::FAILURE;
            }
            float dist = spr._nTreeState->_payload["aaLoSH"];
            spr._nTreeState->_payload.erase("aaLoSH");

            dang::BTNode::Status ret1 = spr.setDestinationWaypointByType(dist < 0 ? WPT_POI : WPT_POI2);
            if (ret1 == dang::BTNode::Status::FAILURE)
            {
                return dang::BTNode::Status::FAILURE;
            }
            spr.prepareChangeState(LOITERING);
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr._currentState == LOITERING && spr._nextState == LOITERING)
        {
            return dang::BTNode::Status::SUCCESS;
        }

        return dang::BTNode::Status::FAILURE;

    }

    dang::BTNode::Status PigBoss::NTHit(dang::FullColSpr& s, uint32_t dt)
    {
        PigBoss& spr = static_cast<PigBoss&>(s);

        if (spr._hit)
        {
            return dang::BTNode::Status::SUCCESS;
        }
        return dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status PigBoss::NTRecover(dang::FullColSpr& s, uint32_t dt)
    {
        PigBoss& spr = static_cast<PigBoss&>(s);

        if (spr._currentState != HIDING && spr._nextState != HIDING)
        {
            spr.prepareChangeState(HIDING);
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr._currentState == HIDING && spr._nextState == HIDING)
        {
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr._currentState == HIDING && spr._nextState != HIDING)
        {
            return dang::BTNode::Status::SUCCESS;
        }
        return dang::BTNode::Status::FAILURE;
    }

}