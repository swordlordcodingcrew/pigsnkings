// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game


#include "pnk_globals.h"
#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "SpriteFactory.hpp"
#include "GSPlay.h"
#include "GSHome.h"
#include "HUDLayer.hpp"
#include "TextLayer.hpp"
#include "actors/hero/Hero.h"
#include "actors/npc/Enemy.h"
#include "actors/npc/HenchPig.h"
#include "actors/npc/PigCrate.h"
#include "actors/npc/PigCannon.h"
#include "actors/npc/PigBoss.h"
#include "actors/throwies/Bombies.h"
#include "actors/throwies/Bubble.h"
#include "actors/throwies/Cannonball.h"
#include "actors/throwies/Throwies.h"
#include "actors/throwies/Craties.h"
#include "actors/others/Moodies.h"
#include "actors/others/MoodiesThatHurt.h"
#include "actors/others/Reward.h"
#include <src/actors/others/LevelTrigger.h>
#include "actors/others/Cannon.h"
#include "levels/Level1SP.hpp"
#include "levels/Level2SP.hpp"

#include "tracks/gocryogo.h"

#include "sfx/cannon_fire_22050_mono.h"
#include "sfx/boss_battle_22050_mono.h"
#include "sfx/bubble_blow_22050_mono.h"
#include "sfx/coin_22050_mono.h"
#include "sfx/king_damage_22050.h"
#include "sfx/health_22050_mono.h"
#include "sfx/lifelost_22050_mono.h"
#include "sfx/teleport_22050_mono.h"
#include "sfx/victory_22050_mono.h"

#include "rsrc/gfx/bomb.png.h"
#include "rsrc/gfx/pig_bomb.png.h"
#include "rsrc/gfx/pig_crate.png.h"
#include "rsrc/gfx/cannons_n_pigs.png.h"
#include "rsrc/gfx/bubble.png.h"
#include "rsrc/gfx/items.png.h"
#include "rsrc/gfx/king.png.h"
#include "rsrc/gfx/pig_king.png.h"
#include "rsrc/gfx/castle_decoration_tiles.png.h"
#include "rsrc/gfx/door.png.h"
#include "rsrc/gfx/pig.png.h"
#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/gfx/hud_ui.png.h"
#include "rsrc/level_1.tmx.hpp"
#include "rsrc/level_2.tmx.hpp"
#include "rsrc/game_strings.hpp"

#include <snd/SndGear.hpp>
#include <sfx/crate_explode_22050_mono.h>
#include <sfx/cheat_22050_mono.h>
#include <bt/NTree.h>
#include <bt/NTBuilder.h>
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

#include <malloc.h>
#include <cassert>
#include <memory>


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
    extern PigsnKings _pnk;

    using spHUDLayer = std::shared_ptr<HUDLayer>;

    std::shared_ptr<GameState> GSPlay::update(dang::Gear &gear, uint32_t time)
    {
//        DEBUG_PRINT("GSPlay: play updatisng\n");

        updateCheatKeyStream(blit::buttons.pressed);
        checkCheatActivation();

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

        loadLevel(_pnk._gamestate.active_level);

        DEBUG_PRINT("GSPlay: callbacks\n");

        // add event callback
        std::function<void (dang::Event&)> func = std::bind(&GSPlay::gameEventReceived, this, std::placeholders::_1);
        _sub_ref = _pnk._dispatcher.registerSubscriber(func, EF_GAME);

        DEBUG_PRINT("GSPlay: entered, let the games begin\n");

        // show starting text
        switch (_pnk._gamestate.active_level)
        {
            case 1:
            default:
                showInfoLayer(true, 10000, str_lvl1_intro);
                break;
            case 2:
                showInfoLayer(true, 10000, str_lvl2_intro);
                break;
        }
    }

    void GSPlay::exit(dang::Gear &gear, uint32_t time)
    {
        DEBUG_PRINT("GSPlay: enter exit()\n");

        // remove callback
        _pnk._dispatcher.removeSubscriber(_sub_ref);
        _sub_ref = 0;

        freeCurrentLevel();

        dang::SndGear::stopMod();

//         _pnk._prefs.active_room = _active_act_index;

        DEBUG_PRINT("GSPlay: exit exit()\n");
    }

    void GSPlay::loadLevel(int8_t level_nr)
    {
        DEBUG_PRINT("GSPlay: load level %d (%d)\n", level_nr, mallinfo().uordblks);

        // choose level acc. to pnk
        switch(level_nr)
        {
            case 1:
            default:
                _screenplay = std::make_shared<Level1SP>(*this);
                _tmx = &level_1_level;
                break;
            case 2:
                _screenplay = std::make_shared<Level2SP>(*this);
                _tmx = &level_2_level;
                break;
        }

        dang::Gear& gear = _pnk.getGear();

        // add level specific behaviour trees
        for (const auto& bthm : _screenplay->_bt)
        {
            gear.addNTree(bthm.first, bthm.second);
        }

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
            txtr.createSceneGraphs(room._extent_pixels, room._scene_graphs);
        }

        DEBUG_PRINT("GSPlay: imagesheet (%d)\n", mallinfo().uordblks);

        // init imagesheets
        txtr.getImagesheets();

        DEBUG_PRINT("GSPlay: tile layer (%d)\n", mallinfo().uordblks);

        // create background Tilelayer
        txtr.getTileLayer(_screenplay->_l_bg_name, true);

        DEBUG_PRINT("GSPlay: mood layer (%d)\n", mallinfo().uordblks);

        // create mood Tilelayer
        if (!_screenplay->_l_mood_name.empty())
        {
            dang::spSpriteLayer sl = txtr.getSpriteLayer(_screenplay->_l_mood_name, true, true, true);
        }

        DEBUG_PRINT("GSPlay: collision sprite layer (%d)\n", mallinfo().uordblks);

        // create Spritelayer with collision detection
        _csl = txtr.getCollisionSpriteLayer(_screenplay->_l_obj_name, false, true);

        DEBUG_PRINT("GSPlay: sprite objects (%d)\n", mallinfo().uordblks);

//#ifdef TARGET_32BLIT_HW

        // memory stats
        //blit::debugf("Mem: %i + %i (%i) = %i\n", static_used, mallinfo().uordblks, heap_total, total_ram);

//#endif

        // create sprites
        for (size_t j = 0; j < _csl->_tmx_layer->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = _csl->_tmx_layer->spriteobjects + j;

            spImagesheet is = gear.getImagesheet(so->tileset);
            std::unordered_map<std::string, spImagesheet> iss = gear.getImagesheets();
            spCollisionSprite spr = nullptr;
            if      (so->type == SpriteFactory::T_HOTRECT)           { spr = SpriteFactory::Hotrect(so); }
            else if (so->type == SpriteFactory::T_HOTRECT_PLATFORM)  { spr = SpriteFactory::HotrectPlatform(so); }
            else if (so->type == SpriteFactory::T_ROOM_TRIGGER)      { spr = SpriteFactory::RoomTrigger(so); }
            else if (so->type == SpriteFactory::T_WARP_ROOM_TRIGGER) { spr = SpriteFactory::WarpRoomTrigger(so); }
            else if (so->type == SpriteFactory::T_LEVEL_TRIGGER)     { spr = SpriteFactory::LevelTrigger(so); }
            else if (so->type == SpriteFactory::T_BOSSBATTLE_TRIGGER){ spr = SpriteFactory::BossbattleTrigger(so); }
            else if (so->type == SpriteFactory::T_PIG_NORMAL)        { spr = SpriteFactory::NormalPig(txtr, so, is, _screenplay); }
            else if (so->type == SpriteFactory::T_PIG_BOMB)          { spr = SpriteFactory::PigBomb(txtr, so, is, _screenplay); }
            else if (so->type == SpriteFactory::T_PIG_BOX)           { spr = SpriteFactory::PigCrate(txtr, so, iss, _screenplay); }
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
            else if (so->type == SpriteFactory::T_BOSS)
            {
                _spr_boss = SpriteFactory::Boss(txtr, so, is);
                spr = _spr_boss;
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
                    spCollisionSprite sprc = SpriteFactory::Bomb(txtr, so, is);
                    assert(sprc != nullptr);
                    _hives["bomb"] = sprc;
                }
                else if (so->type == SpriteFactory::T_EXPLOSION_PROTO)
                {
                    spCollisionSprite sprc = SpriteFactory::Explosion(txtr, so, is);
                    assert(sprc != nullptr);
                    _hives["explosion"] = sprc;
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
        //DEBUG_PRINT("GSPlay: Mem: %i + %i (%i) = %i\n", static_used, mallinfo().uordblks, heap_total, total_ram);

#endif
        }

        DEBUG_PRINT("GSPlay: fg layer\n");

        // create foreground layer
        txtr.getSpriteLayer(_screenplay->_l_fg_name, true, true, false);

        DEBUG_PRINT("GSPlay: hud layer\n");

        // create HUD layer
        spHUDLayer hudl = std::make_shared<HUDLayer>();
        if (!_screenplay->_l_hud_name.empty())
        {
            txtr.fillHUDLayer(hudl, _screenplay->_l_hud_name, true, true);

            // run through all sprites in HUD layer and check for the kings head (and make it change when user cheats)
            if(_pnk.isCheating())
            {
                hudl->changeCheatSprite();
            }
        }

        // create text layser
        _txtl = std::make_shared<TextLayer>();
        _txtl->_z_order = 10;
        gear.addLayer(_txtl);


        DEBUG_PRINT("GSPlay: change room\n");

        // TODO DEBUG ONLY
/*        if(level_nr == 1)
        {
            _pnk._gamestate.active_room = 3;
            _active_act_index = _pnk._gamestate.active_room - 1;
            changeRoom(_pnk._gamestate.active_room, true);
        }
        else
        {
        // choose room acc. to prefs
            _active_act_index = _pnk._gamestate.active_room - 1;
            changeRoom(_pnk._gamestate.active_room, true);
        }
*/
        _active_act_index = _pnk._gamestate.active_room - 1;
        changeRoom(_pnk._gamestate.active_room, true);

        DEBUG_PRINT("GSPlay: viewport\n");

        // set viewport to active room
        updateVpPos();
        gear.setViewportPos(_vp_pos - dang::Vector2F(160, 120));
    }

    void GSPlay::freeCurrentLevel()
    {
        _spr_hero.reset();
        _screenplay.reset();
        _csl.reset();
        _hives.clear();
        _tmx = nullptr;
        _active_act = nullptr;
        _active_act_index = -1;
        _last_time = 0;
        _warp = false;

        dang::Gear& gear = _pnk.getGear();

        // remove images
        gear.removeImagesheets();

        // remove layers
        gear.removeLayers();

        // remove behaviour trees
        gear.removeNTrees();
    }

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

            dang::SndGear::playSfx(bubble_blow_22050_mono, bubble_blow_22050_mono_length, _pnk._prefs.volume_sfx);
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
        else if (pe._type == ETG_CRATE_EXPLODES
                 || pe._type == ETG_BOMB_EXPLODES
                 || pe._type == ETG_CANNONBALL_EXPLODES)
        {
            handleExplodingThrowie(pe);
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
        else if (pe._type == ETG_START_BOSSBATTLE)
        {
            // let the boss battles begin
            startBossBattle();
        }
        else if (pe._type == ETG_BOSS_HIT)
        {
            handleBossHit(pe);
        }
        else if (pe._type == ETG_BOSS_DIES)
        {
            // hero won
            endBossBattle();
        }
        else if (pe._type == ETG_WARP_ROOM)
        {
            if (pe._payload != _active_act_index)
            {
                changeRoom(pe._payload, true);
            }
        }
        else if (pe._type == ETG_CHANGE_LEVEL)
        {
            if (pe._payload != _pnk._gamestate.active_level)
            {
                changeLevel(pe._payload);
            }
        }
    }

    void GSPlay::handleNewThrowie(PnkEvent& pe)
    {
        if (pe._type == ETG_NEW_THROWN_CRATE)
        {
            spCraties proto = std::dynamic_pointer_cast<Craties>(_hives["crate"]);
            assert(proto != nullptr);
            spCraties crate = std::make_shared<Craties>(*proto);
            crate->setPos(pe._pos);
            crate->_to_the_left = pe._to_the_left;
            crate->init();
            _csl->addCollisionSprite(crate);
        }
        else if (pe._type == ETG_NEW_THROWN_BOMB)
        {
            spBombies proto = std::dynamic_pointer_cast<Bombies>(_hives["bomb"]);
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
            mood->_anim_m_standard->setFinishedCallback(std::bind(&Moodies::removeSelf, mood.get()));

            _csl->addCollisionSprite(mood);

            dang::SndGear::playSfx(cannon_fire_22050_mono, cannon_fire_22050_mono_length, _pnk._prefs.volume_sfx);
        }
    }

    void GSPlay::handleExplodingThrowie(PnkEvent& pe)
    {
        // TODO have different animations for crates and the rest
        spMoodiesThatHurt proto = std::dynamic_pointer_cast<MoodiesThatHurt>(_hives["explosion"]);
        assert(proto != nullptr);
        spMoodiesThatHurt boom = std::make_shared<MoodiesThatHurt>(*proto);
        boom->setPosX(pe._pos.x - 16);
        boom->setPosY(pe._pos.y - 16);
        boom->init();
        boom->_anim_m_standard->setFinishedCallback(std::bind(&Moodies::removeSelf, boom.get()));

        dang::SndGear::playSfx(crate_explode_22050_mono, crate_explode_22050_mono_length, _pnk._prefs.volume_sfx);

        if (pe._type == ETG_CRATE_EXPLODES)
        {

        }
        else if (pe._type == ETG_BOMB_EXPLODES)
        {

        }
        else if (pe._type == ETG_CANNONBALL_EXPLODES)
        {

        }

        _csl->addCollisionSprite(boom);
    }

    void GSPlay::handleNewPoof(PnkEvent& pe)
    {
        spMoodies proto = std::dynamic_pointer_cast<Moodies>(_hives["poof"]);
        assert(proto != nullptr);
        spMoodies poof = std::make_shared<Moodies>(*proto);
        poof->setPos(pe._pos);
        poof->init();
        poof->_anim_m_standard->setFinishedCallback(std::bind(&Moodies::removeSelf, poof.get()));

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
            case ST_PIG_NORMAL:
                health -= 30;
                break;
            case ST_PIG_BOMB:
                health -= 35;
                break;
            case ST_PIG_BOX:
                health -= 35;
                break;
            case ST_FLYING_BOMB:
                health -= 10;
                break;
            case ST_FLYING_CRATE:
                health -= 20;
                break;
            case ST_FLYING_CANNONBALL:
                health -= 40;
                break;
            case ST_CANNON:
                health -= 40;
                break;
            case ST_EXPLOSION:
                health -= 50;
                break;
        }

        if(health <= 0)
        {
            handleKingLoosesLife();
        }
        else
        {
            if(_pnk._gamestate.health != health)
            {
                dang::SndGear::playSfx(king_damage_22050, king_damage_22050_length, _pnk._prefs.volume_sfx);
            }
            _pnk._gamestate.health = health;
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
        switch (pe._payload)
        {
            case ST_COIN_SILVER:
                addScore(10);
                break;
            case ST_COIN_GOLD:
                addScore(50);
                break;
            case ST_GEM_BLUE:
                addScore(30);
                break;
            case ST_GEM_GREEN:
                addScore(60);
                break;
            case ST_GEM_RED:
                addScore(100);
                break;
            case ST_POTION_BLUE:
                addHealth(1);
                break;
            case ST_POTION_RED:
                addHealth(5);
                break;
            case ST_POTION_GREEN:
                addHealth(20);
                break;
            case ST_PIG_REWARD:
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

        dang::SndGear::playSfx(teleport_22050_mono, teleport_22050_mono_length, _pnk._prefs.volume_sfx);
    }

    void GSPlay::changeLevel(int8_t level_nr)
    {
        DEBUG_PRINT("Changing level to %d\n\r", level_nr);
        freeCurrentLevel();

        // reset current level and room to new ones
        _pnk._gamestate.active_room = 0;
        _pnk._gamestate.active_level = level_nr;

        // TODO store gamestate values to disc so that the level gets loaded next time

        // TODO check level_nr for bounds
        loadLevel(level_nr);
    }

    void GSPlay::checkCheatActivation()
    {
        // inspector gadget for snes, debug menu
        if(_pnk.cheatKeyStream == "XXDLRULL")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
            DEBUG_PRINT("Cheat activated: Back to last room.\r\n");

            userIsCheating();
            changeRoom(_active_act_index - 1, true);
        }
        else if(_pnk.cheatKeyStream == "XXDLRURR")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
            DEBUG_PRINT("Cheat activated: Forward to next room.\r\n");

            userIsCheating();
            changeRoom(_active_act_index + 1, true);
        }
        else if(_pnk.cheatKeyStream == "XXDLRUUU")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
            DEBUG_PRINT("Cheat activated: Up to the next level.\r\n");

            userIsCheating();
            changeLevel(_pnk._gamestate.active_level + 1);
        }
        else if(_pnk.cheatKeyStream == "XXDLRUDD")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
            DEBUG_PRINT("Cheat activated: Down to the last level.\r\n");

            userIsCheating();
            changeLevel(_pnk._gamestate.active_level - 1);
        }
    }

    void GSPlay::userIsCheating()
    {
        spHUDLayer hudl = std::static_pointer_cast<HUDLayer>(_pnk.getGear().getLayerByTypename(dang::Layer::LT_HUDLAYER));
        if(hudl == nullptr)
        {
            return;
        }

        hudl->changeCheatSprite();
    }


    dang::BTNode::Status GSPlay::NTheroInSightH(dang::spSprite s)
    {
        dang::spCollisionSprite cs = std::dynamic_pointer_cast<dang::CollisionSprite>(s);
        float ret = _csl->aaLoSH(cs, _spr_hero);

        if (ret != 0)
        {
            cs->getNTreeState()->_payload["aaLoSH"] = ret;
            return dang::BTNode::Status::SUCCESS;
        }
        return dang::BTNode::Status::FAILURE;
    }

    void GSPlay::startBossBattle()
    {
        // activate boss
        _pnk._gamestate.boss_health = 100;
        if(_spr_boss != nullptr)
        {

        }

        // change tune?

        // activate boss health display in the hud
        spHUDLayer hudl = std::static_pointer_cast<HUDLayer>(_pnk.getGear().getLayerByTypename(dang::Layer::LT_HUDLAYER));
        if(hudl != nullptr)
        {
            hudl->activateBossHUD();
        }

        // play fanfare (actually its a meep meep)
        dang::SndGear::playSfx(boss_battle_22050_mono, boss_battle_22050_mono_length, _pnk._prefs.volume_sfx);
    }

    void GSPlay::endBossBattle()
    {
        /*
        // lets not do this, player should see how she killed the boss
        spHUDLayer hudl = std::static_pointer_cast<HUDLayer>(_pnk.getGear().getLayerByTypename(dang::Layer::LT_HUDLAYER));
        if(hudl != nullptr)
        {
            hudl->deactivateBossHUD();
        }
         */

        // show doors
        dang::spSpriteLayer mood = std::static_pointer_cast<dang::SpriteLayer>(_pnk.getGear().getLayerByName("lvl_1_mood"));
        if(mood != nullptr)
        {
            auto spr = mood->getSpriteByType("door");
            if(spr != nullptr)
            {
                spr->setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{1, 2, 3}, 900, dang::Ease::Linear, 1)));
            }
        }

        // activate level trigger
        dang::spSpriteLayer ol = std::static_pointer_cast<dang::SpriteLayer>(_pnk.getGear().getLayerByTypename(dang::Layer::LT_COLLISIONSPRITELAYER));
        if(ol != nullptr)
        {
            auto spr = ol->getSpriteByType(SpriteFactory::T_LEVEL_TRIGGER);
            if(spr != nullptr)
            {
                auto lvlTrigger = std::static_pointer_cast<LevelTrigger>(spr);
                lvlTrigger->activateTrigger();
            }
        }

        dang::SndGear::playSfx(victory_22050_mono, victory_22050_mono_length, _pnk._prefs.volume_sfx);

        // show end text
        switch (_pnk._gamestate.active_level)
        {
            case 1:
            default:
                showInfoLayer(false, 10000, str_lvl1_end);
                break;
            case 2:
                showInfoLayer(false, 10000, str_lvl2_end);
                break;
        }

    }

    void GSPlay::handleBossHit(PnkEvent& pe)
    {
        int8_t health = _pnk._gamestate.boss_health;

        // the boss can't be more dead than dead
        if(health <= 0)
        {
            return;
        }

        switch(_pnk._gamestate.active_level)
        {
                case 1:
                    health -= 34;
                    break;
                case 2:
                    health -= 34;
                    break;
        }

        if(health <= 0)
        {
            _spr_boss->die();

            // tell the pig king he is dead
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_BOSS_DIES));
            e->_payload = 1;
            pnk::_pnk._dispatcher.queueEvent(std::move(e));

            _pnk._gamestate.boss_health = 0;
        }
        else
        {
            _pnk._gamestate.boss_health = health;
        }

    }

    void GSPlay::showInfoLayer(bool pause, uint32_t ttl, const std::string_view &message)
    {
/*        if (pause)
        {
            _pnk.getGear().setLayersActive(false);
        }
*/
        _csl->setActive(false);
        _txtl->setText(message);
        _txtl->setTtl(ttl, std::bind(&GSPlay::hideInfoLayer, this));
        _txtl->setActive(true);
        _txtl->setVisibility(true);
    }

    void GSPlay::hideInfoLayer()
    {
//        _pnk.getGear().setLayersActive(true);
        _csl->setActive(true);
        _txtl->setActive(false);
        _txtl->setVisibility(false);
    }


}

