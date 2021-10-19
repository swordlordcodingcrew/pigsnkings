// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game


#include "PigBomb.h"
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

    PigBomb::PigBomb() : pnk::HenchPig()
    {
    }

    PigBomb::PigBomb(const dang::tmx_spriteobject* so, dang::spImagesheet& is) : pnk::HenchPig(so, is)
    {
    }

    void PigBomb::init()
    {
        this->HenchPig::init();
    }

    PigBomb::~PigBomb()
    {
#ifdef PNK_DEBUG
        std::cout << "PigBomb destructor" << std::endl;
#endif
    }

    void PigBomb::update(uint32_t dt)
    {
        this->HenchPig::update(dt);
    }

    dang::CollisionSpriteLayer::eCollisionResponse PigBomb::getCollisionResponse(const dang::spCollisionSprite& other)
    {
        return this->HenchPig::getCollisionResponse(other);
    }

    void PigBomb::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        this->HenchPig::collide(mf);
    }

    bool PigBomb::onEnterThrowing()
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

        // change the anims from bomb-carrying to empty-handed
        _currentState = THROWING;
        _with_bomb = false;
        dang::spTwAnim tmp_anim = _anim_m_loitering;
        _anim_m_loitering = _anim_alt_loitering;
        _anim_alt_loitering = tmp_anim;

        tmp_anim = _anim_m_sleeping;
        _anim_m_sleeping = _anim_alt_sleeping;
        _anim_alt_sleeping = tmp_anim;

        dang::spTwSequence tws = std::make_shared<dang::TwSequence>();
        dang::spTwNull twPrepare = std::make_shared<dang::TwNull>(300, dang::Ease::Linear, 0);
        twPrepare->setFinishedCallback(std::bind(&PigBomb::throwing, this));
        tws->addTween(twPrepare);

        dang::spTwNull twThrown = std::make_shared<dang::TwNull>(300, dang::Ease::Linear, 0);
        twThrown->setFinishedCallback(std::bind(&PigBomb::endThrowing, this));
        tws->addTween(twThrown);
        addTween(tws);

        return true;
    }


    void PigBomb::throwing()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_THROWN_BOMB));
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

    void PigBomb::endThrowing()
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

    void PigBomb::pickupBomb()
    {
        _with_bomb = true;
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

    bool PigBomb::onEnterBubbled()
    {
        if (_with_bomb)
        {
            // drop bomb
            _with_bomb = false;
            dang::spTwAnim tmp_anim = _anim_m_loitering;
            _anim_m_loitering = _anim_alt_loitering;
            _anim_alt_loitering = tmp_anim;

            tmp_anim = _anim_m_sleeping;
            _anim_m_sleeping = _anim_alt_sleeping;
            _anim_alt_sleeping = tmp_anim;

            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_DROP_BOMB));
            e->_to_the_left = this->_transform != blit::SpriteTransform::HORIZONTAL;
            e->_pos = this->getPos();
            e->_pos.y -= 10; // piggie holds the bomb on ground + 10 (yeah, small piggie)
            _pnk._dispatcher.queueEvent(std::move(e));

        }

        return HenchPig::onEnterBubbled();

    }


    dang::BTNode::Status PigBomb::NTPickUpBomb(dang::spSprite s)
    {
        std::shared_ptr<PigBomb> spr = std::dynamic_pointer_cast<PigBomb>(s);

        if (spr->_with_bomb)
        {
            return dang::BTNode::Status::FAILURE;
        }
        else
        {
            spr->pickupBomb();
            return dang::BTNode::Status::SUCCESS;
        }

    }

    dang::BTNode::Status PigBomb::NTThrowBomb(dang::spSprite s)
    {
        std::shared_ptr<PigBomb> spr = std::dynamic_pointer_cast<PigBomb>(s);

        if (spr->_with_bomb && spr->_currentState != THROWING)
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

    dang::BTNode::Status PigBomb::NTWithBomb(dang::spSprite s)
    {
        std::shared_ptr<PigBomb> spr = std::dynamic_pointer_cast<PigBomb>(s);
        return spr->_with_bomb ? dang::BTNode::Status::SUCCESS : dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status PigBomb::NTDistanceOK(dang::spSprite s)
    {
        std::shared_ptr<PigBomb> spr = std::dynamic_pointer_cast<PigBomb>(s);

        if (spr->_nTreeState->_payload.count("aaLoSH"))
        {
            float dist = spr->_nTreeState->_payload["aaLoSH"];
            dist = std::abs(dist);
            if (dist < 150)
            {
                return dang::BTNode::Status::SUCCESS;
            }
        }
        else if (spr->_nTreeState->_payload.count("LoS"))
        {
            float dist = spr->_nTreeState->_payload["LoS"];
            dist = std::abs(dist);
            if (dist < 150)
            {
                return dang::BTNode::Status::SUCCESS;
            }
        }

        return dang::BTNode::Status::FAILURE;
    }



}