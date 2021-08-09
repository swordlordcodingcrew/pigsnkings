// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/pigsnkings.hpp"
#include "HenchPig.h"

#include <tween/TwAnim.hpp>
#include <tween/TwNull.hpp>
#include <Rand.hpp>

#include <iostream>


namespace pnk
{

    extern PigsnKings _pnk;

    HenchPig::HenchPig() : pnk::Enemy()
    {
    }

    HenchPig::HenchPig(const dang::tmx_spriteobject* so, dang::spImagesheet is) : pnk::Enemy(so, is)
    {
    }

    void HenchPig::init()
    {
        _hotrect = {10, 16, 12, 16};

        onEnterSleeping();
//        onEnterLoitering();

        setVel({0, 0});
    }

    HenchPig::~HenchPig()
    {
#ifdef PNK_DEBUG
        std::cout << "HenchPig destructor" << std::endl;
#endif
    }

    void HenchPig::update(uint32_t dt)
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
                case HIDING:
                    onEnterHiding();
                    break;
                case LOITERING:
                    onEnterLoitering();
                    break;
                case RAGING:
                    onEnterRaging();
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

    dang::CollisionSpriteLayer::eCollisionResponse HenchPig::getCollisionResponse(const dang::spCollisionSprite& other)
    {
        /** enemy is bubbled */
        if (_currentState == BUBBLED)
        {
            _coll_response = dang::CollisionSpriteLayer::CR_NONE;
        }
        /** run into the king */
        else if (other->_type_num == dang::SpriteType::KING)
        {
            _coll_response = dang::CollisionSpriteLayer::CR_CROSS;
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

    void HenchPig::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        dang::spCollisionSprite sprOther = mf.me.get() == this ? mf.other : mf.me;

        if (sprOther->_type_num == dang::SpriteType::KING)
        {
            tellTheKingWeHitHim();

            poofing();
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

    void HenchPig::prepareChangeState(e_state wishedState)
    {
        _nextState = wishedState;
    }

    dang::BTNode::Status HenchPig::sleep()
    {
        if (_currentState == SLEEPING)
        {
            return dang::BTNode::Status::RUNNING;
        }

        prepareChangeState(SLEEPING);

        return dang::BTNode::Status::SUCCESS;
    }

    bool HenchPig::onEnterSleeping()
    {
        _nTreeStateDepot = std::move(_nTreeState);
        assert(_anim_m_sleeping != nullptr);
        setAnimation(_anim_m_sleeping);

//        removeTween(_tw_short_jump, true);
//        removeTween(_tw_long_horiz_jump, true);
        uint32_t sleep_duration = dang::Rand::get(500, 1500);;    //!< sleep between 0.5 to 2 secs
//        uint32_t sleep_duration = std::rand() % 1500 + 500;    //!< sleep between 0.5 to 2 secs
        dang::spTwNull nullTw = std::make_shared<dang::TwNull>(sleep_duration, dang::Ease::Linear, 1);
        nullTw->setFinishedCallback(std::bind(&HenchPig::endSleep, this));
        addTween(nullTw);

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
        resetPathVars();
        if (_nTreeState == nullptr)
        {
            _nTreeState = std::move(_nTreeStateDepot);
        }

        _currentState = LOITERING;
        return true;
    }

    void HenchPig::endLoitering()
    {
    }

    bool HenchPig::onEnterRaging()
    {
        std::cout << "enter raging" << std::endl;
        _walkSpeed = _raging_speed;

        // activate the behaviour tree, if not already active
        resetPathVars();
        if (_nTreeState == nullptr)
        {
            _nTreeState = std::move(_nTreeStateDepot);
        }

//        removeTweens(true);
        // rage for 10 sec
        dang::spTwNull nullTw = std::make_shared<dang::TwNull>(10000, dang::Ease::Linear, 1);
        nullTw->setFinishedCallback(std::bind(&HenchPig::endRaging, this));
        addTween(nullTw);

        _currentState = RAGING;

        return true;
    }

    void HenchPig::endRaging()
    {
        std::cout << "end raging" << std::endl;
        _walkSpeed = _loiter_speed;
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

        _gravity = {0,0};
        setVel({0,0});
        removeAnimation();
        removeTweens(true);

        _anim_m_bubbling->reset();
        setAnimation(_anim_m_bubbling);
        _nTreeStateDepot = std::move(_nTreeState);

        _currentState = BUBBLED;
        return true;
    }

    void HenchPig::endBubble()
    {
        resetPathVars();
        _nTreeState = std::move(_nTreeStateDepot);

        _gravity = PigsnKings::_gravity;
        removeAnimation();
        _anim_m_loitering->reset();
        setAnimation(_anim_m_loitering);

        // Pigs are aggressive when debubbled,
        // don't just loiter, piggie!
        prepareChangeState(RAGING);
    }

    bool HenchPig::isBubbled()
    {
        return _currentState == BUBBLED;
    }



    void HenchPig::tellTheKingWeHitHim()
    {
        //
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_spr = shared_from_this();
        e->_payload = static_cast<uint16_t>(dang::SpriteType::PIG_NORMAL);
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
        // remove throwie
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
        e->_spr = shared_from_this();
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void HenchPig::startOutToWaypoint()
    {
        Enemy::startOutToWaypoint();
        removeAnimation();
        setAnimation(_anim_m_loitering);
        if (_vel.x > 0)
        {
            _transform = blit::HORIZONTAL;
        }
        else
        {
            _transform = blit::NONE;

        }
    }

    dang::BTNode::Status HenchPig::NTSleep(dang::spSprite s)
    {
//        std::cout << "NTSleep" << std::endl;
        std::shared_ptr<HenchPig> spr = std::dynamic_pointer_cast<HenchPig>(s);
        return (spr ? spr->sleep() : dang::BTNode::Status::FAILURE);
    }



}