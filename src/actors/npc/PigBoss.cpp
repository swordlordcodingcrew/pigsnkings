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
    }

    PigBoss::PigBoss(const dang::tmx_spriteobject* so, dang::spImagesheet is) : pnk::Enemy(so, is)
    {
        _hotrect = {10, 10, 12, 22};
        _walkSpeed = _loiter_speed;     // king is on steroids
    }

    void PigBoss::init()
    {
        onEnterSleeping();
    }

    PigBoss::~PigBoss()
    {
#ifdef PNK_DEBUG_PRINT
        std::cout << "PigBoss destructor" << std::endl;
#endif
    }

    // TODO: move into the onDying nextstate etc structure
    void PigBoss::die()
    {
        _nextState = DEAD;
    }

    void PigBoss::update(uint32_t dt)
    {
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

    uint8_t  PigBoss::getCollisionResponse(const dang::spCollisionObject& other)
    {
        dang::spCollisionSprite cs_other = std::static_pointer_cast<CollisionSprite>(other);
        /** run into the king */
        if (cs_other->_type_num == ST_KING)
        {
            if (_currentState == DEAD || _currentState == HIDING || _hit)
            {
                _cr = dang::CR_NONE;
            }
            else
            {
                _cr = dang::CR_BOUNCE;
            }
        }
        /** hit a platform hotrect */
        else if (cs_other->_type_num == ST_HOTRECT)
        {
            _cr = dang::CR_SLIDE;
        }
        else
        {
            _cr = dang::CR_NONE;
        }

        return _cr;

    }

    void PigBoss::collide(const dang::manifold &mf)
    {
        dang::spCollisionSprite sprOther = getOther(mf, this);
//        dang::spCollisionSprite sprOther = std::static_pointer_cast<CollisionSprite>(mf.me.get() == this ? mf.other : mf.me);

        // warning, this is the other way round than in, say, the hero
        // we want the others normal, not our normal!
        const dang::Vector2F& normal = mf.me.get() == this ? mf.normalOther : mf.normalMe;

        if (sprOther->_type_num == ST_KING && _currentState == LOITERING)
        {
            if (normal.y > 0)
            {
                if (!_hit)
                {
                    std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_BOSS_HIT));
                    pnk::_pnk._dispatcher.queueEvent(std::move(e));
                    _hit = true;
                    _walkSpeed = _hiding_speed;
                    _vel.x = _vel.x > 0 ? _walkSpeed : -_walkSpeed;
                }
            }
            else
            {
                tellTheKingWeHitHim();
            }
        }
        else if (_cr == dang::CR_SLIDE)
        {
            const dang::Vector2F &normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.x != 0)
            {
                _vel.x = 0;
            }

            if (normal.y > 0)
            {
                _on_ground = true;
                _vel.y = 0;
            }
        }

        /** collision with bubble is handled in the bubble sprite */
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
        _vel.x = 0;

        _currentState = DEAD;

        return true;
    }

    void PigBoss::tellTheKingWeHitHim()
    {
        pnk::_pnk._dispatcher.queueEvent(PnkEvent::createGE(ETG_KING_HIT, ST_PIG_BOSS, shared_from_this()));

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
        if (_vel.x > 0)
        {
            _transform = blit::HORIZONTAL;
        }
        else
        {
            _transform = blit::NONE;
        }
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

    dang::BTNode::Status PigBoss::NTLurk(dang::spSprite s)
    {
        std::shared_ptr<PigBoss> spr = std::static_pointer_cast<PigBoss>(s);
        assert(spr != nullptr);
        if (spr->_currentState != SLEEPING && spr->_nextState != SLEEPING)
        {
            spr->prepareChangeState(SLEEPING);
            return dang::BTNode::Status::RUNNING;
        }
        else if  (spr->_currentState == SLEEPING && spr->_nextState == SLEEPING)
        {
            return dang::BTNode::Status::SUCCESS;
        }

        return dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status PigBoss::NTRun(dang::spSprite s)
    {
        std::shared_ptr<PigBoss> spr = std::static_pointer_cast<PigBoss>(s);
        assert(spr != nullptr);
        if (spr->_currentState != LOITERING && spr->_nextState != LOITERING)
        {
            dang::BTNode::Status ret1 = spr->setRandNeighbourWaypoint();
            if (ret1 == dang::BTNode::Status::FAILURE)
            {
                return dang::BTNode::Status::FAILURE;
            }
            spr->prepareChangeState(LOITERING);
            return dang::BTNode::Status::RUNNING;
        }
        else if  (spr->_currentState == LOITERING && spr->_nextState == LOITERING)
        {
            return dang::BTNode::Status::SUCCESS;
        }

        return dang::BTNode::Status::FAILURE;

    }

    dang::BTNode::Status PigBoss::NTHit(dang::spSprite s)
    {
        std::shared_ptr<PigBoss> spr = std::static_pointer_cast<PigBoss>(s);
        assert(spr != nullptr);
        if (spr->_hit)
        {
            return dang::BTNode::Status::SUCCESS;
        }
        return dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status PigBoss::NTRecover(dang::spSprite s)
    {
        std::shared_ptr<PigBoss> spr = std::static_pointer_cast<PigBoss>(s);
        assert(spr != nullptr);
        if (spr->_currentState != HIDING && spr->_nextState != HIDING)
        {
            spr->prepareChangeState(HIDING);
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr->_currentState == HIDING && spr->_nextState == HIDING)
        {
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr->_currentState == HIDING && spr->_nextState != HIDING)
        {
            return dang::BTNode::Status::SUCCESS;

        }
        return dang::BTNode::Status::FAILURE;
    }

}