// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"

#include "PigCrate.h"
#include "src/pnk_globals.h"
#include "Enemy.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{
    extern PigsnKings _pnk;

    PigCrate::PigCrate() : pnk::HenchPig()
    {
    }

    PigCrate::PigCrate(const dang::tmx_spriteobject* so, spImagesheet is) : pnk::HenchPig(so, is)
    {
    }

    void PigCrate::init()
    {
        this->HenchPig::init();
    }

    PigCrate::~PigCrate()
    {
        std::cout << "PigCrate destructor" << std::endl;
    }

    void PigCrate::update(uint32_t dt)
    {
        this->HenchPig::update(dt);
    }

    dang::CollisionSpriteLayer::eCollisionResponse PigCrate::getCollisionResponse(spSprite other)
    {
        return this->HenchPig::getCollisionResponse(other);
    }

    void PigCrate::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        this->HenchPig::collide(mf);
    }

    bool PigCrate::onEnterLoitering()
    {
        // TODO handle the walking with tweens
        //_walkSpeed = _loiter_speed;
        setAnimation(_anim_m_loitering);
        _transform = _walkSpeed > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;

        _currentState = LOITERING;

        spTwNull nullTw = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 0));
        nullTw->setFinishedCallback(std::bind(&PigCrate::endLoitering, this));
        addTween(nullTw);

        return true;
    }

    bool PigCrate::onEnterThrowing()
    {
        // TODO handle the spawning of a new crate
        //_walkSpeed = _loiter_speed;
        setAnimation(_anim_m_throwing);
        _transform = _walkSpeed > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;

        _currentState = THROWING;

        spTwNull twPrepare = std::make_shared<dang::TwNull>(dang::TwNull(300, dang::Ease::Linear, 0));
        twPrepare->setFinishedCallback(std::bind(&PigCrate::throwing, this));
        addTween(twPrepare);

        spTwNull twThrown = std::make_shared<dang::TwNull>(dang::TwNull(700, dang::Ease::Linear, 0));
        twThrown->setFinishedCallback(std::bind(&PigCrate::endThrowing, this));
        addTween(twThrown);

        return true;
    }

    void PigCrate::endLoitering()
    {
        prepareChangeState(THROWING);
    }

    void PigCrate::throwing()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_THROWN_CRATE));
        e->_to_the_left = this->_transform != blit::SpriteTransform::HORIZONTAL;
        e->_pos = this->getPos();
        e->_pos.y -= 10; // piggie holds the box on ground + 10 (yeah, small piggie)
        _pnk._dispatcher.queueEvent(std::move(e));
    }

    void PigCrate::endThrowing()
    {
        _anim_m_throwing->reset();
        prepareChangeState(SLEEPING);
    }

    dang::BTNodeStatus PigCrate::BTPickUpCrate(std::shared_ptr<Sprite> s)
    {
//        std::cout << "picking up crate: " << s->getPos().x << std::endl;
        return dang::BTNodeStatus::FAILURE;
    }

    dang::BTNodeStatus PigCrate::BTThrowCrate(std::shared_ptr<Sprite> s)
    {
//        std::cout << "throwing crate: " << s->getPos().x << std::endl;
        return dang::BTNodeStatus::FAILURE;
    }

    dang::BTNodeStatus PigCrate::BTHideInCrate(std::shared_ptr<Sprite> s)
    {
//        std::cout << "hiding in crate: " << s->getPos().x << std::endl;
        return dang::BTNodeStatus::FAILURE;
    }
}