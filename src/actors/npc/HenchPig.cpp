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

        setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{5, 6, 7, 8, 9, 10, 11, 12, 13}, 600, &dang::Ease::Linear, -1)));

        setVel({0,0});
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
        if (_bubbled)
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
//            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
//            e->_spr = shared_from_this();
//            pnk::_pnk._dispatcher.queueEvent(std::move(e));
        }
        else if (mf.other->_type_num > SpriteFactory::TN_ENEMIES_START && mf.other->_type_num < SpriteFactory::TN_ENEMIES_END)
        {
            // do nothing (for now)
        }
        else
        {
            const dang::Vector2F& normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.x != 0)
            {
                _walk = -_walk;
                _transform = _walk > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;
            }

            if (normal.y > 0)
            {
                _on_ground = true;
                _vel.y = 0;
                // TODO fix this
                // this may be an interesting thought, but is simply wrong
                //_vel.x = _walk;
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

    void HenchPig::TryToChangeState(uint8_t wishedState)
    {

    }


/*
    void Enemy::setWalk(float w_vel)
    {
        _walk = w_vel;
        _transform = _walk > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;
    }
*/
}