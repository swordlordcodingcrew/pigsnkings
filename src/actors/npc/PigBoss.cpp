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

namespace pnk
{
    extern PigsnKings _pnk;

    PigBoss::PigBoss() : pnk::Enemy()
    {
    }

    PigBoss::PigBoss(const dang::tmx_spriteobject* so, dang::spImagesheet is) : pnk::Enemy(so, is)
    {
        _hotrect = {10, 10, 12, 22};
        _nextState = BERSERK;
    }

    void PigBoss::init()
    {
        setVel({0, 0});
    }

    PigBoss::~PigBoss()
    {
#ifdef PNK_DEBUG
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
                case BERSERK:
                    onEnterBerserk();
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

    dang::CollisionSpriteLayer::eCollisionResponse PigBoss::getCollisionResponse(const dang::spCollisionSprite& other)
    {
        /** run into the king */
        if (other->_type_num == dang::SpriteType::KING)
        {
            _coll_response = dang::CollisionSpriteLayer::CR_BOUNCE;
        }
        /** hit a platform hotrect */
        else if (other->_type_num == dang::SpriteType::HOTRECT_PLATFORM)
        {
            if (other->getHotrectAbs().top() - 6 >= this->_last_pos.y + _hotrect.h && _vel.y > 0)
            {
                _coll_response = dang::CollisionSpriteLayer::CR_SLIDE;
            }
            else
            {
                _coll_response = dang::CollisionSpriteLayer::CR_CROSS;
            }
        }
        else
        {
            _coll_response = dang::CollisionSpriteLayer::CR_SLIDE;
        }

        return _coll_response;

    }

    void PigBoss::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        dang::spCollisionSprite sprOther = mf.me.get() == this ? mf.other : mf.me;

        // warning, this is the other way round than in, say, the hero
        // we want the others normal, not our normal!
        const dang::Vector2F& normal = mf.me.get() == this ? mf.normalOther : mf.normalMe;

        if (sprOther->_type_num == dang::SpriteType::KING && normal.y > 0 && _currentState == BERSERK)
        {
            // TODO check first if the other one can still collide?
            // tell the pig king he is hit, should be stunned for a few rounds
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_BOSS_HIT));
            pnk::_pnk._dispatcher.queueEvent(std::move(e));
        }
        else if (_coll_response == dang::CollisionSpriteLayer::CR_SLIDE)
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

    dang::BTNode::Status PigBoss::sleep()
    {
        if (_currentState == SLEEPING)
        {
            return dang::BTNode::Status::RUNNING;
        }

        prepareChangeState(SLEEPING);

        return dang::BTNode::Status::SUCCESS;
    }

    bool PigBoss::onEnterSleeping()
    {
        _nTreeStateDepot = std::move(_nTreeState);
        assert(_anim_m_sleeping != nullptr);
        setAnimation(_anim_m_sleeping);

        removeTweens(true);
        uint32_t sleep_duration = std::rand() % 1500 + 500;    //!< sleep between 0.5 to 2 secs
        dang::spTwNull nullTw = std::make_shared<dang::TwNull>(sleep_duration, dang::Ease::Linear, 1);
        nullTw->setFinishedCallback(std::bind(&PigBoss::endSleep, this));
        addTween(nullTw);

        _currentState = SLEEPING;

        return true;
    }

    void PigBoss::endSleep()
    {
        prepareChangeState(LOITERING);
    }

    bool PigBoss::onEnterLoitering()
    {
        // activate the behaviour tree, if not already active
        resetPathVars();
        if (_nTreeState == nullptr)
        {
            _nTreeState = std::move(_nTreeStateDepot);
        }

        _currentState = LOITERING;
        return true;
    }

    void PigBoss::endLoitering()
    {
    }

    bool PigBoss::onEnterBerserk()
    {
        std::cout << "enter berserk" << std::endl;
        _walkSpeed = _raging_speed;

        // activate the behaviour tree, if not already active
        resetPathVars();
        if (_nTreeState == nullptr)
        {
            _nTreeState = std::move(_nTreeStateDepot);
        }

        removeTweens(true);
        // rage for 10 sec
//        dang::spTwNull nullTw = std::make_shared<dang::TwNull>(10000, dang::Ease::Linear, 1);
//        nullTw->setFinishedCallback(std::bind(&PigBoss::endBerserk, this));
//        addTween(nullTw);

        _currentState = BERSERK;

        return true;
    }

    void PigBoss::endBerserk()
    {
        _walkSpeed = _loiter_speed;
        prepareChangeState(LOITERING);
    }

    bool PigBoss::onEnterDead()
    {
        std::cout << "enter dead" << std::endl;

        setAnimation(_anim_m_die);
        removeTweens(true);

        _currentState = DEAD;

        return true;
    }

    void PigBoss::tellTheKingWeHitHim()
    {
        //
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_spr = shared_from_this();
        e->_payload = static_cast<uint16_t>(dang::SpriteType::PIG_BOSS);
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void PigBoss::removeSelf()
    {
        // remove throwie
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
        e->_spr = shared_from_this();
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void PigBoss::startOutToWaypoint()
    {
        Enemy::startOutToWaypoint();
        removeAnimation();
        setAnimation(_animation);
        if (_vel.x > 0)
        {
            _transform = blit::HORIZONTAL;
        }
        else
        {
            _transform = blit::NONE;

        }
    }

    dang::BTNode::Status PigBoss::NTSleep(dang::spSprite s)
    {
        //        std::cout << "NTSleep" << std::endl;
        std::shared_ptr<PigBoss> spr = std::dynamic_pointer_cast<PigBoss>(s);
        return (spr ? spr->sleep() : dang::BTNode::Status::FAILURE);
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
}