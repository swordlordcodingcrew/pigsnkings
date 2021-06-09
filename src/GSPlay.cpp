// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <cassert>
#include <memory>

#include <Gear.hpp>
#include <Imagesheet.hpp>
#include <Sprite.hpp>
#include <SpriteLayer.hpp>
#include <TileLayer.hpp>
#include <Layer.hpp>
#include <tween/Ease.hpp>
#include <tween/TwAnim.hpp>
#include <CollisionSprite.hpp>
#include <path/SceneGraph.hpp>
#include <path/Waypoint.hpp>

#include "src/actors/hero/Hero.h"
#include "src/actors/npc/Enemy.h"
#include "src/actors/npc/HenchPig.h"
#include "src/actors/npc/PigCrate.h"
#include "src/actors/npc/PigCannon.h"
#include "src/actors/throwies/Bombies.h"
#include "src/actors/throwies/Bubble.h"
#include "src/actors/throwies/Cannonball.h"
#include "src/actors/throwies/Throwies.h"
#include "src/actors/throwies/Craties.h"
#include "src/actors/others/Moodies.h"

#include "pnk_globals.h"
#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "ScreenPlay.h"
#include "SpriteFactory.hpp"
#include "GSPlay.h"
#include "GSHome.h"
#include "HUDLayer.hpp"

#include "tracks/gocryogo.h"
#include "sfx/coin_22050_mono.h"
#include "sfx/king_damage_22050.h"
#include "sfx/health_22050_mono.h"
#include "sfx/lifelost_22050_mono.h"

#include "rsrc/gfx/pig_bomb.png.h"
#include "rsrc/gfx/pig_crate.png.h"
#include "rsrc/gfx/cannons_n_pigs.png.h"
#include "rsrc/gfx/bubble.png.h"
#include "rsrc/gfx/items.png.h"
#include "rsrc/gfx/king.png.h"
#include "rsrc/gfx/castle_decoration_tiles.png.h"
#include "rsrc/gfx/pig.png.h"
#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/gfx/hud_ui.png.h"
#include "rsrc/level_1.tmx.hpp"

#include <bt/bt.hpp>
#include <malloc.h>
#include <libs/DANG/src/snd/SndGear.hpp>

#ifdef TARGET_32BLIT_HW
/*
#include "32blit.hpp"
#include <malloc.h>
extern "C"
{
    extern char _sbss, _end, __ltdc_start;
}
 */
#include "32blit.hpp"
#include <malloc.h>
#include "../../../fonts/hud_font_small.h"
extern char _sbss, _end, __ltdc_start;
#endif

namespace pnk
{
    std::shared_ptr<GameState> GSPlay::update(dang::Gear &gear, uint32_t time)
    {
//        DEBUG_PRINT("GSPlay: play updatisng\n");

#ifdef PNK_DEBUG
        if (_last_time + 1000 < time)
        {
            _last_time = time;
            DEBUG_PRINT("GSPlay: update check\n");
        }
#endif
        if (blit::buttons.pressed & blit::Button::MENU)
        {
            return GameState::_gs_home;
        }
        else if (blit::buttons.pressed & blit::Button::HOME)
        {
//            return GameState::_gs_prefs;
        }

        updateVpPos();
        if (_warp)
        {
            dang::Vector2F pos;
            pos.x = _vp_pos.x - 160;
            pos.y = _vp_pos.y - 120;

            gear.setViewportPos(pos);
            _warp = false;
        }
        else
        {
            gear.follow(_vp_pos);
        }

//        DEBUG_PRINT("GSPlay: play updated\n");

        return GameState::_gs_play;
    }

    void GSPlay::createBehaviourTrees(dang::Gear& gear)
    {
        auto trInsan = dang::Builder{}
                .sequence()
                    .leaf(dang::Sprite::BTIsHeroAround)
                    .leaf(dang::Sprite::BTLoiter)
                    .leaf(PigCrate::BTThrowCrate)
                .end()
                .build();

        gear.addBehaviourTree("insanity", std::make_shared<dang::BehaviourTree>(trInsan));

        auto treeCratePig = dang::Builder{}
                .sequence()
                    .leaf(dang::Sprite::BTLoiter)
                    .leaf(dang::Sprite::BTIsHeroAround)
                    .leaf(PigCrate::BTHideInCrate)
                    .void_leaf(&dang::Sprite::eat_food) // Void member function
                .end()
                .build();

        gear.addBehaviourTree("cratepig", std::make_shared<dang::BehaviourTree>(treeCratePig));

        auto treeTestPath = dang::Builder{}
                .sequence()
                    .leaf(Enemy::BTsetDestinationWaypointTestFunc)
                    .leaf(Enemy::BTcheckPathCompleted)
                .end()
                .build();

        gear.addBehaviourTree("test_path", std::make_shared<dang::BehaviourTree>(treeTestPath));

        auto treeRandomLoiter = dang::Builder{}
                .sequence()
                .leaf(Enemy::BTsetDestinationBombDepot)
                .leaf(Enemy::BTcheckPathCompleted)
                .leaf(Enemy::BTrandomNextWaypoint)
                .leaf(Enemy::BTcheckPathCompleted)
                .leaf(Enemy::BTrandomNextWaypoint)
                .leaf(Enemy::BTcheckPathCompleted)
                .leaf(Enemy::BTrandomNextWaypoint)
                .leaf(Enemy::BTcheckPathCompleted)
                .leaf(Enemy::BTrandomNextWaypoint)
                .leaf(Enemy::BTcheckPathCompleted)
                .end()
                .build();

        gear.addBehaviourTree("loiter", std::make_shared<dang::BehaviourTree>(treeRandomLoiter));
    }

    void GSPlay::enter(dang::Gear &gear, uint32_t time)
    {
        DEBUG_PRINT("GSPlay: entered (%d)\n", mallinfo().uordblks);

#ifdef TARGET_32BLIT_HW

        // memory stats

        auto static_used = &_end - &_sbss;
        auto heap_total = &__ltdc_start - &_end;
        auto total_ram = static_used + heap_total;
        auto heap_used = mallinfo().uordblks;


        blit::debugf("Mem: %i + %i (%i) = %i\r\n", static_used, heap_used, heap_total, total_ram);

#endif

        _last_time = 0;

        dang::SndGear::playMod(gocryogo_mod, gocryogo_mod_length, _pnk._prefs.volume_track);

        DEBUG_PRINT("GSPlay: choose level (%d)\n", mallinfo().uordblks);

        // choose level acc. to pnk
        switch(_pnk._gamestate.active_level)
        {
            case 1:
            default:
                _screenplay = std::make_shared<L1SP>();
                _tmx = &level_1_level;
                break;
        }

        // create behaviour trees
        // TODO should probably move up into the level specific setup
        createBehaviourTrees(gear);

        dang::TmxExtruder txtr(_tmx, &gear);

        DEBUG_PRINT("GSPlay: extruded (%d)\n", mallinfo().uordblks);

        _last_time = blit::now();

        dang::RectF vp = {0, 0, 320, 240};
        gear.initLevel(_tmx, vp);
        gear.setActiveWorldSize(vp.w + 16, vp.h + 16);

        // fill up screenplay
        for (ScreenPlay::act& room : _screenplay->_acts)
        {
            room._extent_pixels.x = room._extent.x * _tmx->w->tileWidth;
            room._extent_pixels.y = room._extent.y * _tmx->w->tileHeight;
            room._extent_pixels.w = room._extent.w * _tmx->w->tileWidth;
            room._extent_pixels.h = room._extent.h * _tmx->w->tileHeight;

            // add scenegraph
            room._scene_graph = txtr.createPaths(room._extent_pixels);
        }

        DEBUG_PRINT("GSPlay: imagesheet (%d)\n", mallinfo().uordblks);

        // init imagesheets
        txtr.getImagesheets();

        DEBUG_PRINT("GSPlay: tile layer (%d)\n", mallinfo().uordblks);

        // create background Tilelayer
        txtr.getTileLayer(_screenplay->_l_bg_name, true);

        DEBUG_PRINT("GSPlay: mood layer (%d)\n", mallinfo().uordblks);

        // create mood Tilelayer
        if (!_screenplay->_l_mood_name.empty()) txtr.getSpriteLayer(_screenplay->_l_mood_name, true, true);

        DEBUG_PRINT("GSPlay: collision sprite layer (%d)\n", mallinfo().uordblks);

        // create Spritelayer with collision detection
        _csl = txtr.getCollisionSpriteLayer(_screenplay->_l_obj_name, false, true);

        DEBUG_PRINT("GSPlay: sprite objects (%d)\n", mallinfo().uordblks);

#ifdef TARGET_32BLIT_HW

        // memory stats
        blit::debugf("Mem: %i + %i (%i) = %i\n", static_used, mallinfo().uordblks, heap_total, total_ram);

#endif

        // create sprites
        for (size_t j = 0; j < _csl->_tmx_layer->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = _csl->_tmx_layer->spriteobjects + j;

            spImagesheet is = gear.getImagesheet(so->tileset);
            spCollisionSprite spr = nullptr;
            if      (so->type == SpriteFactory::T_HOTRECT)           { spr = SpriteFactory::Hotrect(so); }
            else if (so->type == SpriteFactory::T_HOTRECT_PLATFORM)  { spr = SpriteFactory::HotrectPlatform(so); }
            else if (so->type == SpriteFactory::T_ROOM_TRIGGER)      { spr = SpriteFactory::RoomTrigger(so); }
            else if (so->type == SpriteFactory::T_WARP_ROOM_TRIGGER) { spr = SpriteFactory::WarpRoomTrigger(so); }
            else if (so->type == SpriteFactory::T_PIG_NORMAL)        { spr = SpriteFactory::NormalPig(txtr, so, is, _screenplay); }
            else if (so->type == SpriteFactory::T_PIG_BOMB)          { spr = SpriteFactory::PigBomb(txtr, so, is, _screenplay); }
            else if (so->type == SpriteFactory::T_PIG_BOX)           { spr = SpriteFactory::PigCrate(txtr, so, is, _screenplay); }
            else if (so->type == SpriteFactory::T_COIN_SILVER)       { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so->type == SpriteFactory::T_COIN_GOLD)         { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so->type == SpriteFactory::T_GEM_BLUE)          { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so->type == SpriteFactory::T_GEM_GREEN)         { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so->type == SpriteFactory::T_GEM_RED)           { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so->type == SpriteFactory::T_POTION_BLUE)       { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so->type == SpriteFactory::T_POTION_RED)        { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so->type == SpriteFactory::T_POTION_GREEN)      { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so->type == SpriteFactory::T_PIG_CANNON)
            {
                // TODO refactor, ugly hack
                // implicitly add the cannon and tell the cannoneer about
                auto cannoneer = SpriteFactory::PigCannoneerWCannon(txtr, so, is, _screenplay);
                _csl->addCollisionSprite(cannoneer->_myCannon);

                spr = cannoneer;
            }
            else if (so->type == SpriteFactory::T_KING)
            {
                _spr_hero = SpriteFactory::King(txtr, so, is);
                spr = _spr_hero;
            }

            if (spr != nullptr)
            {
                _csl->addCollisionSprite(spr);
            }
            else
            {
                if (so->type == SpriteFactory::T_BUBBLE_PROTO)
                {
                    spCollisionSprite sprc = SpriteFactory::Bubble(txtr, so, is, false);
                    assert(sprc != nullptr);
                    _hives["bubble"] = sprc;
                }
                else if (so->type == SpriteFactory::T_CRATE_PROTO)
                {
                    spCollisionSprite sprc = SpriteFactory::Crate(txtr, so, is, false);
                    assert(sprc != nullptr);
                    _hives["crate"] = sprc;
                }
                else if (so->type == SpriteFactory::T_BOMB_PROTO)
                {
                    spCollisionSprite sprc = SpriteFactory::Bomb(txtr, so, is, false);
                    assert(sprc != nullptr);
                    _hives["bomb"] = sprc;
                }
                else if (so->type == SpriteFactory::T_PIG_POOF_PROTO)
                {
                    spCollisionSprite sprc = SpriteFactory::PigPoof(txtr, so, is);
                    assert(sprc != nullptr);
                    _hives["poof"] = sprc;
                }
                else if (so->type == SpriteFactory::T_CANNONBALL_PROTO)
                {
                    spCollisionSprite sprc = SpriteFactory::Cannonball(txtr, so, is, false);
                    assert(sprc != nullptr);
                    _hives["cannonball"] = sprc;
                }
                else if (so->type == SpriteFactory::T_CANNONMUZZLE_PROTO)
                {
                    spCollisionSprite sprc = SpriteFactory::Cannonmuzzle(txtr, so, is);
                    assert(sprc != nullptr);
                    _hives["cannonmuzzle"] = sprc;
                }
                else
                {
                    DEBUG_PRINT("GSPlay: sprite type unknown. Id=%i, type=%s\n", so->id, so->type.c_str());
                }
            }

            DEBUG_PRINT("GSPlay: sprite %d of %d (%d)\n", j + 1, _csl->_tmx_layer->spriteobejcts_len, mallinfo().uordblks);

#ifdef TARGET_32BLIT_HW

            // memory stats
        DEBUG_PRINT("GSPlay: Mem: %i + %i (%i) = %i\n", static_used, mallinfo().uordblks, heap_total, total_ram);

#endif
        }

        DEBUG_PRINT("GSPlay: fg layer\n");

        // create foreground layer
        txtr.getSpriteLayer(_screenplay->_l_fg_name, true, true);

        DEBUG_PRINT("GSPlay: hud layer\n");

        // create HUD layer
        spHUDLayer hudl = std::make_shared<HUDLayer>();
        if (!_screenplay->_l_hud_name.empty()) txtr.fillHUDLayer(hudl, _screenplay->_l_hud_name, true, true);

        DEBUG_PRINT("GSPlay: change room\n");

        // choose room acc. to prefs
        _active_act_index = _pnk._gamestate.active_room - 1;
        changeRoom(_pnk._gamestate.active_room, true);

        DEBUG_PRINT("GSPlay: viewport\n");

        // set viewport to active room
        updateVpPos();
        gear.setViewportPos(_vp_pos - dang::Vector2F(160, 120));

        DEBUG_PRINT("GSPlay: callbacks\n");

        // add event callback
        std::function<void (dang::Event&)> func = std::bind(&GSPlay::gameEventReceived, this, std::placeholders::_1);
        _sub_ref = _pnk._dispatcher.registerSubscriber(func, EF_GAME);

        DEBUG_PRINT("GSPlay: entered, let the games begin\n");
    }

    void GSPlay::exit(dang::Gear &gear, uint32_t time)
    {
        DEBUG_PRINT("GSPlay: enter exit()\n");

        // remove callback
        _pnk._dispatcher.removeSubscriber(_sub_ref);

        _spr_hero.reset();
        _screenplay.reset();
        _csl.reset();
        _hives.clear();

        // remove images
        _pnk.removeImagesheets();

        // remove layers
        gear.removeLayers();

        // remove behaviour trees
        gear.removeBehaviourTrees();

        dang::SndGear::stopMod();

//         _pnk._prefs.active_room = _active_act_index;

        DEBUG_PRINT("GSPlay: exit exit()");
    }

/*    void GSPlay::initGameVars()
    {
        _pnk.initGameVars();
    }
*/
    void GSPlay::gameEventReceived(dang::Event &e)
    {
        PnkEvent& pe = static_cast<PnkEvent&>(e);
        if (pe._type == ETG_NEW_BUBBLE)
        {
            spBubble bub_proto = std::dynamic_pointer_cast<Bubble>(_hives["bubble"]);
            assert(bub_proto != nullptr);
            spBubble bub = std::make_shared<Bubble>(*bub_proto);
            bub->setPos(pe._pos);
            bub->_to_the_left = pe._to_the_left;
            bub->init();
            _csl->addCollisionSprite(bub);
        }
        else if (pe._type == ETG_REMOVE_SPRITE)
        {
            std::shared_ptr<dang::Sprite> spr = pe._spr.lock();
            if (spr != nullptr)
            {
                _csl->removeSprite(pe._spr.lock());
#ifdef PNK_DEBUG
                DEBUG_PRINT("GSPlay: remove sprite from layer\n");
#endif
            }
            else
            {
                // TODO if it is stale, we should retry? or wait? or...
                DEBUG_PRINT("GSPlay: CAUTION: attempted to remove sprite with shared_ptr = nullptr\n");
            }
        }
        else if (pe._type == ETG_NEW_THROWN_CRATE
                || pe._type == ETG_NEW_THROWN_BOMB
                || pe._type == ETG_NEW_FIRED_CANNON)
        {
            handleNewThrowie(pe);
        }
        else if (pe._type == ETG_NEW_POOF)
        {
            handleNewPoof(pe);
        }
        else if (pe._type == ETG_KING_HIT)
        {
            handleKingHealth(pe);
        }
        else if (pe._type == ETG_REWARD_HIT)
        {
            handleRewardCollected(pe);
        }
        else if (pe._type == ETG_CHANGE_ROOM)
        {
            if (pe._payload != _active_act_index)
            {
                changeRoom(pe._payload, false);
            }
        }
        else if (pe._type == ETG_WARP_ROOM)
        {
            if (pe._payload != _active_act_index)
            {
                changeRoom(pe._payload, true);
            }
        }
    }

    void GSPlay::handleNewThrowie(PnkEvent& pe)
    {
        if (pe._type == ETG_NEW_THROWN_CRATE)
        {
            spThrowies proto = std::dynamic_pointer_cast<Throwies>(_hives["crate"]);
            assert(proto != nullptr);
            spCraties crate = std::make_shared<Craties>(*proto);
            crate->setPos(pe._pos);
            crate->_to_the_left = pe._to_the_left;
            crate->init();
            _csl->addCollisionSprite(crate);
        }
        else if (pe._type == ETG_NEW_THROWN_BOMB)
        {
            spThrowies proto = std::dynamic_pointer_cast<Throwies>(_hives["bomb"]);
            assert(proto != nullptr);
            spBombies bomb = std::make_shared<Bombies>(*proto);
            bomb->setPos(pe._pos);
            bomb->_to_the_left = pe._to_the_left;
            bomb->init();
            _csl->addCollisionSprite(bomb);
        }
        else if (pe._type == ETG_NEW_FIRED_CANNON)
        {
            spThrowies proto = std::dynamic_pointer_cast<Throwies>(_hives["cannonball"]);
            assert(proto != nullptr);
            spCannonball ball = std::make_shared<Cannonball>(*proto);
            ball->setPosX(pe._pos.x);
            ball->setPosY(pe._pos.y + 6);
            ball->_to_the_left = pe._to_the_left;
            ball->setVelX(20);
            ball->init();
            _csl->addCollisionSprite(ball);

            spMoodies protoMood = std::dynamic_pointer_cast<Moodies>(_hives["cannonmuzzle"]);
            assert(protoMood != nullptr);
            spMoodies mood = std::make_shared<Moodies>(*protoMood);
            mood->setPos(pe._pos);
            mood->setPosX(pe._pos.x + 10);
            mood->_z_order = 100; // TODO make sure that zorder works
            mood->_transform = blit::SpriteTransform::HORIZONTAL;
            mood->init();
            mood->_anim_m_standard->setFinishedCallback([=]
                {
                    std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
                    e->_spr = mood;
                    pnk::_pnk._dispatcher.queueEvent(std::move(e));
                });

            _csl->addCollisionSprite(mood);
        }

    }

    void GSPlay::handleNewPoof(PnkEvent& pe)
    {
        spMoodies proto = std::dynamic_pointer_cast<Moodies>(_hives["poof"]);
        assert(proto != nullptr);
        spMoodies poof = std::make_shared<Moodies>(*proto);
        poof->setPos(pe._pos);
        poof->init();
        poof->_anim_m_standard->setFinishedCallback([=]
           {
               std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
               e->_spr = poof;
               pnk::_pnk._dispatcher.queueEvent(std::move(e));
           });

        _csl->addCollisionSprite(poof);
    }

    void GSPlay::handleKingHealth(PnkEvent& pe)
    {
        if(_pnk._prefs.invincible)
        {
            return;
        }

        // get current health (and yes, we want signed to go below 0!)
        int8_t health = _pnk._gamestate.health;

        switch(static_cast<dang::SpriteType>(pe._payload))
        {
            case dang::SpriteType::PIG_NORMAL:
                health -= 30;
                break;
            case dang::SpriteType::PIG_BOMB:
                health -= 35;
                break;
            case dang::SpriteType::PIG_BOX:
                health -= 35;
                break;
            case dang::SpriteType::FLYING_BOMB:
                health -= 10;
                break;
            case dang::SpriteType::FLYING_CRATE:
                health -= 20;
                break;
            case dang::SpriteType::FLYING_CANNONBALL:
                health -= 50;
                break;
        }

        if(health <= 0)
        {
            handleKingLoosesLife();
        }
        else
        {
            _pnk._gamestate.health = health;
            dang::SndGear::playSfx(king_damage_22050, king_damage_22050_length, _pnk._prefs.volume_sfx);
        }
    }

    void GSPlay::handleKingLoosesLife()
    {
        _pnk._gamestate.lives -= 1;

        if(_pnk._gamestate.lives <= 0)
        {
            // TODO GAME OVER
            _pnk._gamestate.lives = 3;
        }

        dang::Vector2F sp;
        dang::Vector2U restart_pos = _active_act->_passage_from[_active_act_index - 1];
        sp.x = (_active_act->_extent.x + restart_pos.x) * _tmx->w->tileWidth;
        sp.y = (_active_act->_extent.y + restart_pos.y) * _tmx->w->tileHeight;
        _spr_hero->lifeLost(sp);

        // TODO define MAXHEALTH
        _pnk._gamestate.health = 100;

        dang::SndGear::playSfx(lifelost_22050_mono, lifelost_22050_mono_length, _pnk._prefs.volume_sfx);
    }

    void GSPlay::handleRewardCollected(PnkEvent& pe)
    {
        switch (static_cast<dang::SpriteType>(pe._payload))
        {
            case dang::SpriteType::COIN_SILVER:
                addScore(10);
                break;
            case dang::SpriteType::COIN_GOLD:
                addScore(50);
                break;
            case dang::SpriteType::GEM_BLUE:
                addScore(30);
                break;
            case dang::SpriteType::GEM_GREEN:
                addScore(60);
                break;
            case dang::SpriteType::GEM_RED:
                addScore(100);
                break;
            case dang::SpriteType::POTION_BLUE:
                addHealth(1);
                break;
            case dang::SpriteType::POTION_RED:
                addHealth(5);
                break;
            case dang::SpriteType::POTION_GREEN:
                addHealth(20);
                break;
            case dang::SpriteType::PIG_REWARD:
                addScore(100);
                break;
                // Default gets nothing
            default:
                break;
        }
    }

    void GSPlay::addScore(uint8_t score)
    {
        _pnk._gamestate.score += score;
        dang::SndGear::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length, _pnk._prefs.volume_sfx);
    }

    void GSPlay::addHealth(uint8_t healthGain)
    {
        uint8_t h = _pnk._gamestate.health;

        h += healthGain;

        // TODO define MAXHEALTH
        if(h > 100)
        {
            h = 100;
        }

        _pnk._gamestate.health = h;
        dang::SndGear::playSfx(health_22050_mono, health_22050_mono_length, _pnk._prefs.volume_sfx);
    }


    void GSPlay::updateVpPos()
    {
        // viewport follows hero within room
        dang::Vector2F pos = _spr_hero->getPos() + _spr_hero->getSize() / 2.0f;

        if (pos.x < _active_act->_extent_pixels.left() + 160)
        {
            _vp_pos.x = _active_act->_extent_pixels.left() + 160;
        }
        else if (pos.x > _active_act->_extent_pixels.right() - 160)
        {
            _vp_pos.x = _active_act->_extent_pixels.right() - 160;
        }
        else
        {
            _vp_pos.x = pos.x;
        }

        if (pos.y < _active_act->_extent_pixels.top() + 120)
        {
            _vp_pos.y = _active_act->_extent_pixels.top() + 120;
        }
        else if (pos.y > _active_act->_extent_pixels.bottom() - 120)
        {
            _vp_pos.y = _active_act->_extent_pixels.bottom() - 120;
        }
        else
        {
            _vp_pos.y = pos.y;
        }
    }

    void GSPlay::changeRoom(int32_t room_nr, bool warp)
    {
        _active_act = &_screenplay->_acts[room_nr];

        if (warp)
        {
            dang::Vector2F sp;

            dang::Vector2U passage = _active_act->_passage_from[_active_act_index];
            sp.x = (_active_act->_extent.x + passage.x) * _tmx->w->tileWidth;
            sp.y = (_active_act->_extent.y + passage.y) * _tmx->w->tileHeight;
            _spr_hero->setPos(sp);
            _warp = true;
        }

        _active_act_index = room_nr;
        _pnk._gamestate.active_room = room_nr;
    }

}

