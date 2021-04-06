// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"

#include "Enemy.h"
#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

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
        _hotrect = {10, 16, 12, 16};

        setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3, 4, 5}, 600, &dang::Ease::Linear, -1)));

        setVel({0,0});
    }

    Enemy::~Enemy()
    {
        std::cout << "enemy destructor" << std::endl;
    }

    void Enemy::update(uint32_t dt)
    {
        _on_ground = false;
    }

    dang::CollisionSpriteLayer::eCollisionResponse Enemy::getCollisionResponse(spSprite other)
    {
/*        if (_bubbled)
        {
            return dang::CollisionSpriteLayer::CR_NONE;
        }

        if (other->_type_num == SpriteFactory::TN_KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }
*/
        return dang::CollisionSpriteLayer::CR_SLIDE;
    }

    void Enemy::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
/*        if (mf.other->_type_num == SpriteFactory::TN_BUBBLE || mf.me->_type_num == SpriteFactory::TN_BUBBLE)
        {
            bubble();
        }
        else if (mf.other->_type_num == SpriteFactory::TN_KING || mf.me->_type_num == SpriteFactory::TN_KING)
        {
            // remove self
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
            e->_spr = shared_from_this();
            pnk::_pnk._dispatcher.queueEvent(std::move(e));

            // poof
            std::unique_ptr<PnkEvent> ep(new PnkEvent(EF_GAME, ETG_NEW_POOF));
            ep->_spr = shared_from_this();
            pnk::_pnk._dispatcher.queueEvent(std::move(ep));
        }
        else if (mf.other->_type_num > SpriteFactory::TN_ENEMIES && mf.other->_type_num < SpriteFactory::TN_ENEMIES_END)
        {
            // do nothing (for now)
        }
        else
        {
            const dang::Vector2F& normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

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
*/    }

/*    void Enemy::bubble()
    {
        _bubbled = true;
        _gravity = {0,0};
        setVel({0,0});
        removeAnimation();
        setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{12, 13}, 400, dang::Ease::Linear, -1)));
    }

    void Enemy::deBubble()
    {
        _bubbled = false;
        _gravity = PigsnKings::_gravity;
//        setVel({-2.0f, -0.0f});
        removeAnimation();
        setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3, 4, 5}, 600, dang::Ease::Linear, -1)));
    }
*/
}