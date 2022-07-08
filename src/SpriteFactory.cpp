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
#include <Rand.hpp>
#include <TmxExtruder.hpp>
#include <Imagesheet.hpp>
#include <tween/TwAnim.hpp>
#include <tween/Ease.hpp>
#include <path/SceneGraph.hpp>
#include <bt/NTreeState.h>
#include <sprite/ColSpr.hpp>
#include <sprite/FullColSpr.hpp>

#include <32blit.hpp>

#include <cfloat>
#include <cassert>

namespace pnk
{
    spHero SpriteFactory::King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        assert(is != nullptr);
        spHero ret = std::make_shared<pnk::Hero>(so, is);
        ret->setTypeNum(ST_KING);

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

    spBoss SpriteFactory::Boss(dang::TmxExtruder &txtr, const dang::tmx_spriteobject *so, const std::unordered_map<std::string, dang::spImagesheet> &iss, spScreenPlay& sp)
    {
        dang::spImagesheet is = iss.at(so->tileset);

        assert(is != nullptr);
        spBoss ret = std::make_shared<pnk::PigBoss>(so, is);
        ret->setTypeNum(ST_PIG_BOSS);

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

        attachBehaviourTree(sp, so, ret);

        ret->init();

        return ret;
    }

    dang::spColSpr SpriteFactory::RigidObj(const dang::tmx_spriteobject* so)
    {
        dang::spColSpr ret = std::make_shared<dang::ColSpr>(so);
        setTypeNum(ret, so->type);
        ret->setRigid(true);

        return ret;

    }

    spReward SpriteFactory::Reward(dang::TmxExtruder &txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spReward ret = std::make_shared<pnk::Reward>(so, is);
        setTypeNum(ret, so->type);
        ret->init();
        return ret;
    }

    void SpriteFactory::setTypeNum(dang::spSprObj spo, const std::string& type)
    {
        if      (type == SpriteFactory::T_KING)                  { spo->setTypeNum(ST_KING); }
        else if (type == SpriteFactory::T_BOSS)                  { spo->setTypeNum(ST_PIG_BOSS); }
        else if (type == SpriteFactory::T_BUBBLE_PROTO)          { spo->setTypeNum(ST_BUBBLE); }
        else if (type == SpriteFactory::T_CRATE_PROTO)           { spo->setTypeNum(ST_FLYING_CRATE); }
        else if (type == SpriteFactory::T_BOMB_PROTO)            { spo->setTypeNum(ST_FLYING_BOMB); }
        else if (type == SpriteFactory::T_EXPLOSION_PROTO)       { spo->setTypeNum(ST_EXPLOSION); }
        else if (type == SpriteFactory::T_PIG_POOF_PROTO)        { spo->setTypeNum(ST_PIG_POOF); }
        else if (type == SpriteFactory::T_CANNONMUZZLE_PROTO)    { spo->setTypeNum(ST_CANNON_MUZZLE); }
        else if (type == SpriteFactory::T_CANNONBALL_PROTO)      { spo->setTypeNum(ST_FLYING_CANNONBALL); }
        else if (type == SpriteFactory::T_HOTRECT)               { spo->setTypeNum(ST_HOTRECT); }
        else if (type == SpriteFactory::T_HOTRECT_PLATFORM)      { spo->setTypeNum(ST_HOTRECT_PLATFORM); }
        else if (type == SpriteFactory::T_ROOM_TRIGGER)          { spo->setTypeNum(ST_ROOM_TRIGGER); }
        else if (type == SpriteFactory::T_BOSSBATTLE_TRIGGER)    { spo->setTypeNum(ST_BOSS_BATTLE_TRIGGER); }
        else if (type == SpriteFactory::T_SAVEPOINT_TRIGGER)     { spo->setTypeNum(ST_SAVEPOINT_TRIGGER); }
        else if (type == SpriteFactory::T_LEVEL_TRIGGER)         { spo->setTypeNum(ST_LEVEL_TRIGGER); }
        else if (type == SpriteFactory::T_WARP_ROOM_TRIGGER)     { spo->setTypeNum(ST_WARP_ROOM_TRIGGER); }
        else if (type == SpriteFactory::T_NORMAL_PIG_HIVE)       { spo->setTypeNum(ST_NORMAL_PIG_HIVE); }
        else if (type == SpriteFactory::T_PIG_NORMAL)            { spo->setTypeNum(ST_PIG_NORMAL); }
        else if (type == SpriteFactory::T_PIG_BETTER)            { spo->setTypeNum(ST_PIG_BETTER); }
        else if (type == SpriteFactory::T_PIG_BOX)               { spo->setTypeNum(ST_PIG_CRATE); }
        else if (type == SpriteFactory::T_PIG_BOMB)              { spo->setTypeNum(ST_PIG_BOMB); }
        else if (type == SpriteFactory::T_PIG_CANNON)            { spo->setTypeNum(ST_PIG_CANNON); }
        else if (type == SpriteFactory::T_CANNON)                { spo->setTypeNum(ST_CANNON); }
        else if (type == SpriteFactory::T_COIN_SILVER)           { spo->setTypeNum(ST_COIN_SILVER); }
        else if (type == SpriteFactory::T_COIN_GOLD)             { spo->setTypeNum(ST_COIN_GOLD); }
        else if (type == SpriteFactory::T_GEM_BLUE)              { spo->setTypeNum(ST_GEM_BLUE); }
        else if (type == SpriteFactory::T_GEM_GREEN)             { spo->setTypeNum(ST_GEM_GREEN); }
        else if (type == SpriteFactory::T_GEM_RED)               { spo->setTypeNum(ST_GEM_RED); }
        else if (type == SpriteFactory::T_POTION_BLUE)           { spo->setTypeNum(ST_POTION_BLUE); }
        else if (type == SpriteFactory::T_POTION_GREEN)          { spo->setTypeNum(ST_POTION_GREEN); }
        else if (type == SpriteFactory::T_POTION_RED)            { spo->setTypeNum(ST_POTION_RED); }
        else
        {
            std::printf("sprite type %s not implemented in SpriteFactory\n", type.c_str());
        }
    }

/*    dang::spColSpr SpriteFactory::Hotrect(const dang::tmx_spriteobject* so)
    {
        dang::spColSpr ret = std::make_shared<dang::ColSpr>(so);
        ret->setTypeNum(ST_HOTRECT);
        ret->setRigid(true);

        return ret;
    }

    dang::spColSpr SpriteFactory::HotrectPlatform(const dang::tmx_spriteobject* so)
    {
        dang::spColSpr ret = std::make_shared<dang::ColSpr>(so);
        ret->setTypeNum(ST_HOTRECT_PLATFORM);
        ret->setRigid(true);

        return ret;
    }
*/
    dang::spColSpr SpriteFactory::RoomTrigger(const dang::tmx_spriteobject* so, bool warp)
    {
        dang::spColSpr ret = std::make_shared<pnk::RoomTrigger>(so, warp);
        ret->setTypeNum(warp ? ST_WARP_ROOM_TRIGGER : ST_ROOM_TRIGGER);
        ret->setRigid(true);

        return ret;
    }

    dang::spColSpr SpriteFactory::LevelTrigger(const dang::tmx_spriteobject* so)
    {
        dang::spColSpr ret = std::make_shared<pnk::LevelTrigger>(so);
        ret->setTypeNum(ST_LEVEL_TRIGGER);
        ret->setRigid(true);

        return ret;
    }

    dang::spColSpr SpriteFactory::BossbattleTrigger(const dang::tmx_spriteobject* so)
    {
        dang::spColSpr ret = std::make_shared<pnk::BossbattleTrigger>(so);
        ret->setTypeNum(ST_BOSS_BATTLE_TRIGGER);
        ret->setRigid(true);

        return ret;
    }

    dang::spColSpr SpriteFactory::SavepointTrigger(const dang::tmx_spriteobject* so)
    {
        dang::spColSpr ret = std::make_shared<pnk::SavepointTrigger>(so);
        ret->setTypeNum(ST_SAVEPOINT_TRIGGER);
        ret->setRigid(true);

        return ret;
    }

    spHenchPig SpriteFactory::NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, spScreenPlay& sp)
    {
        spHenchPig ret = std::make_shared<pnk::HenchPig>(so, is);
        ret->setTypeNum(ST_PIG_NORMAL);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "sleeping");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_sleeping->delay(dang::Rand::get(uint32_t(1000), uint32_t(2000)));

        ret->_anim_m_loitering = txtr.getAnimation(is->getName(), "loitering");
        assert(ret->_anim_m_loitering != nullptr);
        ret->_anim_m_bubbling = txtr.getAnimation(is->getName(), "bubbling");
        assert(ret->_anim_m_bubbling != nullptr);

        attachBehaviourTree(sp, so, ret);

        dang::spNTree t = sp->_bt["berserk"];
        if (t != nullptr)
        {
            ret->setNTreeBerserk(std::make_shared<dang::NTreeState>(t));
        }

        ret->init();

        return ret;
    }

    // it is a wooden crate.. for making buses out of them
    spHenchPig SpriteFactory::PigCrate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::unordered_map<std::string, dang::spImagesheet> &iss, spScreenPlay& sp)
    {
        dang::spImagesheet is = iss.at(so->tileset);

        spHenchPig ret = std::make_shared<pnk::PigCrate>(so, is);
        ret->setTypeNum(ST_PIG_CRATE);

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

        attachBehaviourTree(sp, so, ret);

        dang::spNTree t = sp->_bt["berserk"];
        if (t != nullptr)
        {
            ret->setNTreeBerserk(std::make_shared<dang::NTreeState>(t));
        }

        ret->init();

        return ret;
    }

    spHenchPig SpriteFactory::PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::unordered_map<std::string, dang::spImagesheet> &iss, spScreenPlay& sp)
    {
        dang::spImagesheet is = iss.at(so->tileset);

        spHenchPig ret = std::make_shared<pnk::PigBomb>(so, is);
        ret->setTypeNum(ST_PIG_BOMB);

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

        attachBehaviourTree(sp, so, ret);

        dang::spNTree t = sp->_bt["berserk"];
        if (t != nullptr)
        {
            ret->setNTreeBerserk(std::make_shared<dang::NTreeState>(t));
        }

        ret->init();

        return ret;

    }

    spHenchPig SpriteFactory::PigCannoneer(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, spScreenPlay& sp)
    {
        spPigCannon ret = std::make_shared<pnk::PigCannon>(so, is);
        ret->setTypeNum(ST_PIG_CANNON);

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

        ret->setTransform(blit::SpriteTransform::HORIZONTAL);

        ret->init();

        attachBehaviourTree(sp, so, ret);

        return ret;
    }

    // HACK, REFACTOR
    spPigCannon SpriteFactory::PigCannoneerWCannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, spScreenPlay& sp)
    {
        spPigCannon ret = std::make_shared<pnk::PigCannon>(so, is);
        ret->setTypeNum(ST_PIG_CANNON);

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

        ret->setTransform(blit::SpriteTransform::HORIZONTAL);

        ret->init();

        attachBehaviourTree(sp, so, ret);

        std::shared_ptr<dang::Imagesheet> imgs = txtr.getImagesheet("character_cannonsnpigs");
        ret->_myCannon = CannonForCannoneer(txtr, so, imgs);

        return ret;
    }

    spCannon SpriteFactory::CannonForCannoneer(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spCannon ret = std::make_shared<pnk::Cannon>(so, is);

        ret->setTypeNum(ST_CANNON);
//        ret->_type_name = "cannon";
        ret->setPosX(so->x + 25);
        ret->setPosY(so->y);
        ret->setSize(32, 32);
        ret->setVisible(true);
        ret->setImgIndex(0);
        ret->setImagesheet(is);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "idling");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_shooting = txtr.getAnimation(is->getName(), "shooting");
        assert(ret->_anim_m_shooting != nullptr);
        ret->_anim_m_shooting->loops(1);
        ret->_anim_m_shooting->setFinishedCallback(std::bind(&Cannon::cannonHasFired, ret.get()));

        ret->init();

        ret->setTransform(blit::SpriteTransform::HORIZONTAL);

        return ret;
    }

    dang::spFullSpr SpriteFactory::Cannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spCannon ret = std::make_shared<pnk::Cannon>(so, is);
        ret->setTypeNum(ST_CANNON);

        ret->_anim_m_sleeping = txtr.getAnimation(is->getName(), "idling");
        assert(ret->_anim_m_sleeping != nullptr);
        ret->_anim_m_shooting = txtr.getAnimation(is->getName(), "shooting");
        assert(ret->_anim_m_shooting != nullptr);

        ret->init();

        ret->setTransform(blit::SpriteTransform::HORIZONTAL);

        return ret;
    }

    dang::spFullSpr SpriteFactory::Cannonmuzzle(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spMoodies ret = std::make_shared<pnk::Moodies>(so, is);
        ret->setTypeNum(ST_CANNON_MUZZLE);
        ret->setRigid(true);

        ret->_anim_m_standard = txtr.getAnimation(is->getName(), "muzzle_flash");
        assert(ret->_anim_m_standard != nullptr);
        ret->_anim_m_standard->loops(1);

        ret->init();

        return ret;
    }

    dang::spFullSpr SpriteFactory::PigPoof(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spMoodies ret = std::make_shared<pnk::Moodies>(so, is);
        ret->setTypeNum(ST_PIG_POOF);

        ret->_anim_m_standard = txtr.getAnimation(is->getName(), "poof");
        assert(ret->_anim_m_standard != nullptr);
        ret->_anim_m_standard->loops(1);

        ret->init();

        return ret;
    }


    spBubble SpriteFactory::Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, bool to_the_left, uint8_t num_bubble_loops)
    {
        spBubble ret = std::make_shared<pnk::Bubble>(so, is);
        ret->setTypeNum(ST_BUBBLE);
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

        ret->_anim_catched->loops(num_bubble_loops);

        ret->init();

        return ret;
    }

    spThrowies SpriteFactory::Crate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, bool to_the_left)
    {
        spThrowies ret = std::make_shared<pnk::Craties>(so, is);
        ret->setTypeNum(ST_FLYING_CRATE);
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

    spThrowies SpriteFactory::Bomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spBombies ret = std::make_shared<pnk::Bombies>(so, is);
        ret->setTypeNum(ST_FLYING_BOMB);

        ret->_anim_flying = txtr.getAnimation(is->getName(), "bomb_off");
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(-1);

        ret->_anim_on_fire = txtr.getAnimation(is->getName(), "bomb_on");
        assert(ret->_anim_on_fire != nullptr);
        ret->_anim_on_fire->loops(3);

        ret->init();

        return ret;
    }

    dang::spFullSpr SpriteFactory::Explosion(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spMoodiesThatHurt ret = std::make_shared<pnk::MoodiesThatHurt>(so, is);
        ret->setTypeNum(ST_EXPLOSION);

        ret->_anim_m_standard = txtr.getAnimation(is->getName(), "boom");
        assert(ret->_anim_m_standard != nullptr);
        ret->_anim_m_standard->loops(1);

        ret->init();

        return ret;
    }

    spThrowies SpriteFactory::Cannonball(dang::TmxExtruder &txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, bool to_the_left)
    {
        spThrowies ret = std::make_shared<pnk::Cannonball>(so, is);
        ret->setTypeNum(ST_FLYING_CANNONBALL);
        ret->_to_the_left = to_the_left;

        ret->_anim_flying = txtr.getAnimation(is->getName(), "cannonball");
        assert(ret->_anim_flying != nullptr);
        ret->_anim_flying->loops(-1);

        ret->init();

        return ret;
    }

    void SpriteFactory::attachBehaviourTree(const spScreenPlay& sp, const dang::tmx_spriteobject* so, const dang::spFullSpr& cs)
    {
        if (so->bt.length() > 0)
        {
            std::shared_ptr<dang::NTree> ntr = sp->_bt[so->bt];
            if (ntr != nullptr)
            {
                cs->setNTreeState(std::make_shared<dang::NTreeState>(ntr));
            }
            else
            {
                std::printf("ERROR: could not find behaviour tree %s\n", so->bt.c_str());
            }
        }
    }


}
