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
        ret->_anim_m_wait = txtr.getAnimation(is, "wait", dang::Ease::Linear, -1, false, 2000);
        // walk animation
        ret->_anim_m_walk = txtr.getAnimation(is, "walk");
        // jump animation
        ret->_anim_m_jump = txtr.getAnimation(is, "jump");
        // on air (= not touching the ground) 'animation'
        ret->_anim_m_on_air = txtr.getAnimation(is, "on_air");
        // bubble animation
        ret->_anim_bubble = txtr.getAnimation(is, "bubble", dang::Ease::Linear, 1);
        ret->_anim_bubble->easeFunction(&dang::Ease::OutQuad);
        // entering & hit game animation
        ret->_anim_s_blink = txtr.getAnimation(is, "blink");
        // life lost animation
        ret->_anim_s_life_lost = txtr.getAnimation(is, "life_lost", dang::Ease::Linear, 1);

        ret->activateState();
        return ret;
    }

    spBoss SpriteFactory::Boss(dang::TmxExtruder &txtr, const dang::tmx_spriteobject *so, const std::unordered_map<std::string, dang::spImagesheet> &iss, spScreenPlay& sp)
    {
        dang::spImagesheet is = iss.at(so->tileset);

        assert(is != nullptr);
        spBoss ret = std::make_shared<pnk::PigBoss>(so, is);
        ret->setTypeNum(ST_PIG_BOSS);

        ret->_anim_m_sleeping = txtr.getAnimation(is, "sleeping", dang::Ease::Linear, -1, false, 300);
        ret->_anim_m_running = txtr.getAnimation(is, "running");
        ret->_anim_m_landing = txtr.getAnimation(is, "landing", dang::Ease::Linear, 1);
        ret->_anim_m_jumping = txtr.getAnimation(is, "jumping", dang::Ease::Linear, 1);
        ret->_anim_m_hit = txtr.getAnimation(is, "hit", dang::Ease::Linear, 2);
        ret->_anim_m_die = txtr.getAnimation(is, "die", dang::Ease::Linear, 1);

        ret->_anim_m_recovering = std::make_shared<dang::TwAnim>(*(ret->_anim_m_sleeping));
        ret->_anim_m_recovering->duration(300);
        ret->_anim_m_recovering->delay(0);

        ret->initBT(getBT(sp, so));

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

        ret->_anim_m_sleeping = txtr.getAnimation(is, "sleeping", dang::Ease::Linear, -1, false, dang::Rand::get(uint32_t(1000), uint32_t(2000)));
        ret->_anim_m_loitering = txtr.getAnimation(is, "loitering");
        ret->_anim_m_bubbling = txtr.getAnimation(is, "bubbling");

        ret->initBT(getBT(sp, so), std::make_shared<dang::NTreeState>(sp->_bt["berserk"]));

        return ret;
    }

    // it is a wooden crate.. for making buses out of them
    spHenchPig SpriteFactory::PigCrate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::unordered_map<std::string, dang::spImagesheet> &iss, spScreenPlay& sp)
    {
        dang::spImagesheet is = iss.at(so->tileset);

        spHenchPig ret = std::make_shared<pnk::PigCrate>(so, is);
        ret->setTypeNum(ST_PIG_CRATE);

        ret->_anim_m_sleeping = txtr.getAnimation(is, "sleeping", dang::Ease::Linear, -1, false, dang::Rand::get(uint32_t(1000), uint32_t(2000)));
        ret->_anim_m_loitering = txtr.getAnimation(is, "loitering");
        ret->_anim_m_bubbling = txtr.getAnimation(is, "bubbling");
        ret->_anim_m_picking_up = txtr.getAnimation(is, "picking_up");
        ret->_anim_m_throwing = txtr.getAnimation(is, "throwing", dang::Ease::Linear, 0);

        // animations without crate
        is = iss.at("gfx_pig");
        ret->_anim_alt_sleeping = txtr.getAnimation(is, "sleeping", dang::Ease::Linear, -1, false, dang::Rand::get(uint32_t(1000), uint32_t(2000)));
        ret->_anim_alt_loitering = txtr.getAnimation(is, "loitering");

        ret->initBT(getBT(sp, so), std::make_shared<dang::NTreeState>(sp->_bt["berserk"]));
/*        attachBehaviourTree(sp, so, ret);

        dang::spNTree t = sp->_bt["berserk"];
        if (t != nullptr)
        {
            ret->setBTSBerserk(std::make_shared<dang::NTreeState>(t));
        }

        ret->init();
*/
        return ret;
    }

    spHenchPig SpriteFactory::PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::unordered_map<std::string, dang::spImagesheet> &iss, spScreenPlay& sp)
    {
        dang::spImagesheet is = iss.at(so->tileset);

        spHenchPig ret = std::make_shared<pnk::PigBomb>(so, is);
        ret->setTypeNum(ST_PIG_BOMB);

        ret->_anim_m_sleeping = txtr.getAnimation(is, "sleeping", dang::Ease::Linear, -1, false, dang::Rand::get(uint32_t(1000), uint32_t(2000)));
        ret->_anim_m_loitering = txtr.getAnimation(is, "loitering");
        ret->_anim_m_bubbling = txtr.getAnimation(is, "bubbling");
        ret->_anim_m_picking_up = txtr.getAnimation(is, "picking_up");
        ret->_anim_m_throwing = txtr.getAnimation(is, "throwing", dang::Ease::Linear, 0);

        // animations without bomb
        is = iss.at("gfx_pig");
        ret->_anim_alt_sleeping = txtr.getAnimation(is, "sleeping", dang::Ease::Linear, -1, false, dang::Rand::get(uint32_t(1000), uint32_t(2000)));
        ret->_anim_alt_loitering = txtr.getAnimation(is, "loitering");

        ret->initBT(getBT(sp, so), std::make_shared<dang::NTreeState>(sp->_bt["berserk"]));
/*        attachBehaviourTree(sp, so, ret);

        dang::spNTree t = sp->_bt["berserk"];
        if (t != nullptr)
        {
            ret->setBTSBerserk(std::make_shared<dang::NTreeState>(t));
        }

        ret->init();
*/
        return ret;

    }

    spPigCannon SpriteFactory::PigCannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, spScreenPlay& sp)
    {
        spPigCannon ret = std::make_shared<pnk::PigCannon>(so, is);
        ret->setTypeNum(ST_PIG_CANNON);

        ret->_anim_m_sleeping = txtr.getAnimation(is, "sleeping", dang::Ease::Linear, -1, false, dang::Rand::get(uint32_t(1000), uint32_t(2000)));
        ret->_anim_m_loitering = txtr.getAnimation(is, "loitering");
        ret->_anim_m_picking_up = txtr.getAnimation(is, "lighting_match", dang::Ease::Linear, 1);
        ret->_anim_m_picking_up->setFinishedCallback(std::bind(&PigCannon::matchLit, ret.get()));
        ret->_anim_m_match_lit = txtr.getAnimation(is, "match_lit", dang::Ease::Linear, 5);
        ret->_anim_m_match_lit->setFinishedCallback(std::bind(&PigCannon::lightingCannon, ret.get()));
        ret->_anim_m_throwing = txtr.getAnimation(is, "lighting_cannon", dang::Ease::Linear, 2);
        ret->_anim_m_throwing->setFinishedCallback(std::bind(&PigCannon::cannonIsLit, ret.get()));

        ret->initBT(getBT(sp, so), std::make_shared<dang::NTreeState>(sp->_bt["berserk"]));
//        attachBehaviourTree(sp, so, ret);

//        ret->init();

        // now the cannon
        std::shared_ptr<dang::Imagesheet> imgs = txtr.getImagesheet("character_cannonsnpigs");
        dang::tmx_spriteobject cso {
            so->id,
            "",
            "cannon",
            ret->getTransform() == blit::HORIZONTAL ? 25 : -25,
            0,
            32,
            32,
            true,
            "character_cannonsnpigs",
            0,
            "",
            so->z_order,
            so->transform
        };


        spCannon can = std::make_shared<pnk::Cannon>(&cso, imgs);
        can->setTypeNum(ST_CANNON);

        can->_anim_m_sleeping = txtr.getAnimation(imgs, "idling");
        can->_anim_m_shooting = txtr.getAnimation(imgs, "shooting", dang::Ease::Linear, 1);
        can->_anim_m_shooting->setFinishedCallback(std::bind(&Cannon::cannonHasFired, can.get()));

        can->init();

        ret->addSpriteObject(can);

        return ret;

    }


    dang::spFullSpr SpriteFactory::Cannonmuzzle(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spMoodies ret = std::make_shared<pnk::Moodies>(so, is);
        ret->setTypeNum(ST_CANNON_MUZZLE);
        ret->setRigid(true);

        ret->_anim_m_standard = txtr.getAnimation(is, "muzzle_flash", dang::Ease::Linear, 1);
        ret->setAnimation(ret->_anim_m_standard);

        return ret;
    }

    dang::spFullSpr SpriteFactory::PigPoof(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spMoodies ret = std::make_shared<pnk::Moodies>(so, is);
        ret->setTypeNum(ST_PIG_POOF);

        ret->_anim_m_standard = txtr.getAnimation(is, "poof", dang::Ease::Linear, 1);
        ret->setAnimation(ret->_anim_m_standard);

        return ret;
    }


    spBubble SpriteFactory::Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, bool to_the_left, uint8_t num_bubble_loops)
    {
        spBubble ret = std::make_shared<pnk::Bubble>(so, is);
        ret->setTypeNum(ST_BUBBLE);
        ret->_to_the_left = to_the_left;

        // bubble grows
        ret->_anim_blow = txtr.getAnimation(is, "bubble_blow", dang::Ease::OutQuad, 1);
        // bubble bobbles
        ret->_anim_bobble = txtr.getAnimation(is, "bubble_bobble", dang::Ease::Linear, 3);
        // bubble poofs
        ret->_anim_poof = txtr.getAnimation(is, "bubble_poof", dang::Ease::Linear, 1);
        // bobbling with catched enemy
        ret->_anim_catched = std::make_shared<dang::TwAnim>(*(ret->_anim_bobble));
        ret->_anim_catched->loops(num_bubble_loops);

        ret->init();

        return ret;
    }

    spCraties SpriteFactory::Crate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, bool to_the_left)
    {
        spCraties ret = std::make_shared<pnk::Craties>(so, is);
        ret->setTypeNum(ST_FLYING_CRATE);
        ret->_to_the_left = to_the_left;

        // flying crates
        ret->_anim_flying = txtr.getAnimation(is, "crate");
        // crate destruction
        ret->_anim_destruction = txtr.getAnimation(is, "hit", dang::Ease::Linear, 1);

        ret->setAnimation(ret->_anim_flying);

        return ret;
    }

    spBombies SpriteFactory::Bomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spBombies ret = std::make_shared<pnk::Bombies>(so, is);
        ret->setTypeNum(ST_FLYING_BOMB);

        ret->_anim_flying = txtr.getAnimation(is, "bomb_off");
        ret->_anim_on_fire = txtr.getAnimation(is, "bomb_on", dang::Ease::Linear, 3);

        ret->setAnimation(ret->_anim_flying);

        return ret;
    }

    spCannonball SpriteFactory::Cannonball(dang::TmxExtruder &txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, bool to_the_left)
    {
        spCannonball ret = std::make_shared<pnk::Cannonball>(so, is);
        ret->setTypeNum(ST_FLYING_CANNONBALL);
        ret->_to_the_left = to_the_left;
        ret->_anim_flying = txtr.getAnimation(is, "cannonball");
        ret->setAnimation(ret->_anim_flying);

        return ret;
    }


    spCraties SpriteFactory::CrateFromProto(const spCraties& proto, const dang::Vector2F& pos, bool to_the_left)
    {
        assert(proto != nullptr);
        spCraties ret = std::make_shared<Craties>(*proto);
        ret->setAnimation(ret->_anim_flying);
        ret->setPos(pos);
        ret->_to_the_left = to_the_left;
        return ret;
    }

    spBombies SpriteFactory::BombFromProto(const spBombies& proto, const dang::Vector2F& pos, bool to_the_left)
    {
        assert(proto != nullptr);
        spBombies ret = std::make_shared<Bombies>(*proto);
        ret->setAnimation(ret->_anim_flying);
        ret->setPos(pos);
        ret->_to_the_left = to_the_left;
        return ret;
    }

    spCannonball SpriteFactory::CannonballFromProto(const spCannonball& proto, const dang::Vector2F& pos, bool to_the_left)
    {
        assert(proto != nullptr);
        spCannonball ret = std::make_shared<pnk::Cannonball>(*proto);
        ret->setAnimation(ret->_anim_flying);
        ret->setPosX(pos.x);
        ret->setPosY(pos.y + 6);
        ret->setVelX(to_the_left ? -20 : 20);
        ret->_to_the_left = to_the_left;
        return ret;
    }

    spMoodies SpriteFactory::CannonmuzzleFromProto(const spMoodies& proto, const dang::Vector2F& pos, bool to_the_left)
    {
        assert(proto != nullptr);
        spMoodies ret = std::make_shared<Moodies>(*proto);

        ret->setPosY(pos.y);
        ret->setPosX(to_the_left ? pos.x - 10 : pos.x + 10);
        ret->setTransform(to_the_left ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE);
        ret->setZOrder(100);
        ret->init();
        ret->_anim_m_standard->setFinishedCallback(std::bind(&Moodies::markRemove, ret.get()));

        return ret;
    }



    dang::spFullSpr SpriteFactory::Explosion(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is)
    {
        spMoodiesThatHurt ret = std::make_shared<pnk::MoodiesThatHurt>(so, is);
        ret->setTypeNum(ST_EXPLOSION);

        ret->_anim_m_standard = txtr.getAnimation(is, "boom", dang::Ease::Linear, 1);
        ret->setAnimation(ret->_anim_m_standard);

//        ret->init();

        return ret;
    }


    /*    void SpriteFactory::attachBehaviourTree(const spScreenPlay& sp, const dang::tmx_spriteobject* so, const dang::spFullSpr& cs)
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
*/
    dang::spNTreeState SpriteFactory::getBT(const spScreenPlay &sp, const dang::tmx_spriteobject *so)
    {
        if (so->bt.length() > 0)
        {
            std::shared_ptr<dang::NTree> ntr = sp->_bt[so->bt];
            if (ntr != nullptr)
            {
                return std::make_shared<dang::NTreeState>(ntr);
            }
            else
            {
                std::printf("ERROR: could not find behaviour tree %s\n", so->bt.c_str());
            }
        }
        return nullptr;

    }


}
