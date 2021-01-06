// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>

#include "TmxExtruder.hpp"
#include "pigsnkings.hpp"

#include "Enemy.h"
#include "pnk_globals.h"
#include "PnkEvent.h"

namespace pnk
{

    extern PigsnKings _pnk;

    Enemy::Enemy() : dang::CollisionSprite()
    {
        _gravity = PigsnKings::_gravity;
    }

    Enemy::Enemy(const dang::tmx_spriteobject &so, spImagesheet is) : dang::CollisionSprite(so, is)
    {
        _gravity = PigsnKings::_gravity;
    }

    void Enemy::init()
    {
        setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        _type_num = TN_ENEMY1;

        _hotrect = {10, 16, 12, 16};

        setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{32, 33, 34, 35, 36, 37}, 600, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), -1)));

        setVel({-2.0f, -0.0f});

    }

    Enemy::~Enemy()
    {
        std::cout << "enemy destructor" << std::endl;
    }

    void Enemy::update(uint32_t dt)
    {

    }

    dang::CollisionSpriteLayer::eCollisionResponse Enemy::getCollisionResponse(spSprite other)
    {
        if (_bubbled)
        {
            return dang::CollisionSpriteLayer::CR_NONE;
        }

        if (other->_type_num == TN_HERO)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }

        return dang::CollisionSpriteLayer::CR_SLIDE;
    }

    void Enemy::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.other->_type_num == TN_BUBBLE || mf.me->_type_num == TN_BUBBLE)
        {
            _bubbled = true;
            _gravity = {0,0};
            setVel({0,0});
            removeAnimation();
            setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{30, 31}, 400, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), -1)));
        }
        else if (mf.other->_type_num == TN_HERO || mf.me->_type_num == TN_HERO)
        {
//            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
//            e->_spr = shared_from_this();
//            pnk::_pnk._dispatcher.queueEvent(std::move(e));
        }
        else
        {
            const dang::Vector2F& normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.x != 0)
            {
                _vel.x = -_vel.x;
                _transform = _vel.x > 0 ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE;
            }
            else if (normal.y > 0)
            {
                _vel.y = 0;
            }

#ifdef PNK_DEBUG
            if (mf.overlaps)
        {
            std::cout << "overlap, pos(" << _pos.x << ", " << _pos.y << ")" << std::endl;
        }
#endif
        }
    }

}