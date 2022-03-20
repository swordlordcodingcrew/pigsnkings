// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "SpriteFactory.hpp"
#include "GSPlay.h"
#include "pnk_globals.h"
#include "actors/npc/PigCrate.h"
#include "actors/npc/PigBomb.h"
#include "actors/npc/PigBoss.h"
#include "actors/throwies/Craties.h"
#include "actors/throwies/Bombies.h"
#include "actors/throwies/Cannonball.h"
#include "actors/throwies/Bubble.h"
#include "actors/others/Moodies.h"
#include "actors/others/MoodiesThatHurt.h"
#include "actors/others/Cannon.h"
#include "actors/others/Reward.h"
#include "actors/npc/PigCannon.h"
#include "actors/hero/Hero.h"
#include "actors/others/RoomTrigger.h"
#include "actors/others/LevelTrigger.h"
#include "actors/others/BossbattleTrigger.h"
#include "actors/others/SavepointTrigger.h"

#include <Gear.hpp>
#include <CollisionSprite.hpp>
#include <TmxExtruder.hpp>
#include <Imagesheet.hpp>
#include <Sprite.hpp>
#include <tween/TwAnim.hpp>
#include <tween/Ease.hpp>
#include <path/SceneGraph.hpp>

#include <32blit.hpp>

#include <cfloat>
#include <cassert>
#include <iostream>
#include <libs/DANG/src/Rand.hpp>

namespace pnk
{
    spHero SpriteFactory::King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        assert(is != nullptr);
        spHero ret = std::make_shared<pnk::Hero>(so, is);
        ret->_type_num = ST_KING;

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

    spBoss SpriteFactory::Boss(dang::TmxExtruder &txtr, const dang::tmx_spriteobject *so, const std::unordered_map<std::string, spImagesheet> &iss, spScreenPlay& sp)
    {
        spImagesheet is = iss.at(so->tileset);

        assert(is != nullptr);
        spBoss ret = std::make_shared<pnk::PigBoss>(so, is);
        ret->_type_num = ST_PIG_BOSS;

        ret->_anim_m_sleeping = txtr.getAnimation(is, "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_sleeping->delay(300);
        ret->_anim_m_running = txtr.getAnimation(is, "running");
        assert(ret->_anim_m_running != nullptr);
        ret->_anim_m_landing = txtr.getAnimation(is, "landing");
        assert(ret->_anim_m_landing != nullptr);
        ret->_anim_m_landing->loops(1);
        ret->_anim_m_jumping = txtr.getAnimation(is, "jumping");
        assert(ret->_anim_m_jumping != nullptr);
        ret->_anim_m_jumping->loops(1);
        ret->_anim_m_hit = txtr.getAnimation(is, "hit");
        assert(ret->_anim_m_hit != nullptr);
        ret->_anim_m_hit->loops(2);
        ret->_anim_m_die = txtr.getAnimation(is, "die");
        assert(ret->_anim_m_die != nullptr);
        ret->_anim_m_die->loops(1);

        ret->_anim_m_recovering = std::make_shared<dang::TwAnim>(*(ret->_anim_m_sleeping));
        ret->_anim_m_recovering->duration(300);
        ret->_anim_m_recovering->delay(0);

        attachBehaviourTree(txtr, so, ret);

        ret->init();

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::Hotrect(const dang::tmx_spriteobject* so)
    {
        dang::spCollisionSprite ret = std::make_shared<dang::CollisionSprite>(so, nullptr);
        ret->_visible = false;
        ret->_type_num = ST_HOTRECT;
        ret->setRigid(true);

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::HotrectPlatform(const dang::tmx_spriteobject* so)
    {
        dang::spCollisionSprite ret = std::make_shared<dang::CollisionSprite>(so, nullptr);
        ret->_visible = false;
        ret->_type_num = ST_HOTRECT_PLATFORM;
        ret->setRigid(true);

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::RoomTrigger(const dang::tmx_spriteobject* so)
    {
        dang::spCollisionSprite ret = std::make_shared<pnk::RoomTrigger>(so, false);
        ret->_visible = false;
        ret->_type_num = ST_ROOM_TRIGGER;
        ret->setRigid(true);

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::WarpRoomTrigger(const dang::tmx_spriteobject* so)
    {
        dang::spCollisionSprite ret = std::make_shared<pnk::RoomTrigger>(so, true);
        ret->_visible = false;
        ret->_type_num = ST_WARP_ROOM_TRIGGER;
        ret->setRigid(true);

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::LevelTrigger(const dang::tmx_spriteobject* so)
    {
        dang::spCollisionSprite ret = std::make_shared<pnk::LevelTrigger>(so);
        ret->_visible = false;
        ret->_type_num = ST_LEVEL_TRIGGER;
        ret->setRigid(true);

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::BossbattleTrigger(const dang::tmx_spriteobject* so)
    {
        dang::spCollisionSprite ret = std::make_shared<pnk::BossbattleTrigger>(so);
        ret->_visible = false;
        ret->_type_num = ST_BOSS_BATTLE_TRIGGER;
        ret->setRigid(true);

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::SavepointTrigger(const dang::tmx_spriteobject* so)
    {
        dang::spCollisionSprite ret = std::make_shared<pnk::SavepointTrigger>(so);
        ret->_visible = false;
        ret->_type_num = ST_SAVEPOINT_TRIGGER;
        ret->setRigid(true);

        return ret;
    }

    spHenchPig SpriteFactory::NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp)
    {
        spHenchPig ret = std::make_shared<pnk::HenchPig>(so, is);
        ret->_type_num = ST_PIG_NORMAL;

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_sleeping->delay(dang::Rand::get(uint32_t(1000), uint32_t(2000)));

        ret->_anim_m_loitering = txtr.getAnimation(is->getName(), "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_bubbling = txtr.getAnimation(is->getName(), "bubbling");
        assert(ret->_anim_m_bubbling != nullptr);

        attachBehaviourTree(txtr, so, ret);

        dang::spNTree t = sp->_bt["berserk"];
        if (t != nullptr)
        {
            ret->setNTreeBerserk(std::make_shared<dang::NTreeState>(t));
        }

        ret->init();

        return ret;
    }

    // it is a wooden crate.. for making buses out of them
    spHenchPig SpriteFactory::PigCrate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::unordered_map<std::string, spImagesheet> &iss, spScreenPlay& sp)
    {
        spImagesheet is = iss.at(so->tileset);

        spHenchPig ret = std::make_shared<pnk::PigCrate>(so, is);
        ret->_type_num = ST_PIG_CRATE;

        ret->_anim_m_sleeping = txtr.getAnimation(is, "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_sleeping->delay(dang::Rand::get(uint32_t(1000), uint32_t(2000)));

        ret->_anim_m_loitering = txtr.getAnimation(is, "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_bubbling = txtr.getAnimation(is, "bubbling");
        assert(ret->_anim_m_bubbling != nullptr);
        ret->_anim_m_picking_up = txtr.getAnimation(is, "picking_up");
        assert(ret->_anim_m_picking_up != nullptr);
        ret->_anim_m_throwing = txtr.getAnimation(is, "throwing");
        assert(ret->_anim_m_throwing != nullptr);
        ret->_anim_m_throwing->loops(0);

        // animations without crate
        is = iss.at("gfx_pig");
        ret->_anim_alt_sleeping = txtr.getAnimation(is, "sleeping");
        assert(ret->_anim_alt_sleeping != nullptr);
        ret->_anim_alt_sleeping->delay(dang::Rand::get(uint32_t(1000), uint32_t(2000)));

        ret->_anim_alt_loitering = txtr.getAnimation(is, "loitering");
        assert(ret->_anim_alt_loitering != nullptr);

        attachBehaviourTree(txtr, so, ret);

        dang::spNTree t = sp->_bt["berserk"];
        if (t != nullptr)
        {
            ret->setNTreeBerserk(std::make_shared<dang::NTreeState>(t));
        }

        ret->init();

        return ret;
    }

    spHenchPig SpriteFactory::PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::unordered_map<std::string, spImagesheet> &iss, spScreenPlay& sp)
    {
        spImagesheet is = iss.at(so->tileset);

        spHenchPig ret = std::make_shared<pnk::PigBomb>(so, is);
        ret->_type_num = ST_PIG_BOMB;

        ret->_anim_m_sleeping = txtr.getAnimation(is, "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_sleeping->delay(dang::Rand::get(uint32_t(1000), uint32_t(2000)));

        ret->_anim_m_loitering = txtr.getAnimation(is, "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_bubbling = txtr.getAnimation(is, "bubbling");
        assert(ret->_anim_m_bubbling != nullptr);
        ret->_anim_m_picking_up = txtr.getAnimation(is, "picking_up");
        assert(ret->_anim_m_picking_up != nullptr);
        ret->_anim_m_throwing = txtr.getAnimation(is, "throwing");
        assert(ret->_anim_m_throwing != nullptr);
        ret->_anim_m_throwing->loops(0);

        // animations without bomb
        is = iss.at("gfx_pig");
        ret->_anim_alt_sleeping = txtr.getAnimation(is, "sleeping");
        assert(ret->_anim_alt_sleeping != nullptr);
        ret->_anim_alt_sleeping->delay(dang::Rand::get(uint32_t(1000), uint32_t(2000)));

        ret->_anim_alt_loitering = txtr.getAnimation(is, "loitering");
        assert(ret->_anim_alt_loitering != nullptr);

        attachBehaviourTree(txtr, so, ret);

        dang::spNTree t = sp->_bt["berserk"];
        if (t != nullptr)
        {
            ret->setNTreeBerserk(std::make_shared<dang::NTreeState>(t));
        }

        ret->init();

        return ret;

    }

    spHenchPig SpriteFactory::PigCannoneer(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp)
    {
        spPigCannon ret = std::make_shared<pnk::PigCannon>(so, is);
        ret->_type_num = ST_PIG_CANNON;

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_sleeping->delay(dang::Rand::get(uint32_t(1000), uint32_t(2000)));

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

        return ret;
    }

    // HACK, REFACTOR
    spPigCannon SpriteFactory::PigCannoneerWCannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp)
    {
        spPigCannon ret = std::make_shared<pnk::PigCannon>(so, is);
        ret->_type_num = ST_PIG_CANNON;

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_sleeping->delay(dang::Rand::get(uint32_t(1000), uint32_t(2000)));

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

        std::shared_ptr<dang::Imagesheet> imgs = txtr.getImagesheet("character_cannonsnpigs");
        ret->_myCannon = CannonForCannoneer(txtr, so, imgs);

        return ret;
    }

    spCannon SpriteFactory::CannonForCannoneer(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spCannon ret = std::make_shared<pnk::Cannon>();

        ret->_type_num = ST_CANNON;
        ret->_type_name = "cannon";
        ret->setPosX(so->x + 25); // TODO respect _transform
        ret->setPosY(so->y);
        ret->setSize(32, 32);
        ret->_visible = true;
        ret->_img_index = 0;
        ret->_imagesheet = is;

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

    dang::spCollisionSprite SpriteFactory::Cannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spCannon ret = std::make_shared<pnk::Cannon>(so, is);
        ret->_type_num = ST_CANNON;

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "idling");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_shooting = txtr.getAnimation(is->getName(), "shooting");
        assert(ret->_anim_m_shooting != nullptr);

        ret->init();

        ret->_transform = blit::SpriteTransform::HORIZONTAL;

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::Cannonmuzzle(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spMoodies ret = std::make_shared<pnk::Moodies>(so, is);
        ret->_type_num = ST_CANNON_MUZZLE;
        ret->setRigid(true);

        ret->_anim_m_standard = txtr.getAnimation(is->getName(), "muzzle_flash");
        assert(ret->_anim_m_standard != nullptr);
        ret->_anim_m_standard->loops(1);

        ret->init();

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::PigPoof(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spMoodies ret = std::make_shared<pnk::Moodies>(so, is);
        ret->_type_num = ST_PIG_POOF;

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
            ret->_type_num = ST_COIN_SILVER;
        }
        else if(so->type == SpriteFactory::T_COIN_GOLD)
        {
            ret->_type_num = ST_COIN_GOLD;
        }
        else if(so->type == SpriteFactory::T_GEM_BLUE)
        {
            ret->_type_num = ST_GEM_BLUE;
        }
        else if(so->type == SpriteFactory::T_GEM_GREEN)
        {
            ret->_type_num = ST_GEM_GREEN;
        }
        else if(so->type == SpriteFactory::T_GEM_RED)
        {
            ret->_type_num = ST_GEM_RED;
        }
        else if(so->type == SpriteFactory::T_POTION_BLUE)
        {
            ret->_type_num = ST_POTION_BLUE;
        }
        else if(so->type == SpriteFactory::T_POTION_RED)
        {
            ret->_type_num = ST_POTION_RED;
        }
        else if(so->type == SpriteFactory::T_POTION_GREEN)
        {
            ret->_type_num = ST_POTION_GREEN;
        }
        else
        {
            std::cout << "current TN Type not implemented in SpriteFactory Reward: " << so->type << std::endl;
            ret->_type_num = ST_COIN_SILVER;
        }

        ret->init();

        return ret;
    }

    spBubble SpriteFactory::Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left)
    {
        spBubble ret = std::make_shared<pnk::Bubble>(so, is);
        ret->_type_num = ST_BUBBLE;
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
        ret->_type_num = ST_FLYING_CRATE;
        ret->_to_the_left = to_the_left;

        // flying crates
        ret->_anim_flying = txtr.getAnimation(is, "crate");
        assert(ret->_anim_flying != nullptr);

        // crate destruction
        ret->_anim_destruction = txtr.getAnimation(is, "hit", dang::Ease::Linear, 1);
        assert(ret->_anim_destruction != nullptr);

        ret->init();

        return ret;
    }

    spThrowies SpriteFactory::Bomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spBombies ret = std::make_shared<pnk::Bombies>(so, is);
        ret->_type_num = ST_FLYING_BOMB;

        ret->_anim_flying = txtr.getAnimation(is->getName(), "bomb_off");
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(-1);

        ret->_anim_on_fire = txtr.getAnimation(is->getName(), "bomb_on");
        assert(ret->_anim_on_fire != nullptr);
        ret->_anim_on_fire->loops(3);

        ret->init();

        return ret;
    }

    dang::spCollisionSprite SpriteFactory::Explosion(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is)
    {
        spMoodiesThatHurt ret = std::make_shared<pnk::MoodiesThatHurt>(so, is);
        ret->_type_num = ST_EXPLOSION;

        ret->_anim_m_standard = txtr.getAnimation(is->getName(), "boom");
        assert(ret->_anim_m_standard != nullptr);
        ret->_anim_m_standard->loops(1);

        ret->init();

        return ret;
    }

    spThrowies SpriteFactory::Cannonball(dang::TmxExtruder &txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left)
    {
        spThrowies ret = std::make_shared<pnk::Cannonball>(so, is);
        ret->_type_num = ST_FLYING_CANNONBALL;
        ret->_to_the_left = to_the_left;

        ret->_anim_flying = txtr.getAnimation(is->getName(), "cannonball");
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(-1);

        ret->init();

        return ret;
    }

    void SpriteFactory::attachBehaviourTree(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const dang::spCollisionSprite& cs)
    {
        if (so->bt.length() > 0)
        {
            std::shared_ptr<dang::NTree> ntr = txtr._gear->getNTree(so->bt);
            if (ntr != nullptr)
            {
                cs->setNTreeState(std::make_shared<dang::NTreeState>(ntr));
            }
            else
            {
                std::cout << "ERROR: could not find behaviour tree " << so->bt << std::endl;
            }
        }
    }


}
