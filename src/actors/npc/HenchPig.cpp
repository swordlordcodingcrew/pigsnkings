// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"

#include "HenchPig.h"
#include "src/pnk_globals.h"
#include "src/Enemy.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{

    extern PigsnKings _pnk;

    HenchPig::HenchPig() : pnk::Enemy()
    {
    }

    HenchPig::HenchPig(const dang::tmx_spriteobject &so, spImagesheet is) : pnk::Enemy(so, is)
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
        _on_ground = false;
    }

    dang::CollisionSpriteLayer::eCollisionResponse HenchPig::getCollisionResponse(spSprite other)
    {
        if (currentState == BUBBLED)
        {
            return dang::CollisionSpriteLayer::CR_NONE;
        }

        if (other->_type_num == SpriteFactory::TN_KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }

        return dang::CollisionSpriteLayer::CR_SLIDE;
    }

    void HenchPig::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.other->_type_num == SpriteFactory::TN_BUBBLE || mf.me->_type_num == SpriteFactory::TN_BUBBLE)
        {
            bubble();
        }
        else if (mf.other->_type_num == SpriteFactory::TN_KING || mf.me->_type_num == SpriteFactory::TN_KING)
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
            e->_spr = shared_from_this();
            pnk::_pnk._dispatcher.queueEvent(std::move(e));
        }
        else if (mf.other->_type_num > SpriteFactory::TN_ENEMIES_START &&
                 mf.other->_type_num < SpriteFactory::TN_ENEMIES_END)
        {
            // do nothing (for now)
        }
        else
        {
            const dang::Vector2F &normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.x != 0)
            {
                _walkSpeed = -_walkSpeed;
                _transform = _walkSpeed > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;
            }

            if (normal.y > 0)
            {
                _on_ground = true;
                _vel.y = 0;
                // TODO fix this
                // this may be an interesting thought, but is simply wrong
                //_vel.x = _walkSpeed;
            }
            else
            {
                _vel.x = 0;
            }

#ifdef PNK_DEBUG
            if (mf.overlaps)
        {
            std::cout << "overlap, pos(" << _pos.x << ", " << _pos.y << ")" << std::endl;
        }
#endif
        }
    }

    bool HenchPig::changeStateTo(e_state wishedState)
    {
        bool b = false;

        switch (wishedState)
        {
            case SLEEPING:
                b = onEnterSleeping();
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
                b = onEnterBubbled();
                break;
        }

        // actually we could do some logic here and enter a specific state instead?
        // like pig wanted to enter picking up, could not because, let it enter sleep instead
        return b;
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
        currentState = SLEEPING;

        removeTweens(true);

        spTwNull nullTw = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 0));
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

        currentState = LOITERING;

        spTwNull nullTw = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 0));
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
        currentState = BUBBLED;
        return true;
    }

    void HenchPig::bubble()
    {
        this->Enemy::bubble();

        changeStateTo(BUBBLED);
    }

    void HenchPig::deBubble()
    {
        this->Enemy::deBubble();

        // TODO Pigs are aggressive when debubbled,
        // don't just loiter, piggie!
        changeStateTo(LOITERING);
    }

    void HenchPig::endSleep()
    {
        changeStateTo(LOITERING);
    }

    void HenchPig::endLoitering()
    {
        changeStateTo(SLEEPING);
    }
}