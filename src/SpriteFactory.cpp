// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <cassert>
#include <iostream>
#include <src/actors/npc/PigCrate.h>
#include <src/actors/npc/PigBomb.h>
#include <src/actors/throwies/Throwies.h>
#include <src/actors/throwies/Craties.h>
#include <src/actors/throwies/Bombies.h>
#include <src/actors/throwies/Cannonball.h>
#include "src/actors/throwies/Bubble.h"
#include <src/actors/others/Moodies.h>
#include <src/actors/others/MoodiesThatHurt.h>
#include <src/actors/others/Cannon.h>
#include <src/actors/npc/PigCannon.h>
#include "CollisionSprite.hpp"
#include "TmxExtruder.hpp"
#include "Imagesheet.hpp"
#include "Sprite.hpp"
#include "tween/TwAnim.hpp"
#include "tween/Ease.hpp"
#include "path/SceneGraph.hpp"

#include "SpriteFactory.hpp"
#include "src/actors/hero/Hero.h"
#include "src/actors/npc/Enemy.h"
#include "src/actors/others/RoomTrigger.h"
#include "src/actors/others/LevelTrigger.h"
#include "PnkEvent.h"

#include "GSPlay.h"

namespace pnk
{
    spHero SpriteFactory::King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        assert(is != nullptr);
        spHero ret = std::make_shared<pnk::Hero>(so, is);
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_type_num = dang::SpriteType::KING;

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
        ret->_type_num = dang::SpriteType::HOTRECT;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spCollisionSprite SpriteFactory::HotrectPlatform(const dang::tmx_spriteobject* so)
    {
        spCollisionSprite ret = std::make_shared<dang::CollisionSprite>(so, nullptr);
        ret->_visible = false;
        ret->_type_num = dang::SpriteType::HOTRECT_PLATFORM;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spCollisionSprite SpriteFactory::RoomTrigger(const dang::tmx_spriteobject* so)
    {
        spCollisionSprite ret = std::make_shared<pnk::RoomTrigger>(so, false);
        ret->_visible = false;
        ret->_type_num = dang::SpriteType::ROOM_TRIGGER;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spCollisionSprite SpriteFactory::WarpRoomTrigger(const dang::tmx_spriteobject* so)
    {
        spCollisionSprite ret = std::make_shared<pnk::RoomTrigger>(so, true);
        ret->_visible = false;
        ret->_type_num = dang::SpriteType::WARP_ROOM_TRIGGER;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    spCollisionSprite SpriteFactory::LevelTrigger(const dang::tmx_spriteobject* so)
    {
        spCollisionSprite ret = std::make_shared<pnk::LevelTrigger>(so);
        ret->_visible = false;
        ret->_type_num = dang::SpriteType::LEVEL_TRIGGER;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        return ret;
    }

    void SpriteFactory::attachBehaviourTree(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const spCollisionSprite& cs)
    {
        if (so->bt.length() > 0)
        {
            std::shared_ptr<dang::NTree> ntr = txtr._gear->getNTree(so->bt);
            if (ntr != nullptr)
            {
                cs->setNTreeState(std::make_shared<dang::NTreeState>(ntr));
            }
        }
    }

    spHenchPig SpriteFactory::NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp)
    {
        spHenchPig ret = std::make_shared<pnk::HenchPig>(so, is);
        ret->_type_num = dang::SpriteType::PIG_NORMAL;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_loitering = txtr.getAnimation(is->getName(), "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_bubbling = txtr.getAnimation(is->getName(), "bubbling");
        assert(ret->_anim_m_bubbling != nullptr);

        ret->init();

        attachBehaviourTree(txtr, so, ret);

        initSceneGraph(sp, ret);

        return ret;
    }

    // it is a wooden crate.. for making buses out of them
    spHenchPig SpriteFactory::PigCrate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp)
    {
        spHenchPig ret = std::make_shared<pnk::PigCrate>(so, is);
        ret->_type_num = dang::SpriteType::PIG_BOX;
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

        // function checks if there is a so.behaviourtree property
        // if set, checks in gear whats the pointer to said tree
        // generates treestate object, sets pointer to tree and sets that to the new sprite
        attachBehaviourTree(txtr, so, ret);

        initSceneGraph(sp, ret);

        return ret;
    }

    spHenchPig SpriteFactory::PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp)
    {
        spHenchPig ret = std::make_shared<pnk::PigBomb>(so, is);
        ret->_type_num = dang::SpriteType::PIG_BOMB;
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

        attachBehaviourTree(txtr, so, ret);

        initSceneGraph(sp, ret);

        return ret;
    }

    spHenchPig SpriteFactory::PigCannoneer(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp)
    {
        spPigCannon ret = std::make_shared<pnk::PigCannon>(so, is);
        ret->_type_num = dang::SpriteType::PIG_CANNON;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_loitering = txtr.getAnimation(is->getName(), "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_picking_up = txtr.getAnimation(is->getName(), "lighting_match");
        assert(ret->_anim_m_picking_up != nullptr);
        ret->_anim_m_picking_up->loops(1);
        ret->_anim_m_picking_up->setFinishedCallback(std::bind(&PigCannon::matchLit, ret.get()));

        ret->_anim_m_match_lit = txtr.getAnimation(is->getName(), "match_lit");
        assert(ret->_anim_m_match_lit != nullptr);
        ret->_anim_m_match_lit->loops(5);
        ret->_anim_m_match_lit->setFinishedCallback(std::bind(&PigCannon::lightingCannon, ret.get()));

        ret->_anim_m_throwing = txtr.getAnimation(is->getName(), "lighting_cannon");
        assert(ret->_anim_m_throwing != nullptr);
        ret->_anim_m_throwing->loops(2);
        ret->_anim_m_throwing->setFinishedCallback(std::bind(&PigCannon::cannonIsLit, ret.get()));

        ret->_transform = blit::SpriteTransform::HORIZONTAL;

        ret->init();

        attachBehaviourTree(txtr, so, ret);

        // removed scene graph since there is no walking involved
        // initSceneGraph(sp, ret);

        return ret;
    }

    // HACK, REFACTOR
    spPigCannon SpriteFactory::PigCannoneerWCannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp)
    {
        spPigCannon ret = std::make_shared<pnk::PigCannon>(so, is);
        ret->_type_num = dang::SpriteType::PIG_CANNON;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_loitering = txtr.getAnimation(is->getName(), "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_picking_up = txtr.getAnimation(is->getName(), "lighting_match");
        assert(ret->_anim_m_picking_up != nullptr);
        ret->_anim_m_picking_up->loops(1);
        ret->_anim_m_picking_up->setFinishedCallback(std::bind(&PigCannon::matchLit, ret.get()));

        ret->_anim_m_match_lit = txtr.getAnimation(is->getName(), "match_lit");
        assert(ret->_anim_m_match_lit != nullptr);
        ret->_anim_m_match_lit->loops(5);
        ret->_anim_m_match_lit->setFinishedCallback(std::bind(&PigCannon::lightingCannon, ret.get()));

        ret->_anim_m_throwing = txtr.getAnimation(is->getName(), "lighting_cannon");
        assert(ret->_anim_m_throwing != nullptr);
        ret->_anim_m_throwing->loops(2);
        ret->_anim_m_throwing->setFinishedCallback(std::bind(&PigCannon::cannonIsLit, ret.get()));

        ret->_transform = blit::SpriteTransform::HORIZONTAL;

        ret->init();

        attachBehaviourTree(txtr, so, ret);

        // removed scene graph since there is no walking involved
        // initSceneGraph(sp, ret);
        std::shared_ptr<dang::Imagesheet> imgs = txtr.getImagesheet("character_cannonsnpigs");
        ret->_myCannon = CannonForCannoneer(txtr, so, imgs);

        return ret;
    }

    spCannon SpriteFactory::CannonForCannoneer(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spCannon ret = std::make_shared<pnk::Cannon>();

        ret->_type_num = dang::SpriteType::CANNON;
        ret->_type_name = "cannon";
        ret->setPosX(so->x + 25); // TODO respect _transform
        ret->setPosY(so->y);
        ret->setSize(32, 32);
        ret->_visible = true;
        ret->_img_index = 0;
        ret->_imagesheet = is;

        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "idling");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_shooting = txtr.getAnimation(is->getName(), "shooting");
        assert(ret->_anim_m_shooting != nullptr);
        ret->_anim_m_shooting->loops(1);
        ret->_anim_m_shooting->setFinishedCallback(std::bind(&Cannon::cannonHasFired, ret.get()));

        ret->init();

        ret->_transform = blit::SpriteTransform::HORIZONTAL;

        return ret;
    }

    spCollisionSprite SpriteFactory::Cannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spCannon ret = std::make_shared<pnk::Cannon>(so, is);
        ret->_type_num = dang::SpriteType::CANNON;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "idling");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_shooting = txtr.getAnimation(is->getName(), "shooting");
        assert(ret->_anim_m_shooting != nullptr);

        ret->init();

        ret->_transform = blit::SpriteTransform::HORIZONTAL;

        return ret;
    }

    spCollisionSprite SpriteFactory::Cannonmuzzle(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spMoodies ret = std::make_shared<pnk::Moodies>(so, is);
        ret->_type_num = dang::SpriteType::CANNON_MUZZLE;
        ret->setCOType(dang::CollisionSpriteLayer::COT_RIGID);

        ret->_anim_m_standard = txtr.getAnimation(is->getName(), "muzzle_flash");
        assert(ret->_anim_m_standard != nullptr);
        ret->_anim_m_standard->loops(1);

        ret->init();

        return ret;
    }

    spCollisionSprite SpriteFactory::PigPoof(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spMoodies ret = std::make_shared<pnk::Moodies>(so, is);
        ret->_type_num = dang::SpriteType::PIG_POOF;
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
            ret->_type_num = dang::SpriteType::COIN_SILVER;
        }
        else if(so->type == SpriteFactory::T_COIN_GOLD)
        {
            ret->_type_num = dang::SpriteType::COIN_GOLD;
        }
        else if(so->type == SpriteFactory::T_GEM_BLUE)
        {
            ret->_type_num = dang::SpriteType::GEM_BLUE;
        }
        else if(so->type == SpriteFactory::T_GEM_GREEN)
        {
            ret->_type_num = dang::SpriteType::GEM_GREEN;
        }
        else if(so->type == SpriteFactory::T_GEM_RED)
        {
            ret->_type_num = dang::SpriteType::GEM_RED;
        }
        else if(so->type == SpriteFactory::T_POTION_BLUE)
        {
            ret->_type_num = dang::SpriteType::POTION_BLUE;
        }
        else if(so->type == SpriteFactory::T_POTION_RED)
        {
            ret->_type_num = dang::SpriteType::POTION_RED;
        }
        else if(so->type == SpriteFactory::T_POTION_GREEN)
        {
            ret->_type_num = dang::SpriteType::POTION_GREEN;
        }
        else
        {
            std::cout << "current TN Type not implemented in SpriteFactory Reward: " << so->type << std::endl;
            ret->_type_num = dang::SpriteType::COIN_SILVER;
        }

        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->init();

        return ret;
    }

    spBubble SpriteFactory::Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left)
    {
        spBubble ret = std::make_shared<pnk::Bubble>(so, is);
        ret->_type_num = dang::SpriteType::BUBBLE;
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
        ret->_type_num = dang::SpriteType::FLYING_CRATE;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_to_the_left = to_the_left;

        // flying crates
        ret->_anim_flying = txtr.getAnimation(is->getName(), "crate");
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(-1);

        ret->init();

        return ret;
    }

    spThrowies SpriteFactory::Bomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spBombies ret = std::make_shared<pnk::Bombies>(so, is);
        ret->_type_num = dang::SpriteType::FLYING_BOMB;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_flying = txtr.getAnimation(is->getName(), "bomb_off");
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(-1);

        ret->_anim_on_fire = txtr.getAnimation(is->getName(), "bomb_on");
        assert(ret->_anim_on_fire != nullptr);
        ret->_anim_on_fire->loops(3);

        ret->init();

        return ret;
    }

    spCollisionSprite SpriteFactory::Explosion(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spMoodiesThatHurt ret = std::make_shared<pnk::MoodiesThatHurt>(so, is);
        ret->_type_num = dang::SpriteType::EXPLOSION;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);

        ret->_anim_m_standard = txtr.getAnimation(is->getName(), "boom");
        assert(ret->_anim_m_standard != nullptr);
        ret->_anim_m_standard->loops(1);

        ret->init();

        return ret;
    }

    spThrowies SpriteFactory::Cannonball(dang::TmxExtruder &txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left)
    {
        spThrowies ret = std::make_shared<pnk::Cannonball>(so, is);
        ret->_type_num = dang::SpriteType::FLYING_CANNONBALL;
        ret->setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        ret->_to_the_left = to_the_left;

        ret->_anim_flying = txtr.getAnimation(is->getName(), "cannonball");
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(-1);

        ret->init();

        return ret;
    }

    void SpriteFactory::initSceneGraph(const spScreenPlay &sp, const spEnemy &spr)
    {
        for (auto room : sp->_acts)
        {
            if (room._extent_pixels.contains(spr->getPos()))
            {
                spr->_scene_graph = room._scene_graph;
                spWaypoint wp = spr->_scene_graph->findNearestWaypoint(spr->getHotrectAbs().center());
                if (spr->_scene_graph->waypointReached(spr->getHotrectAbs(), wp))
                {
                    spr->_current_wp = wp;
                }
                else
                {
                    spr->_path.push_back(wp);
                    spr->_path_index = 0;
                    spr->startOutToWaypoint();
                }
                break;
            }
        }

    }

}
