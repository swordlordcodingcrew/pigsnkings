// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <cassert>
#include <iostream>
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
#include "src/actors/throwies/Bombies.h"
#include "src/actors/throwies/Bubble.h"
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
#include "rsrc/gfx/bubble.png.h"
#include "rsrc/gfx/items.png.h"
#include "rsrc/gfx/king.png.h"
#include "rsrc/gfx/castle_decoration_tiles.png.h"
#include "rsrc/gfx/pig.png.h"
#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/gfx/hud_ui.png.h"
#include "rsrc/level_1.tmx.hpp"

#include <bt/bt.hpp>


namespace pnk
{
    std::shared_ptr<GameState> GSPlay::update(dang::Gear &gear, uint32_t time)
    {
//        blit::debugf("play updating\r\n");

#ifdef PNK_DEBUG
        if (_last_time + 1000 < time)
        {
            _last_time = time;
            std::cout << "update check" << std::endl;
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

//        blit::debugf("play updated\r\n");

        return GameState::_gs_play;
    }

    void GSPlay::createBehaviourTrees(dang::Gear& gear)
    {
        auto tr = dang::Builder{}
                .sequence()
                .leaf(dang::Sprite::BTLoiter)
                .leaf(dang::Sprite::BTIsHeroAround)
                .leaf(PigCrate::BTHideInCrate)
                .inverter()
                .leaf(dang::EnemiesAroundChecker{}) // Passing functor
                .end()
                .void_leaf(&dang::Sprite::eat_food) // Void member function
                .end()
                .build();

        /*
                auto tr = dang::Builder{}
                .sequence()
                .leaf([](std::shared_ptr<dang::Sprite> s) -> dang::BTNodeStatus { // Passing a lambda
                    return s->is_hungry ? dang::BTNodeStatus::SUCCESS : dang::BTNodeStatus::FAILURE;
                })
                .leaf(dang::Sprite::BTLoiter) // Passed a member function pointer
                .leaf([](std::shared_ptr<dang::Sprite> s) { return dang::BTNodeStatus::RUNNING; })
                .inverter()
                .leaf(dang::EnemiesAroundChecker{}) // Passing functor
                .end()
                .void_leaf(&dang::Sprite::eat_food) // Void member function
                .end()
                .build();

         */
        std::shared_ptr<dang::BehaviourTree> tree = std::make_shared<dang::BehaviourTree>(tr);

        // TODO: change tree to move(tree) within addbehaviourtree...? rename to assign or moveto?
        gear.addBehaviourTree("insanity", tree);

        //auto ts = gear._tree->make_state();
        //dang::TreeState ts {tree};
        //_spr_hero->_btTreeState = std::make_shared<dang::TreeState>(ts);
    }

    void GSPlay::enter(dang::Gear &gear, uint32_t time)
    {
        std::cout << "enter enter()" << std::endl;

        _last_time = 0;

        blit::debugf("entered\r\n");

        PigsnKings::playMod(gocryogo_mod, gocryogo_mod_length);

        blit::debugf("choose level\r\n");

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

        blit::debugf("extruded\r\n");

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

        std::cout << "test: " << _screenplay->_acts[0]._extent_pixels.w << std::endl;
        blit::debugf("imagesheet\r\n");

        // init imagesheets
        txtr.getImagesheets();

        blit::debugf("tile layer\r\n");

        // create background Tilelayer
        txtr.getTileLayer(_screenplay->_l_bg_name, true);

        blit::debugf("mood layer\r\n");

        // create mood Tilelayer
        if (!_screenplay->_l_mood_name.empty()) txtr.getSpriteLayer(_screenplay->_l_mood_name, true, true);

        blit::debugf("collision sprite layer\r\n");

        // create Spritelayer with collision detection
        _csl = txtr.getCollisionSpriteLayer(_screenplay->_l_obj_name, false, true);

        blit::debugf("sprite objects\r\n");

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
                else
                {
                        std::cerr << "sprite type unknown. Id=" << so->id << ", type=" << so->type << std::endl;
                }
            }
        }

        blit::debugf("fg layer\r\n");

        // create foreground layer
        txtr.getSpriteLayer(_screenplay->_l_fg_name, true, true);

        blit::debugf("hud layer\r\n");

        // create HUD layer
        spHUDLayer hudl = std::make_shared<HUDLayer>();
        if (!_screenplay->_l_hud_name.empty()) txtr.fillHUDLayer(hudl, _screenplay->_l_hud_name, true, true);

        blit::debugf("change room\r\n");

        // choose room acc. to prefs
        _active_act_index = _pnk._gamestate.active_room - 1;
        changeRoom(_pnk._gamestate.active_room, true);

        blit::debugf("viewport\r\n");

        // set viewport to active room
        updateVpPos();
        gear.setViewportPos(_vp_pos - dang::Vector2F(160, 120));


        blit::debugf("callbacks\r\n");

        // add event callback
        std::function<void (dang::Event&)> func = std::bind(&GSPlay::gameEventReceived, this, std::placeholders::_1);
        _sub_ref = _pnk._dispatcher.registerSubscriber(func, EF_GAME);

        blit::debugf("entered, let the games begin\r\n");
        std::cout << "exit enter()" << std::endl;
    }

    void GSPlay::exit(dang::Gear &gear, uint32_t time)
    {
        std::cout << "enter exit()" << std::endl;

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

        PigsnKings::stopMod();

//         _pnk._prefs.active_room = _active_act_index;

        std::cout << "exit exit()" << std::endl;
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
                std::cout << "remove sprite event" << std::endl;
#endif
            }
            else
            {
                // TODO if it is stale, we should retry? or wait? or...
                std::cerr << "attempted to remove stale sprite" << std::endl;
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
        {}

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

        switch(pe._payload)
        {
            case SpriteFactory::TN_PIG_NORMAL:
                health -= 30;
                break;
            case SpriteFactory::TN_PIG_BOMB:
                health -= 35;
                break;
            case SpriteFactory::TN_PIG_BOX:
                health -= 35;
                break;
            case SpriteFactory::TN_FLYING_BOMB:
                health -= 10;
                break;
            case SpriteFactory::TN_FLYING_CRATE:
                health -= 20;
                break;
            case SpriteFactory::TN_FLYING_CANNONBALL:
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
            PigsnKings::playSfx(king_damage_22050, king_damage_22050_length);
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

        PigsnKings::playSfx(lifelost_22050_mono, lifelost_22050_mono_length);
    }

    void GSPlay::handleRewardCollected(PnkEvent& pe)
    {
        switch (pe._payload)
        {
            case SpriteFactory::TN_COIN_SILVER:
                addScore(10);
                break;
            case SpriteFactory::TN_COIN_GOLD:
                addScore(50);
                break;
            case SpriteFactory::TN_GEM_BLUE:
                addScore(30);
                break;
            case SpriteFactory::TN_GEM_GREEN:
                addScore(60);
                break;
            case SpriteFactory::TN_GEM_RED:
                addScore(100);
                break;
            case SpriteFactory::TN_POTION_BLUE:
                addHealth(1);
                break;
            case SpriteFactory::TN_POTION_RED:
                addHealth(5);
                break;
            case SpriteFactory::TN_POTION_GREEN:
                addHealth(20);
                break;
            case SpriteFactory::TN_PIG_REWARD:
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
        PigsnKings::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length);
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
        PigsnKings::playSfx(health_22050_mono, health_22050_mono_length);
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

