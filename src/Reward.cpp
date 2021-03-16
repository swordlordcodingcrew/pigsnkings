// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>

#include "TmxExtruder.hpp"
#include "pigsnkings.hpp"

#include "Reward.h"
#include "GSPlay.h"
#include "SpriteFactory.hpp"

namespace pnk
{
    extern PigsnKings _pnk;

    Reward::Reward() : dang::CollisionSprite()
    {
        _gravity = PigsnKings::_gravity;
    }

    Reward::Reward(const dang::tmx_spriteobject &so, spImagesheet is) : dang::CollisionSprite(so, is)
    {
        _gravity = PigsnKings::_gravity;
    }

    void Reward::init()
    {
        // todo: fix hotrect
        _hotrect = {10, 16, 12, 16};

        // todo: set animation depending on type
        setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3}, 600, &dang::Ease::Linear, -1)));

        setVel({0,0});

    }

    Reward::~Reward()
    {
        std::cout << "reward destructor" << std::endl;
    }

    void Reward::update(uint32_t dt)
    {
    }

    dang::CollisionSpriteLayer::eCollisionResponse Reward::getCollisionResponse(spSprite other)
    {
        if (other->_type_num == SpriteFactory::TN_KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }

        return dang::CollisionSpriteLayer::CR_NONE;
    }

    void Reward::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.other->_type_num == SpriteFactory::TN_KING || mf.me->_type_num == SpriteFactory::TN_KING)
        {
//            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
//            e->_spr = shared_from_this();
//            pnk::_pnk._dispatcher.queueEvent(std::move(e));
        }
    }
}