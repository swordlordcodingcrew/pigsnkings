// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "PigCrate.h"
#include "src/pigsnkings.hpp"
#include "src/pnk_globals.h"
#include "src/PnkEvent.h"

#include <tween/TwAnim.hpp>
#include <tween/TwNull.hpp>
#include <tween/TwSequence.hpp>

#include <iostream>
#include <cassert>

namespace pnk
{
    extern PigsnKings _pnk;

    PigCrate::PigCrate() : pnk::HenchPig()
    {
    }

    PigCrate::PigCrate(const dang::tmx_spriteobject* so, dang::spImagesheet& is) : pnk::HenchPig(so, is)
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


    bool PigCrate::onEnterThrowing()
    {
        assert(_anim_m_throwing != nullptr);
        removeAnimation();
        setAnimation(_anim_m_throwing);
        if (_nTreeState->_payload.count("aaLoSH"))
        {
            _transform = _nTreeState->_payload["aaLoSH"] < 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;
        }
        else if (_nTreeState->_payload.count("LoS"))
        {
            _transform = _nTreeState->_payload["LoS"] < 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;
        }
        setVel({0,0});

        // change the anims from crate-carrying to empty-handed
        _currentState = THROWING;
        _crated = false;
        dang::spTwAnim tmp_anim = _anim_m_loitering;
        _anim_m_loitering = _anim_alt_loitering;
        _anim_alt_loitering = tmp_anim;

        tmp_anim = _anim_m_sleeping;
        _anim_m_sleeping = _anim_alt_sleeping;
        _anim_alt_sleeping = tmp_anim;


        dang::spTwSequence tws = std::make_shared<dang::TwSequence>();
        dang::spTwNull twPrepare = std::make_shared<dang::TwNull>(300, dang::Ease::Linear, 0);
        twPrepare->setFinishedCallback(std::bind(&PigCrate::throwing, this));
        tws->addTween(twPrepare);

        dang::spTwNull twThrown = std::make_shared<dang::TwNull>(300, dang::Ease::Linear, 0);
        twThrown->setFinishedCallback(std::bind(&PigCrate::endThrowing, this));
        tws->addTween(twThrown);
        addTween(tws);

        return true;

    }

    void PigCrate::throwing()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_THROWN_CRATE));
        if (_nTreeState->_payload.count("aaLoSH"))
        {
            e->_to_the_left = _nTreeState->_payload["aaLoSH"] > 0;
            _nTreeState->_payload.erase("aaLoSH");
        }
        else if (_nTreeState->_payload.count("LoS"))
        {
            e->_to_the_left = _nTreeState->_payload["LoS"] > 0;
            _nTreeState->_payload.erase("LoS");
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
        if (_nTreeStateDefault == nullptr)
        {
            prepareChangeState(SLEEPING);
        }
        else
        {
            prepareChangeState(LOITERING);
        }
    }

    void PigCrate::pickupCrate()
    {
        _crated = true;
        dang::spTwAnim tmp_anim = _anim_m_loitering;
        _anim_m_loitering = _anim_alt_loitering;
        _anim_alt_loitering = tmp_anim;

        tmp_anim = _anim_m_sleeping;
        _anim_m_sleeping = _anim_alt_sleeping;
        _anim_alt_sleeping = tmp_anim;

        if (_currentState == LOITERING)
        {
            setAnimation(_anim_m_loitering);
        }
        else if (_currentState == SLEEPING)
        {
            setAnimation(_anim_m_sleeping);
        }
    }

    bool PigCrate::onEnterBubbled()
    {
        // drop crate
        _crated = false;
        dang::spTwAnim tmp_anim = _anim_m_loitering;
        _anim_m_loitering = _anim_alt_loitering;
        _anim_alt_loitering = tmp_anim;

        tmp_anim = _anim_m_sleeping;
        _anim_m_sleeping = _anim_alt_sleeping;
        _anim_alt_sleeping = tmp_anim;

        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_DROP_CRATE));
        e->_to_the_left = this->_transform != blit::SpriteTransform::HORIZONTAL;
        e->_pos = this->getPos();
        e->_pos.y -= 10; // piggie holds the box on ground + 10 (yeah, small piggie)
        _pnk._dispatcher.queueEvent(std::move(e));

        HenchPig::onEnterBubbled();

        return true;

    }



    dang::BTNode::Status PigCrate::NTPickUpCrate(dang::spSprite s)
    {
        std::shared_ptr<PigCrate> spr = std::dynamic_pointer_cast<PigCrate>(s);

        if (spr->_crated)
        {
            return dang::BTNode::Status::FAILURE;
        }
        else
        {
            spr->pickupCrate();
            return dang::BTNode::Status::SUCCESS;
        }

    }

    dang::BTNode::Status PigCrate::NTThrowCrate(dang::spSprite s)
    {
        std::shared_ptr<PigCrate> spr = std::dynamic_pointer_cast<PigCrate>(s);

        if (spr->_crated && spr->_currentState != THROWING)
        {
            spr->prepareChangeState(THROWING);
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr->_currentState == THROWING && spr->_nextState == THROWING)
        {
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr->_currentState == THROWING && spr->_nextState != THROWING)
        {
            return dang::BTNode::Status::SUCCESS;
        }

        return dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status PigCrate::NTWithCrate(dang::spSprite s)
    {
        std::shared_ptr<PigCrate> spr = std::dynamic_pointer_cast<PigCrate>(s);
        return spr->_crated ? dang::BTNode::Status::SUCCESS : dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status PigCrate::NTDistanceOK(dang::spSprite s)
    {

        std::shared_ptr<PigCrate> spr = std::dynamic_pointer_cast<PigCrate>(s);

        if (spr->_nTreeState->_payload.count("aaLoSH"))
        {
            float dist = spr->_nTreeState->_payload["aaLoSH"];
            dist = std::abs(dist);
            if (dist < 110)
            {
                return dang::BTNode::Status::SUCCESS;
            }
        }
        else if (spr->_nTreeState->_payload.count("LoS"))
        {
            float dist = spr->_nTreeState->_payload["LoS"];
            dist = std::abs(dist);
            if (dist < 110)
            {
                return dang::BTNode::Status::SUCCESS;
            }
        }

        return dang::BTNode::Status::FAILURE;
    }


/*    dang::BTNodeStatus PigCrate::BTHideInCrate(dang::spSprite s)
    {
        return dang::BTNodeStatus::FAILURE;
    }
*/


}