// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/pigsnkings.hpp"
#include "PigBoss.h"

#include <tween/TwAnim.hpp>
#include <tween/TwNull.hpp>
#include <Imagesheet.hpp>
#include <TmxExtruder.hpp>

#include <iostream>
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
        std::cout << "enter dead" << std::endl;

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

//        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
//        e->_spr = shared_from_this();
//        e->_payload = ST_PIG_BOSS;
//        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void PigBoss::removeSelf()
    {
        markRemove();

//        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
//        e->_spr = shared_from_this();
//        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void PigBoss::startOutToWaypoint()
    {
        Enemy::startOutToWaypoint();
        removeAnimation();
        setAnimation(_anim_m_running);
        setTransform(getVel().x > 0 ? blit::HORIZONTAL : blit::NONE);
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