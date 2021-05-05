// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <cassert>
#include <iostream>
#include <src/actors/npc/PigCrate.h>
#include <src/actors/npc/PigBomb.h>
#include <src/actors/throwies/Throwies.h>
#include <src/actors/throwies/Craties.h>
#include <src/actors/throwies/Bombies.h>
#include <src/actors/others/Moodies.h>
#include "CollisionSprite.hpp"
#include "TmxExtruder.hpp"
#include "Imagesheet.hpp"
#include "tween/TwAnim.hpp"
#include "tween/Ease.hpp"

#include "SpriteFactory.hpp"
#include "src/actors/hero/Hero.h"
#include "src/actors/npc/Enemy.h"
#include "src/actors/throwies/Bubble.h"
#include "src/actors/others/RoomTrigger.h"
#include "PnkEvent.h"

#include "GSPlay.h"

namespace pnk
{
    spHero SpriteFactory::King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        assert(is != nullptr);
        spHero ret = std::make_shared<pnk::Hero>(so, is);
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_type_num = SpriteFactory::TN_KING;

        // wait animation
        ret->_anim_m_wait = txtr.getAnimation(is->getName(), "wait");
        assert(ret->_anim_m_wait != nullptr);
        ret->_anim_m_wait->delay(2000);

        // walk animation
        ret->_anim_m_walk = txtr.getAnimation(is->getName(), "walk");
        assert(ret->_anim_m_walk != nullptr);

        // jump animation
        ret->_anim_m_jump = txtr.getAnimation(is->getName(), "jump");
        assert(ret->_anim_m_jump != nullptr);

        // on air (= not touching the ground) 'animation'
        ret->_anim_m_on_air = txtr.getAnimation(is->getName(), "on_air");
        assert(ret->_anim_m_on_air != nullptr);

        // bubble animation
        ret->_anim_bubble = txtr.getAnimation(is->getName(), "bubble");
        assert(ret->_anim_bubble != nullptr);
        ret->_anim_bubble->loops(1);
        ret->_anim_bubble->easeFunction(&dang::Ease::OutQuad);

        // entering & hit game animation
        ret->_anim_s_blink = txtr.getAnimation(is->getName(), "blink");
        assert(ret->_anim_s_blink != nullptr);

        // life lost animation
        ret->_anim_s_life_lost = txtr.getAnimation(is->getName(), "life_lost");
        assert(ret->_anim_s_life_lost != nullptr);
        ret->_anim_s_life_lost->loops(1);

        ret->activateState();
        return ret;
    }

    spCollisionSprite SpriteFactory::Hotrect(const dang::tmx_spriteobject* so)
    {
        spCollisionSprite ret = std::make_shared<dang::CollisionSprite>(so, nullptr);
        ret->_visible = false;
        ret->_type_num = SpriteFactory::TN_HOTRECT;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spCollisionSprite SpriteFactory::HotrectPlatform(const dang::tmx_spriteobject* so)
    {
        spCollisionSprite ret = std::make_shared<dang::CollisionSprite>(so, nullptr);
        ret->_visible = false;
        ret->_type_num = SpriteFactory::TN_HOTRECT_PLATFORM;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spCollisionSprite SpriteFactory::RoomTrigger(const dang::tmx_spriteobject* so)
    {
        spCollisionSprite ret = std::make_shared<pnk::RoomTrigger>(so, false);
        ret->_visible = false;
        ret->_type_num = SpriteFactory::TN_ROOM_TRIGGER;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spCollisionSprite SpriteFactory::WarpRoomTrigger(const dang::tmx_spriteobject* so)
    {
        spCollisionSprite ret = std::make_shared<pnk::RoomTrigger>(so, true);
        ret->_visible = false;
        ret->_type_num = SpriteFactory::TN_WARP_ROOM_TRIGGER;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spHenchPig SpriteFactory::NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spHenchPig ret = std::make_shared<pnk::HenchPig>(so, is);
        ret->_type_num = SpriteFactory::TN_PIG_NORMAL;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_loitering = txtr.getAnimation(is->getName(), "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_bubbling = txtr.getAnimation(is->getName(), "bubbling");
        assert(ret->_anim_m_bubbling != nullptr);

        ret->init();

        return ret;
    }

    // it is a wooden crate.. for making buses out of them
    spHenchPig SpriteFactory::PigCrate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spHenchPig ret = std::make_shared<pnk::PigCrate>(so, is);
        ret->_type_num = SpriteFactory::TN_PIG_BOX;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_loitering = txtr.getAnimation(is->getName(), "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_bubbling = txtr.getAnimation(is->getName(), "bubbling");
        assert(ret->_anim_m_bubbling != nullptr);
        ret->_anim_m_picking_up = txtr.getAnimation(is->getName(), "picking_up");
        assert(ret->_anim_m_picking_up != nullptr);
        ret->_anim_m_throwing = txtr.getAnimation(is->getName(), "throwing");
        assert(ret->_anim_m_throwing != nullptr);
        ret->_anim_m_throwing->loops(0);

        ret->init();

        return ret;
    }

    spHenchPig SpriteFactory::PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spHenchPig ret = std::make_shared<pnk::PigBomb>(so, is);
        ret->_type_num = SpriteFactory::TN_PIG_BOMB;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_loitering = txtr.getAnimation(is->getName(), "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_bubbling = txtr.getAnimation(is->getName(), "bubbling");
        assert(ret->_anim_m_bubbling != nullptr);
        ret->_anim_m_picking_up = txtr.getAnimation(is->getName(), "picking_up");
        assert(ret->_anim_m_picking_up != nullptr);
        ret->_anim_m_throwing = txtr.getAnimation(is->getName(), "throwing");
        assert(ret->_anim_m_throwing != nullptr);

        ret->init();

        return ret;
    }

    spCollisionSprite SpriteFactory::PigPoof(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spMoodies ret = std::make_shared<pnk::Moodies>(so, is);
        ret->_type_num = SpriteFactory::TN_PIG_POOF;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_standard = txtr.getAnimation(is->getName(), "poof");
        assert(ret->_anim_m_standard != nullptr);
        ret->_anim_m_standard->loops(1);

        ret->init();

        return ret;
    }


    spReward SpriteFactory::Reward(dang::TmxExtruder &txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spReward ret = std::make_shared<pnk::Reward>(so, is);

        if(so->type == SpriteFactory::T_COIN_SILVER)
        {
            ret->_type_num = SpriteFactory::TN_COIN_SILVER;
        }
        else if(so->type == SpriteFactory::T_COIN_GOLD)
        {
            ret->_type_num = SpriteFactory::TN_COIN_GOLD;
        }
        else if(so->type == SpriteFactory::T_GEM_BLUE)
        {
            ret->_type_num = SpriteFactory::TN_GEM_BLUE;
        }
        else if(so->type == SpriteFactory::T_GEM_GREEN)
        {
            ret->_type_num = SpriteFactory::TN_GEM_GREEN;
        }
        else if(so->type == SpriteFactory::T_GEM_RED)
        {
            ret->_type_num = SpriteFactory::TN_GEM_RED;
        }
        else if(so->type == SpriteFactory::T_POTION_BLUE)
        {
            ret->_type_num = SpriteFactory::TN_POTION_BLUE;
        }
        else if(so->type == SpriteFactory::T_POTION_RED)
        {
            ret->_type_num = SpriteFactory::TN_POTION_RED;
        }
        else if(so->type == SpriteFactory::T_POTION_GREEN)
        {
            ret->_type_num = SpriteFactory::TN_POTION_GREEN;
        }
        else
        {
            std::cout << "current TN Type not implemented in SpriteFactory Reward: " << so->type << std::endl;
            ret->_type_num = SpriteFactory::TN_COIN_SILVER;
        }

        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->init();

        return ret;
    }

    spBubble SpriteFactory::Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left)
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

    spThrowies SpriteFactory::Crate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left)
    {
        spThrowies ret = std::make_shared<pnk::Craties>(so, is);
        ret->_type_num = SpriteFactory::TN_FLYING_CRATE;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_to_the_left = to_the_left;

        // bobbling with catched enemy
        ret->_anim_flying = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{34}, 600, dang::Ease::OutQuad , -1, false, 2000));
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(0);

        ret->init();

        return ret;
    }

    spThrowies SpriteFactory::Bomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left)
    {
        spThrowies ret = std::make_shared<pnk::Bombies>(so, is);
        ret->_type_num = SpriteFactory::TN_FLYING_BOMB;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_to_the_left = to_the_left;

        // bobbling with catched enemy
        ret->_anim_flying = txtr.getAnimation(is->getName(), "bomb_on");
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(-1);

        ret->init();

        return ret;
    }

    spThrowies SpriteFactory::Cannonball(dang::TmxExtruder &txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left)
    {
        spThrowies ret = std::make_shared<pnk::Throwies>(so, is);
        ret->_type_num = SpriteFactory::TN_FLYING_CANNONBALL;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_to_the_left = to_the_left;

        // bobbling with catched enemy
        ret->_anim_flying = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{26, 27, 28, 29}, 600, dang::Ease::OutQuad , -1, false, 2000));
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(0);

        ret->init();

        return ret;
    }

}
