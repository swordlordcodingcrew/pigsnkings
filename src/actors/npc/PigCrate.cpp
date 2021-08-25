// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "PigCrate.h"
#include "src/pigsnkings.hpp"
#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
//#include "src/GSPlay.h"
//#include "src/SpriteFactory.hpp"
//#include "Enemy.h"

#include <tween/TwAnim.hpp>
#include <tween/TwNull.hpp>
#include <tween/TwSequence.hpp>
#include <Imagesheet.hpp>
#include <TmxExtruder.hpp>

#include <iostream>

namespace pnk
{
    extern PigsnKings _pnk;

    PigCrate::PigCrate() : pnk::HenchPig()
    {
    }

    PigCrate::PigCrate(const dang::tmx_spriteobject* so, dang::spImagesheet is) : pnk::HenchPig(so, is)
    {
    }

    void PigCrate::init()
    {
        this->HenchPig::init();
    }

    PigCrate::~PigCrate()
    {
#ifdef PNK_DEBUG
        std::cout << "PigCrate destructor" << std::endl;
#endif
    }

    void PigCrate::update(uint32_t dt)
    {
        this->HenchPig::update(dt);
    }

    dang::CollisionSpriteLayer::eCollisionResponse PigCrate::getCollisionResponse(const dang::spCollisionSprite& other)
    {
        return this->HenchPig::getCollisionResponse(other);
    }

    void PigCrate::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        this->HenchPig::collide(mf);
    }

/*    bool PigCrate::onEnterLoitering()
    {
        return HenchPig::onEnterLoitering();
    }
*/

    bool PigCrate::onEnterThrowing()
    {
        assert(_anim_m_throwing != nullptr);
        removeAnimation();
        setAnimation(_anim_m_throwing);
        _transform = _walkSpeed > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;

        _currentState = THROWING;
//        _crated = false;

        dang::spTwSequence tws = std::make_shared<dang::TwSequence>();
        dang::spTwNull twPrepare = std::make_shared<dang::TwNull>(100, dang::Ease::Linear, 0);
        twPrepare->setFinishedCallback(std::bind(&PigCrate::throwing, this));
        tws->addTween(twPrepare);

        dang::spTwNull twThrown = std::make_shared<dang::TwNull>(400, dang::Ease::Linear, 0);
        twThrown->setFinishedCallback(std::bind(&PigCrate::endThrowing, this));
        tws->addTween(twThrown);
        addTween(tws);

        return true;

/*        // TODO handle the spawning of a new crate
        //_walkSpeed = _loiter_speed;
        setAnimation(_anim_m_throwing);
        _transform = _walkSpeed > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;

        _currentState = THROWING;

        dang::spTwNull twPrepare = std::make_shared<dang::TwNull>(300, dang::Ease::Linear, 0);
        twPrepare->setFinishedCallback(std::bind(&PigCrate::throwing, this));
        addTween(twPrepare);

        dang::spTwNull twThrown = std::make_shared<dang::TwNull>(700, dang::Ease::Linear, 0);
        twThrown->setFinishedCallback(std::bind(&PigCrate::endThrowing, this));
        addTween(twThrown);

        return true;
        */

        return false;
    }

    void PigCrate::throwing()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_THROWN_CRATE));
        if (_nTreeState->_payload.count("aaLoSH"))
        {
            e->_to_the_left = _nTreeState->_payload["aaLoSH"] > 0;
            _nTreeState->_payload.erase("aaLoSH");
        }
        else
        {
            e->_to_the_left = this->_transform != blit::SpriteTransform::HORIZONTAL;
        }

        e->_pos = this->getPos();
        e->_pos.y -= 10; // piggie holds the box on ground + 10 (yeah, small piggie)
        _pnk._dispatcher.queueEvent(std::move(e));
    }

    void PigCrate::endThrowing()
    {
//        _anim_m_throwing->reset();
        prepareChangeState(LOITERING);
    }

    dang::BTNodeStatus PigCrate::BTPickUpCrate(dang::spSprite s)
    {
//        std::cout << "picking up crate: " << s->getPos().x << std::endl;
        return dang::BTNodeStatus::FAILURE;
    }

    dang::BTNodeStatus PigCrate::BTThrowCrate(dang::spSprite s)
    {
//        std::cout << "throwing crate: " << s->getPos().x << std::endl;
        return dang::BTNodeStatus::FAILURE;
    }

    dang::BTNodeStatus PigCrate::BTHideInCrate(dang::spSprite s)
    {
//        std::cout << "hiding in crate: " << s->getPos().x << std::endl;
        return dang::BTNodeStatus::FAILURE;
    }

    dang::BTNode::Status PigCrate::NTThrowCrate(dang::spSprite s)
    {
        std::shared_ptr<PigCrate> spr = std::dynamic_pointer_cast<PigCrate>(s);

        if (!spr->_crated)
        {
            return dang::BTNode::Status::FAILURE;
        }

        if (spr->_currentState != THROWING)
        {
            spr->prepareChangeState(THROWING);
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr->_currentState == THROWING && spr->_nextState != THROWING)
        {
            return dang::BTNode::Status::SUCCESS;
        }
        else
        {
            return dang::BTNode::Status::RUNNING;
        }

    }

}