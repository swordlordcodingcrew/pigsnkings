// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <cassert>
#include <iostream>
#include <src/actors/npc/PigBox.h>
#include "CollisionSprite.hpp"
#include "TmxExtruder.hpp"
#include "Imagesheet.hpp"
#include "tween/TwAnim.hpp"
#include "tween/Ease.hpp"

#include "SpriteFactory.hpp"
#include "Hero.h"
#include "Enemy.h"
#include "Bubble.h"

#include "GSPlay.h"

namespace pnk
{
    spHero SpriteFactory::King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is)
    {
        spHero ret = std::make_shared<pnk::Hero>(so, is);
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_type_num = SpriteFactory::TN_KING;

        // wait animation
//      std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3, 4, 5, 6, 0}, 600, dang::Ease::Linear , -1, false, 2000));
        ret->_anim_m_wait = txtr.getAnimation(is->getName(), "wait");
//        std::shared_ptr<dang::TwAnim> a = txtr.getAnimation(is->getName(), "wait");
        assert(ret->_anim_m_wait != nullptr);
        ret->_anim_m_wait->delay(2000);

        // walk animation
//        _anim_m_walk = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{10, 11, 12, 13, 14, 15, 16, 17}, 600, dang::Ease::Linear, -1));
        ret->_anim_m_walk = txtr.getAnimation(is->getName(), "walk");
        assert(ret->_anim_m_walk != nullptr);

        // jump animation
//        _anim_m_jump = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{9, 7, 8}, 100, dang::Ease::Linear));
        ret->_anim_m_jump = txtr.getAnimation(is->getName(), "jump");
        assert(ret->_anim_m_jump != nullptr);

        // on air (= not touching the ground) 'animation'
//      _anim_m_on_air = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{7}, 600, dang::Ease::Linear))
        ret->_anim_m_on_air = txtr.getAnimation(is->getName(), "on_air");
        assert(ret->_anim_m_on_air != nullptr);

        // bubble animation
//        _anim_bubble = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{18, 19, 20, 19, 18}, 300, dang::Ease::OutQuad, 1));
        ret->_anim_bubble = txtr.getAnimation(is->getName(), "bubble");
        assert(ret->_anim_bubble != nullptr);
        ret->_anim_bubble->loops(1);
        ret->_anim_bubble->easeFunction(&dang::Ease::OutQuad);

        // entering game animation
//        _anim_s_enter = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 25}, 200, dang::Ease::Linear, -1));
        ret->_anim_s_enter = txtr.getAnimation(is->getName(), "blink");
        assert(ret->_anim_bubble != nullptr);


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

    spEnemy SpriteFactory::NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject &so, spImagesheet is)
    {
        spEnemy ret = std::make_shared<pnk::Enemy>(so, is);
        ret->_type_num = SpriteFactory::TN_PIG_NORMAL;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->init();

        return ret;
    }

    spEnemy SpriteFactory::PigBox(dang::TmxExtruder& txtr, const dang::tmx_spriteobject &so, spImagesheet is)
    {
        spEnemy ret = std::make_shared<pnk::PigBox>(so, is);
        ret->_type_num = SpriteFactory::TN_PIG_BOX;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->init();

        return ret;
    }

    spEnemy SpriteFactory::PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject &so, spImagesheet is)
    {
        spEnemy ret = std::make_shared<pnk::PigBox>(so, is);
        ret->_type_num = SpriteFactory::TN_PIG_BOMB;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->init();

        return ret;
    }

    spReward SpriteFactory::Reward(dang::TmxExtruder &txtr, const dang::tmx_spriteobject &so, spImagesheet is)
    {
        spReward ret = std::make_shared<pnk::Reward>(so, is);

        if(so.type == SpriteFactory::T_COIN_SILVER)
        {
            ret->_type_num = SpriteFactory::TN_COIN_SILVER;
        }
        else if(so.type == SpriteFactory::T_COIN_GOLD)
        {
            ret->_type_num = SpriteFactory::TN_COIN_GOLD;
        }
        else if(so.type == SpriteFactory::T_GEM_BLUE)
        {
            ret->_type_num = SpriteFactory::TN_GEM_BLUE;
        }
        else if(so.type == SpriteFactory::T_GEM_GREEN)
        {
            ret->_type_num = SpriteFactory::TN_GEM_GREEN;
        }
        else if(so.type == SpriteFactory::T_GEM_RED)
        {
            ret->_type_num = SpriteFactory::TN_GEM_RED;
        }
        else if(so.type == SpriteFactory::T_POTION_BLUE)
        {
            ret->_type_num = SpriteFactory::TN_POTION_BLUE;
        }
        else if(so.type == SpriteFactory::T_POTION_RED)
        {
            ret->_type_num = SpriteFactory::TN_POTION_RED;
        }
        else if(so.type == SpriteFactory::T_POTION_GREEN)
        {
            ret->_type_num = SpriteFactory::TN_POTION_GREEN;
        }
        else
        {
            std::cout << "current TN Type not implemented in SpriteFactory Reward: " << so.type << std::endl;
            ret->_type_num = SpriteFactory::TN_COIN_SILVER;
        }

        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->init();

        return ret;
    }

    spBubble SpriteFactory::Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject &so, spImagesheet is, bool to_the_left)
    {
        spBubble ret = std::make_shared<pnk::Bubble>(so, is);
        ret->_type_num = SpriteFactory::TN_BUBBLE;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_to_the_left = to_the_left;

        // bubble grows
        ret->_anim_blow = txtr.getAnimation(is->getName(), "bubble_blow");
        assert(ret->_anim_blow != nullptr);
        ret->_anim_blow->easeFunction(&dang::Ease::OutQuad);
        ret->_anim_blow->loops(1);

        // bubble bobbles
        ret->_anim_bobble = txtr.getAnimation(is->getName(), "bubble_bobble");
        assert(ret->_anim_bobble != nullptr);
        ret->_anim_bobble->loops(3);

        // bubble poofs
        ret->_anim_poof = txtr.getAnimation(is->getName(), "bubble_poof");
        assert(ret->_anim_poof != nullptr);
        ret->_anim_poof->loops(1);

        // bobbling with catched enemy
        ret->_anim_catched = std::make_shared<dang::TwAnim>(*(ret->_anim_bobble));
        ret->_anim_catched->loops(12);

        ret->init();

        return ret;
    }


}
