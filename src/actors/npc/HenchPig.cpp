// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <TmxExtruder.hpp>

#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"
#include "src/pigsnkings.hpp"

#include "HenchPig.h"
#include "Enemy.h"

namespace pnk
{

    extern PigsnKings _pnk;

    HenchPig::HenchPig() : pnk::Enemy()
    {
    }

    HenchPig::HenchPig(const dang::tmx_spriteobject* so, spImagesheet is) : pnk::Enemy(so, is)
    {
    }

    void HenchPig::init()
    {
        _hotrect = {10, 16, 12, 16};

        onEnterSleeping();

        setVel({0, 0});
    }

    HenchPig::~HenchPig()
    {
        std::cout << "HenchPig destructor" << std::endl;
    }

    void HenchPig::update(uint32_t dt)
    {
        handlePath();

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

    dang::CollisionSpriteLayer::eCollisionResponse HenchPig::getCollisionResponse(spSprite other)
    {
        if (_currentState == BUBBLED)
        {
            return dang::CollisionSpriteLayer::CR_NONE;
        }

        if (other->_type_num == dang::SpriteType::KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }

        return dang::CollisionSpriteLayer::CR_SLIDE;
    }

    void HenchPig::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        spCollisionSprite sprOther = mf.me.get() == this ? mf.other : mf.me;
        if (sprOther->_type_num == dang::SpriteType::BUBBLE)
        {
            // the bubble will call bubble()
        }
        else if (sprOther->_type_num == dang::SpriteType::KING)
        {
            tellTheKingWeHitHim();

            poofing();
        }
        else if ((sprOther->_type_num > dang::SpriteType::ENEMIES && sprOther->_type_num < dang::SpriteType::ENEMIES_END))
        {
            // do nothing (for now)
        }
        else
        {
            const dang::Vector2F &normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.x != 0)
            {
                _vel.x = 0;
//                _walkSpeed = -_walkSpeed;
//                _transform = _walkSpeed > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;
            }

            if (normal.y > 0)
            {
                _on_ground = true;
                _vel.y = 0;
            }
            else
            {
//                _vel.x = 0;
            }

#ifdef PNK_DEBUG
            if (mf.overlaps)
        {
            std::cout << "overlap, pos(" << _pos.x << ", " << _pos.y << ")" << std::endl;
        }
#endif
        }
    }

    void HenchPig::prepareChangeState(e_state wishedState)
    {
        // TODO We could have some logic here as well, or in the update routine?
        _nextState = wishedState;
    }

    bool HenchPig::onEnterSleeping()
    {
        // TODO check if we are on the air or on the ground. pigs don't sleep mid-air
        if(_anim_m_sleeping != nullptr)
        {
            setAnimation(_anim_m_sleeping);
        }
        else
        {
            std::cerr << "_anim_m_sleeping is not set in HenchPig" << std::endl;
        }
        _currentState = SLEEPING;

        removeTweens(true);

        spTwNull nullTw = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 1));
        nullTw->setFinishedCallback(std::bind(&HenchPig::endSleep, this));
        addTween(nullTw);

        return true;
    }

    bool HenchPig::onEnterHiding()
    {
        // handled by subclasses
        return false;
    }

    bool HenchPig::onEnterLoitering()
    {
        // TODO handle the walking with tweens
        //_walkSpeed = _loiter_speed;
        setAnimation(_anim_m_loitering);
        _transform = _walkSpeed > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;

        _currentState = LOITERING;

        spTwNull nullTw = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 1));
        nullTw->setFinishedCallback(std::bind(&HenchPig::endLoitering, this));
        addTween(nullTw);

        return true;
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

    bool HenchPig::onEnterBubbled()
    {
        // TODO depending on subclass and type of henchpig the pig will let crates or bombs fall to the ground
        removeTweens(true);
        _currentState = BUBBLED;
        return true;
    }

    void HenchPig::bubble()
    {
        _gravity = {0,0};
        setVel({0,0});
        removeAnimation();
        _anim_m_bubbling->reset();
        setAnimation(_anim_m_bubbling);

        prepareChangeState(BUBBLED);
    }

    void HenchPig::deBubble()
    {
        // TODO Pigs are aggressive when debubbled,
        // don't just loiter, piggie!
        _gravity = PigsnKings::_gravity;
        removeAnimation();
        _anim_m_loitering->reset();
        setAnimation(_anim_m_loitering);

        prepareChangeState(LOITERING);
    }

    bool HenchPig::isBubbled()
    {
        return _currentState == BUBBLED;
    }

    void HenchPig::endSleep()
    {
        prepareChangeState(LOITERING);
    }

    void HenchPig::endLoitering()
    {
        prepareChangeState(SLEEPING);
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
}