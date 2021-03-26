// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"

#include "PigBomb.h"
#include "src/pnk_globals.h"
#include "src/Enemy.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{

    extern PigsnKings _pnk;

    PigBomb::PigBomb() : pnk::HenchPig()
    {
    }

    PigBomb::PigBomb(const dang::tmx_spriteobject &so, spImagesheet is) : pnk::HenchPig(so, is)
    {
    }

    void PigBomb::init()
    {
        this->HenchPig::init();
    }

    PigBomb::~PigBomb()
    {
        std::cout << "PigBomb destructor" << std::endl;
    }

    void PigBomb::update(uint32_t dt)
    {
        this->HenchPig::update(dt);
    }

    dang::CollisionSpriteLayer::eCollisionResponse PigBomb::getCollisionResponse(spSprite other)
    {
        return this->HenchPig::getCollisionResponse(other);
    }

    void PigBomb::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        this->HenchPig::collide(mf);
    }

    bool PigBomb::onEnterLoitering()
    {
        // TODO handle the walking with tweens
        //_walkSpeed = _loiter_speed;
        setAnimation(_anim_m_loitering);
        _transform = _walkSpeed > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;

        _currentState = LOITERING;

        spTwNull nullTw = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 0));
        nullTw->setFinishedCallback(std::bind(&PigBomb::endLoitering, this));
        addTween(nullTw);

        return true;
    }

    bool PigBomb::onEnterThrowing()
    {
        // TODO handle the spawning of a new crate
        //_walkSpeed = _loiter_speed;
        setAnimation(_anim_m_throwing);
        _transform = _walkSpeed > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;

        _currentState = THROWING;

        spTwNull twPrepare = std::make_shared<dang::TwNull>(dang::TwNull(300, dang::Ease::Linear, 0));
        twPrepare->setFinishedCallback(std::bind(&PigBomb::throwing, this));
        addTween(twPrepare);

        spTwNull twThrown = std::make_shared<dang::TwNull>(dang::TwNull(700, dang::Ease::Linear, 0));
        twThrown->setFinishedCallback(std::bind(&PigBomb::endThrowing, this));
        addTween(twThrown);

        return true;
    }

    void PigBomb::endLoitering()
    {
        prepareChangeState(THROWING);
    }

    void PigBomb::throwing()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_THROWN_BOMB));
        e->_to_the_left = this->_transform != blit::SpriteTransform::HORIZONTAL;
        e->_pos = this->getPos();
        e->_pos.y -= 10; // piggie holds the box on ground + 10 (yeah, small piggie)
        _pnk._dispatcher.queueEvent(std::move(e));
    }

    void PigBomb::endThrowing()
    {
        _anim_m_throwing->reset();
        prepareChangeState(SLEEPING);
    }
}