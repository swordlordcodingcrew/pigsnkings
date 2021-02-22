// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "CollisionSprite.hpp"

#include "SpriteFactory.hpp"
#include "Hero.h"
#include "Enemy.h"
#include "Bubble.h"

#include "GSPlay.h"

namespace pnk
{
    spHero SpriteFactory::King(const dang::tmx_spriteobject& so, spImagesheet is)
    {
        spHero ret = std::make_shared<pnk::Hero>(so, is);
        ret->activateState();
        return ret;
    }

    spCollisionSprite SpriteFactory::Hotrect(const dang::tmx_spriteobject& so)
    {
        spCollisionSprite ret = std::make_shared<dang::CollisionSprite>(so, nullptr);
        ret->_visible = false;
        ret->_type_num = SpriteFactory::TN_HOTRECT;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spCollisionSprite SpriteFactory::HotrectPlatform(const dang::tmx_spriteobject& so)
    {
        spCollisionSprite ret = std::make_shared<dang::CollisionSprite>(so, nullptr);
        ret->_visible = false;
        ret->_type_num = SpriteFactory::TN_HOTRECT_PLATFORM;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spEnemy SpriteFactory::NormalPig(const dang::tmx_spriteobject &so, spImagesheet is)
    {
        spEnemy ret = std::make_shared<pnk::Enemy>(so, is);
        ret->_type_num = SpriteFactory::TN_NORMAL_PIG;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->init();

        return ret;
    }

    spBubble SpriteFactory::Bubble(const dang::tmx_spriteobject &so, spImagesheet is, bool to_the_left)
    {
        spBubble ret = std::make_shared<pnk::Bubble>(so, is);
        ret->_type_num = SpriteFactory::TN_BUBBLE;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_to_the_left = to_the_left;
        ret->init();

        return ret;
    }


}
