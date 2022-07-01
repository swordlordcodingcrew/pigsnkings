// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/pigsnkings.hpp"
#include "HenchPig.h"

#include <bt/NTreeState.h>
#include <tween/TwAnim.hpp>
#include <tween/TwNull.hpp>
#include <Rand.hpp>

#include <iostream>
#include <cassert>


namespace pnk
{

    extern PigsnKings _pnk;

    HenchPig::HenchPig() : pnk::Enemy()
    {
    }

    HenchPig::HenchPig(const dang::tmx_spriteobject* so, const dang::spImagesheet& is) : pnk::Enemy(so, is)
    {
    }

    void HenchPig::init()
    {
        _hotrect = {10, 16, 12, 16};

        if (_nTreeState != nullptr)
        {
            _nTreeStateDefault = _nTreeState;
            _nTreeState->_payload["sleep_duration"] = dang::Rand::get(uint32_t(500), uint32_t(1500));
            onEnterSleeping();
        }
        else
        {
            onEnterSleeping();
        }

        setVel({0, 0});
    }

    HenchPig::~HenchPig()
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("HenchPig destructor\n");
#endif
    }

    void HenchPig::update(uint32_t dt)
    {
        this->dang::FullSpr::update(dt);

        _on_ground = false;

        if(_currentState != _nextState)
        {
            // If you need some logic here, have it decided by a BehaviourTree
            switch (_nextState)
            {
                case SLEEPING:
                    onEnterSleeping();
                    break;
                case HIDING:
                    onEnterHiding();
                    break;
                case LOITERING:
                    onEnterLoitering();
                    break;
                case BERSERK:
                    onEnterBerserk();
                    break;
                case THROWING:
                    onEnterThrowing();
                    break;
                case PICKING_UP:
                    onEnterPickingUp();
                    break;
                case BUBBLED:
                    onEnterBubbled();
                    break;
                case REMOVE_SELF:
                    removeSelf();
                    break;
            }
        }
    }

    uint8_t  HenchPig::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = dynamic_cast<const ColSpr*>(other);

        /** enemy is bubbled */
        if (_currentState == BUBBLED || _currentState == REMOVE_SELF || _nextState == REMOVE_SELF)
        {
            return dang::CR_NONE;
        }
        /** run into the king */
        else if (cs_other->typeNum() == ST_KING || cs_other->typeNum() == ST_BUBBLE)
        {
            return dang::CR_CROSS;
        }
        /** hit a platform hotrect */
        else if (cs_other->typeNum() == ST_HOTRECT_PLATFORM)
        {
            if (other->getCSPosition().y + cs_other->getHotrect().top() - 6 >= this->_co_pos.y + _hotrect.h && getVel().y > 0)
            {
                return dang::CR_SLIDE;
            }
            else
            {
                return dang::CR_CROSS;
            }
        }
        else if (cs_other->typeNum() == ST_HOTRECT)
        {
            return dang::CR_SLIDE;
        }

        return dang::CR_NONE;

    }

    void HenchPig::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        const dang::CollisionObject* co_this = static_cast<dang::CollisionObject*>(this);
        const dang::Vector2F& normal = mf.me.get() == co_this ? mf.normalMe : mf.normalOther;

        if (sprOther->typeNum() == ST_KING)
        {
            tellTheKingWeHitHim();

            poofing();
        }
        /** stand on something solid */
        else if (normal.y > 0)
        {
            setVelY(0);
            _on_ground = true;
        }
        /** hit somethin above */
        else if (normal.y < 0 && sprOther->typeNum() == ST_HOTRECT)
        {
            setVelY(0);
        }
        else if (normal.x != 0)
        {
            setVelX(0);
        }
    }

    void HenchPig::prepareChangeState(e_state wishedState)
    {
        _nextState = wishedState;
    }

    bool HenchPig::onEnterSleeping()
    {
        assert(_anim_m_sleeping != nullptr);
        setAnimation(_anim_m_sleeping);

        if (_nTreeState != nullptr)
        {
            if (_nTreeState->_payload.count("sleep_duration") > 0)
            {
                dang::spTwNull nullTw = std::make_shared<dang::TwNull>(_nTreeState->_payload["sleep_duration"], dang::Ease::Linear, 1);
                nullTw->setFinishedCallback(std::bind(&HenchPig::endSleep, this));
                addTween(nullTw);
            }
        }

        _currentState = SLEEPING;

        return true;
    }

    void HenchPig::endSleep()
    {
        prepareChangeState(LOITERING);
    }

    bool HenchPig::onEnterLoitering()
    {
        // activate the behaviour tree, if not already active
        if (_nTreeState != _nTreeStateDefault)
        {
            _nTreeState = _nTreeStateDefault;
        }

        _currentState = LOITERING;
        return true;
    }

    void HenchPig::endLoitering()
    {
        _nTreeState.reset();
        if (_nTreeStateDefault != nullptr)
        {
            _nTreeStateDefault->clearState();
        }
        resetPathVars();
    }

    bool HenchPig::onEnterBerserk()
    {
        DEBUG_PRINT("enter berserk");
        _walkSpeed = _berserk_speed;
        _loiter_anim_duration = _anim_m_loitering->duration();
        _anim_m_loitering->duration(_loiter_anim_duration / 2);

        // activate the behaviour tree, if not already active
        resetPathVars();
        if (_nTreeState != _nTreeStateBerserk)
        {
            _nTreeState = _nTreeStateBerserk;
        }

//        removeTweens(true);
        dang::spTwNull nullTw = std::make_shared<dang::TwNull>(BERSERK_DURATION, dang::Ease::Linear, 1);
        nullTw->setFinishedCallback(std::bind(&HenchPig::endBerserk, this));
        addTween(nullTw);

        _currentState = BERSERK;

        return true;
    }

    void HenchPig::endBerserk()
    {
        DEBUG_PRINT("end berserk");
        _walkSpeed = _loiter_speed;
        _anim_m_loitering->duration(_loiter_anim_duration);
        _nTreeState.reset();
        _nTreeStateBerserk->clearState();
        resetPathVars();
        prepareChangeState(LOITERING);
    }

    bool HenchPig::onEnterHiding()
    {
        // handled by subclasses
        return false;
    }


    bool HenchPig::onEnterThrowing()
    {
        // handled by subclasses
        return false;
    }

    void HenchPig::endThrowing()
    {
    }

    bool HenchPig::onEnterPickingUp()
    {
        // handled by subclasses
        return false;
    }

    void HenchPig::bubble()
    {
        prepareChangeState(BUBBLED);
    }

    bool HenchPig::onEnterBubbled()
    {
        // Remark: depending on subclass and type of henchpig the pig will let crates or bombs fall to the ground

        setGravity({0,0});
        setVel(0,0);
        removeAnimation();
        removeTweens(true);

        _anim_m_bubbling->reset();
        setAnimation(_anim_m_bubbling);
        _nTreeState.reset();
        if (_nTreeStateDefault != nullptr)
        {
            _nTreeStateDefault->clearState();
        }

        _currentState = BUBBLED;
        return true;
    }

    void HenchPig::endBubble()
    {
        resetPathVars();

        setGravity(PigsnKings::_gravity);
        removeAnimation();
        _anim_m_loitering->reset();
        setAnimation(_anim_m_loitering);

        // Pigs are aggressive when debubbled,
        // don't just loiter, piggie!
        prepareChangeState(BERSERK);
    }

    bool HenchPig::isBubbled()
    {
        return _currentState == BUBBLED;
    }



    void HenchPig::tellTheKingWeHitHim()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_payload = ST_PIG_NORMAL;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void HenchPig::poofing()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_POOF));
        e->_pos = this->getPos();
        e->_pos.y += 5;
        _pnk._dispatcher.queueEvent(std::move(e));

        _nextState = REMOVE_SELF;
    }

    void HenchPig::removeSelf()
    {
        markRemove();
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_SPR_CONSUMED_BY_HERO, id()));
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void HenchPig::startOutToWaypoint()
    {
        Enemy::startOutToWaypoint();
        removeAnimation();
        setAnimation(_anim_m_loitering);
        setTransform(getVel().x > 0 ? blit::HORIZONTAL : blit::NONE);
    }

    dang::BTNode::Status HenchPig::NTsetSleepShort(dang::FullSpr& s, uint32_t dt)
    {
        HenchPig& spr = dynamic_cast<HenchPig&>(s);
        uint32_t duration = dang::Rand::get(uint32_t(500), uint32_t(1500));
        spr._nTreeState->_payload["sleep_duration"] = duration;
        spr.prepareChangeState(SLEEPING);
        return dang::BTNode::Status::SUCCESS;
    }

    dang::BTNode::Status HenchPig::NTsetSleepMedium(dang::FullSpr& s, uint32_t dt)
    {
        HenchPig& spr = dynamic_cast<HenchPig&>(s);
        uint32_t duration = dang::Rand::get(uint32_t(2000), uint32_t(4000));
        spr._nTreeState->_payload["sleep_duration"] = duration;
        spr.prepareChangeState(SLEEPING);
        return dang::BTNode::Status::SUCCESS;
    }

    dang::BTNode::Status HenchPig::NTsetSleepLong(dang::FullSpr& s, uint32_t dt)
    {
        HenchPig& spr = dynamic_cast<HenchPig&>(s);
        uint32_t duration = dang::Rand::get(uint32_t(5000), uint32_t(10000));
        spr._nTreeState->_payload["sleep_duration"] = duration;
        spr.prepareChangeState(SLEEPING);
        return dang::BTNode::Status::SUCCESS;
    }

    dang::BTNode::Status HenchPig::NTdoSleep(dang::FullSpr& s, uint32_t dt)
    {
        HenchPig& spr = dynamic_cast<HenchPig&>(s);
        if (spr._currentState == SLEEPING || spr._nextState == SLEEPING)
        {
            return dang::BTNode::Status::RUNNING;
        }

        return dang::BTNode::Status::SUCCESS;
    }

}