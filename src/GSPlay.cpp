// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game


#include "pnk_globals.h"
#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "SpriteFactory.hpp"
#include "GSPlay.h"
#include "GSHome.h"
#include "GSEndScene.hpp"
#include "HUDLayer.hpp"

#include "actors/hero/Hero.h"
#include "actors/npc/Enemy.h"
#include "actors/npc/HenchPig.h"
#include "actors/npc/PigCannon.h"
#include "actors/npc/PigBoss.h"
#include "actors/throwies/Bombies.h"
#include "actors/throwies/Bubble.h"
#include "actors/throwies/Cannonball.h"
#include "actors/throwies/Throwies.h"
#include "actors/throwies/Craties.h"
#include "actors/others/Moodies.h"
#include "actors/others/MoodiesThatHurt.h"
#include "actors/others/LevelTrigger.h"
#include "actors/others/Cannon.h"
#include "actors/others/Reward.h"

#include "levels/Level1SP.hpp"
#include "levels/Level2SP.hpp"
#include "levels/Level3SP.hpp"
#include "levels/Level4SP.hpp"

#include "tracks/kingsofdawn.h"

#include "sfx/cannon_fire_22050_mono.h"
#include "sfx/bomb_explode_22050_mono.h"
#include "sfx/boss_battle_22050_mono.h"
#include "sfx/bubble_blow_22050_mono.h"
#include "sfx/cheat_22050_mono.h"
#include "sfx/coin_22050_mono.h"
#include "sfx/crate_explode_22050_mono.h"
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
#include "rsrc/gfx/lives.png.h"
#include "rsrc/gfx/lives_hud.png.h"
#include "rsrc/gfx/pig_king.png.h"
#include "rsrc/gfx/castle_decoration_tiles.png.h"
#include "rsrc/gfx/door.png.h"
#include "rsrc/gfx/pig.png.h"
#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/gfx/hud_ui.png.h"
#include "rsrc/level_1.tmx.hpp"
#include "rsrc/level_2.tmx.hpp"
#include "rsrc/level_3.tmx.hpp"
#include "rsrc/level_4.tmx.hpp"
#include "rsrc/game_strings.hpp"
#include "fonts/barcadebrawl.h"

#include <Gear.hpp>
#include <Imagesheet.hpp>
#include <snd/SndGear.hpp>
#include <sprite/ColSpr.hpp>
#include <sprite/ImgSpr.hpp>
#include <sprite/FullImgSpr.hpp>
#include <layer/TileLayer.hpp>
#include <layer/MessageLayer.hpp>
#include <layer/ImgSprLayer.hpp>
#include <layer/ColSprLayer.hpp>
#include <tween/Ease.hpp>
#include <tween/TwAnim.hpp>
#include <tween/TwVel.hpp>
#include <path/SceneGraph.hpp>
#include <path/Waypoint.hpp>
#include <bt/NTreeState.h>

#include <malloc.h>
#include <cassert>
#include <memory>
#include <iostream>


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
//        DEBUG_PRINT("GSPlay: play updating\n");

        updateCheatKeyStream(blit::buttons.pressed);
        checkCheatActivation();

        if (_endScene)
        {
            return GameState::_gs_end_scene;
        }
        if (blit::buttons.pressed & BTN_EXIT || _leaveGame)
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
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: entered (%d)\n", mallinfo().uordblks);
#endif

        // make sure to not just leave again now we met
        _leaveGame = false;

#ifdef TARGET_32BLIT_HW
#ifdef PNK_DEBUG_MEM
        // memory stats

        auto static_used = &_end - &_sbss;
        auto heap_total = &__ltdc_start - &_end;
        auto total_ram = static_used + heap_total;
        auto heap_used = mallinfo().uordblks;


        blit::debugf("Mem: %i + %i (%i) = %i\r\n", static_used, heap_used, heap_total, total_ram);
#endif
#endif

        dang::SndGear::stopMod();
        dang::SndGear::playMod(kingsofdawn_mod, kingsofdawn_mod_length, _pnk._prefs.volume_track);

#ifdef PNK_LF_LEVEL_DEBUG
        _pnk._gamestate.saved_level = 1;
#endif
        loadLevel(_pnk._gamestate.saved_level);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: callbacks\n");
#endif
        // add event callback
        std::function<void (dang::Event&)> func = std::bind(&GSPlay::gameEventReceived, this, std::placeholders::_1);
        _sub_ref = _pnk._dispatcher.registerSubscriber(func, EF_GAME);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: entered, let the games begin\n");
#endif

    }

    void GSPlay::exit(dang::Gear &gear, uint32_t time)
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: enter exit()\n");
#endif
        // only happens when dying or when solved the game, reset some params
        if(_leaveGame || _endScene)
        {
            _pnk._gamestate.saved_room = 0;
            _pnk._gamestate.saved_level = 1;
            _pnk._gamestate.score = 0;
            _pnk._gamestate.lives = HERO_MAX_LIVES;
            _pnk._gamestate.health = HERO_MAX_HEALTH;
            _pnk._gamestate.boss_health = BOSS_MAX_HEALTH;
            _pnk._gamestate.has_cheated = false;
            _pnk._gamestate.invincible = false;
            _pnk._removed_sprites.clear();
        }
        saveGamestate();

        // remove callback
        _pnk._dispatcher.removeSubscriber(_sub_ref);
        _sub_ref = 0;

        freeCurrentLevel();

        dang::SndGear::stopMod();

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: exit exit()\n");
#endif
    }

    void GSPlay::loadLevel(int8_t level_nr)
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: load level %d (%d)\n", level_nr, mallinfo().uordblks);
#endif
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
            case 3:
                _screenplay = std::make_shared<Level3SP>(*this);
                _tmx = &level_3_level;
                break;
            case 4:
                _screenplay = std::make_shared<Level4SP>(*this);
                _tmx = &level_4_level;
                break;
        }

        dang::Gear& gear = _pnk.getGear();

        dang::TmxExtruder txtr(_tmx, &gear);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: extruded (%d)\n", mallinfo().uordblks);
#endif
        dang::RectF vp = {0, 0, 320, 240};
        gear.initLevel(_tmx, vp);
//        gear.setActiveWorldSize(vp.w + 16, vp.h + 16);

        // fill up screenplay
        for (size_t i = 0; i < _tmx->zones_len; ++i)
        {
            // add scenegraph
            dang::RectF zone{_tmx->zones[i].x, _tmx->zones[i].y, _tmx->zones[i].w, _tmx->zones[i].h};
            txtr.createSceneGraphs(zone, _screenplay->_scene_graphs[_tmx->zones[i].zone_nr]);

        }

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: imagesheet (%d)\n", mallinfo().uordblks);
#endif

        // init imagesheets
        txtr.getImagesheets();

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: tile layer (%d)\n", mallinfo().uordblks);
#endif

        // create background Tilelayer
        txtr.getTileLayer(_screenplay->_l_bg_name, true);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: mood layer (%d)\n", mallinfo().uordblks);
#endif

        // create mood Tilelayer
        if (!_screenplay->_l_mood_name.empty())
        {
            dang::spImgSprLayer sl = txtr.getImgSprLayer(_screenplay->_l_mood_name, false, true, true);

            for (size_t j = 0; j < sl->tmxLayer()->spriteobejcts_len; j++)
            {
                const dang::tmx_spriteobject* so = sl->tmxLayer()->spriteobjects + j;

                dang::spImagesheet is = gear.getImagesheet(so->tileset);

                assert(is != nullptr);

                if (!so->type.empty())
                {
                    dang::spFullImgSpr spr = std::make_shared<dang::FullImgSpr>(so, is);
                    if (so->type == "door")
                    {
                        spr->setTypeNum(ST_MOOD_DOOR);
                    }

                    dang::spTwAnim animation = txtr.getAnimation(is, so->type);
                    if (animation != nullptr)
                    {
                        spr->setAnimation(animation);
                    }
                    sl->addSprite((dang::spImgSpr)spr);
                }
                else
                {
                    dang::spImgSpr spr = std::make_shared<dang::ImgSpr>(so, is);
                    sl->addSprite(spr);
                }

            }


        }

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: collision sprite layer (%d)\n", mallinfo().uordblks);
#endif

        // create Spritelayer with collision detection
        _csl = txtr.getColSprLayer(_screenplay->_l_obj_name, true);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: sprite objects (%d)\n", mallinfo().uordblks);
#endif

//#ifdef TARGET_32BLIT_HW

        // memory stats
        //blit::debugf("Mem: %i + %i (%i) = %i\n", static_used, mallinfo().uordblks, heap_total, total_ram);

//#endif

        // create sprites
        for (size_t j = 0; j < _csl->tmxLayer()->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = _csl->tmxLayer()->spriteobjects + j;

            dang::spImagesheet is = gear.getImagesheet(so->tileset);
            std::unordered_map<std::string, dang::spImagesheet> iss = gear.getImagesheets();
            dang::spColSpr spr = nullptr;

            if (   so->type == SpriteFactory::T_HOTRECT
                || so->type == SpriteFactory::T_HOTRECT_PLATFORM)
            {
                spr = SpriteFactory::RigidObj(so);
            }
            else if (so->type == SpriteFactory::T_ROOM_TRIGGER) { spr = SpriteFactory::RoomTrigger(so, false); }
            else if (so->type == SpriteFactory::T_WARP_ROOM_TRIGGER) { spr = SpriteFactory::RoomTrigger(so, true); }
            else if (so->type == SpriteFactory::T_LEVEL_TRIGGER) { spr = SpriteFactory::LevelTrigger(so); }
            else if (so->type == SpriteFactory::T_BOSSBATTLE_TRIGGER) { spr = SpriteFactory::BossbattleTrigger(so); }
            else if (so->type == SpriteFactory::T_SAVEPOINT_TRIGGER) { spr = SpriteFactory::SavepointTrigger(so); }
            else if (  so->type == SpriteFactory::T_COIN_SILVER
                    || so->type == SpriteFactory::T_COIN_GOLD
                    || so->type == SpriteFactory::T_GEM_BLUE
                    || so->type == SpriteFactory::T_GEM_GREEN
                    || so->type == SpriteFactory::T_GEM_RED
                    || so->type == SpriteFactory::T_POTION_BLUE
                    || so->type == SpriteFactory::T_POTION_RED
                    || so->type == SpriteFactory::T_POTION_GREEN)
            {
                spr = SpriteFactory::Reward(txtr, so, is);
            }
            else if (so->type == SpriteFactory::T_PIG_CANNON)
            {
                spr = SpriteFactory::PigCannon(txtr, so, is, _screenplay);

            }
            else if (so->type == SpriteFactory::T_KING)
            {
                _spr_hero = SpriteFactory::King(txtr, so, is);
                spr = _spr_hero;
            }

            if (spr != nullptr)
            {
                _csl->addSprite(spr);
            }
            else
            {
                if (so->type == SpriteFactory::T_BOSS)
                {
                    _spr_boss = SpriteFactory::Boss(txtr, so, iss, _screenplay);
                    spr = _spr_boss;
                }
                else if (so->type == SpriteFactory::T_PIG_NORMAL)             { spr = SpriteFactory::NormalPig(txtr, so, is, _screenplay); }
                else if (so->type == SpriteFactory::T_PIG_BOX)           { spr = SpriteFactory::PigCrate(txtr, so, iss, _screenplay); }
                else if (so->type == SpriteFactory::T_PIG_BOMB)          { spr = SpriteFactory::PigBomb(txtr, so, iss, _screenplay); }

                if (spr != nullptr)
                {
                    _csl->addSprite(spr);

                    // global pos is needed for the graphs and the global pos can only be determind when added to a layer due to the tree-structure
                    spEnemy en = std::static_pointer_cast<Enemy>(spr);
                    en->initSceneGraph(_screenplay, txtr);
                }

            }

            if (spr == nullptr)
            {

                if (so->type == SpriteFactory::T_BUBBLE_PROTO)
                {
                    dang::spColSpr sprc = SpriteFactory::Bubble(txtr, so, is, false, _screenplay->_bubble_loops);
                    assert(sprc != nullptr);
                    _hives["bubble"] = sprc;
                }
                else if (so->type == SpriteFactory::T_CRATE_PROTO)
                {
                    dang::spColSpr sprc = SpriteFactory::Crate(txtr, so, is, false);
                    assert(sprc != nullptr);
                    _hives["crate"] = sprc;
                }
                else if (so->type == SpriteFactory::T_BOMB_PROTO)
                {
                    dang::spColSpr sprc = SpriteFactory::Bomb(txtr, so, is);
                    assert(sprc != nullptr);
                    _hives["bomb"] = sprc;
                }
                else if (so->type == SpriteFactory::T_EXPLOSION_PROTO)
                {
                    dang::spColSpr sprc = SpriteFactory::Explosion(txtr, so, is);
                    assert(sprc != nullptr);
                    _hives["explosion"] = sprc;
                }
                else if (so->type == SpriteFactory::T_PIG_POOF_PROTO)
                {
                    dang::spColSpr sprc = SpriteFactory::PigPoof(txtr, so, is);
                    assert(sprc != nullptr);
                    _hives["poof"] = sprc;
                }
                else if (so->type == SpriteFactory::T_CANNONBALL_PROTO)
                {
                    dang::spColSpr sprc = SpriteFactory::Cannonball(txtr, so, is, false);
                    assert(sprc != nullptr);
                    _hives["cannonball"] = sprc;
                }
                else if (so->type == SpriteFactory::T_CANNONMUZZLE_PROTO)
                {
                    dang::spColSpr sprc = SpriteFactory::Cannonmuzzle(txtr, so, is);
                    assert(sprc != nullptr);
                    _hives["cannonmuzzle"] = sprc;
                }
                else
                {
#ifdef PNK_DEBUG_COMMON
                    DEBUG_PRINT("GSPlay: sprite type unknown. Id=%i, type=%s\n", so->id, so->type.c_str());
#endif
                }
            }

#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("GSPlay: sprite %d of %d (%d)\n", j + 1, _csl->_tmx_layer->spriteobejcts_len, mallinfo().uordblks);
#endif
            // remove sprites that have been taken/vanquished before
            for (auto id : _pnk._removed_sprites)
            {
                dang::spSprObj s = _csl->getSpriteById(id);
                if (s != nullptr)
                {
                    // if the types are wrong, then the save file is not OK
                    // and the player messed around with the savefile
                    // no need to crash, just jump over it.
                    if((s->typeNum() >= ST_ENEMIES && s->typeNum() < ST_ENEMIES_END)
                        || (s->typeNum() >= ST_REWARDS && s->typeNum() < ST_REWARDS_END))
                    {
                        s->markRemove();
                    }
                }
            }
            _csl->cleanSpritelist();


#ifdef TARGET_32BLIT_HW

            // memory stats
        //DEBUG_PRINT("GSPlay: Mem: %i + %i (%i) = %i\n", static_used, mallinfo().uordblks, heap_total, total_ram);

#endif
        }

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: fg layer\n");
#endif
        // create foreground layer
        txtr.getImgSprLayer(_screenplay->_l_fg_name, true, true, false);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: hud layer\n");
#endif
        // create HUD layer
        spHUDLayer hudl = std::make_shared<HUDLayer>();
        if (!_screenplay->_l_hud_name.empty())
        {
            txtr.fillHUDLayer(hudl, _screenplay->_l_hud_name, false, true);
            hudl->fillSprites(_pnk.getGear());
            hudl->setAnimations(txtr);

            if(_pnk.isCheating())
            {
                hudl->changeCheatSprite();
            }
        }

        // create text layer
        dang::PointF p{0,0};
        _txtl = std::make_shared<dang::MessageLayer>(barcadebrawl, p, 10, "", true, true);
        _txtl->setButtons(BTN_OK, BTN_CANCEL);
        gear.addLayer(_txtl);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: change room\n");
#endif
        resetRoomFromSave();

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: viewport\n");
#endif
        // set viewport to active room
        updateVpPos();
        gear.setViewportPos(_vp_pos - dang::Vector2F(160, 120));
        // set the zonebits of each sprite correcty
        _csl->resetZoneBit(gear.getViewport());


        // show starting text (only at the beginning of the level)
        if (_active_room_index == 0)
        {
            switch (_pnk._gamestate.saved_level)
            {
                case 1:
                default:
                    showInfoLayer(true, 10000, str_lvl1_intro);
                    break;
                case 2:
                    showInfoLayer(true, 10000, str_lvl2_intro);
                    break;
                case 3:
                    showInfoLayer(true, 10000, str_lvl3_intro);
                    break;
                case 4:
                {
                    showInfoLayer(true, 10000, str_lvl4_intro);
                    auto spr = _csl->getSpriteByTypeNum(ST_LEVEL_TRIGGER);
                    if(spr != nullptr)
                    {
                        auto lvlTrigger = std::static_pointer_cast<LevelTrigger>(spr);
                        lvlTrigger->activateTrigger();
                    }
                    break;
                }
            }
        }
    }

    void GSPlay::freeCurrentLevel()
    {
        _tmx = nullptr;
        _screenplay.reset();
        _spr_hero.reset();
        _spr_boss.reset();
        _csl.reset();
        _txtl.reset();
        _hives.clear();
        _vp_pos = {0,0};

        _active_room = {0,0,0,0};
        _active_room_index = -1;
        _warp = false;
        _leaveGame = false;
        _endScene = false;


        dang::Gear& gear = _pnk.getGear();

        // remove images
        gear.removeImagesheets();

        // remove layers
        gear.removeLayers();
    }

    void GSPlay::gameEventReceived(dang::Event &e)
    {
        PnkEvent& pe = static_cast<PnkEvent&>(e);
        if (pe._type == ETG_NEW_BUBBLE)
        {
            spBubble bub_proto = std::static_pointer_cast<Bubble>(_hives["bubble"]);
            assert(bub_proto != nullptr);
            spBubble bub = std::make_shared<Bubble>(*bub_proto);
            bub->setPos(pe._pos);
            bub->_to_the_left = pe._to_the_left;
            bub->init();
            _csl->addSprite((dang::spColSpr)bub);

            dang::SndGear::playSfx(bubble_blow_22050_mono, bubble_blow_22050_mono_length, _pnk._prefs.volume_sfx);
        }
        else if (pe._type == ETG_SPR_CONSUMED_BY_HERO)
        {
//            std::cout << "add sprite with id=" << pe._payload << " to removed list" << std::endl;
            _pnk._removed_sprites.push_front(pe._payload);
        }
        else if (pe._type == ETG_REMOVE_SPRITE)
        {
            std::printf("depracated: ETG_REMOVE_SPRITE. Use markRemove() instead\n");
            throw;
/*            std::shared_ptr<dang::Sprite> spr = pe._spr.lock();
            if (spr != nullptr)
            {
                std::cout << "depracated: ETG_REMOVE_SPRITE with sprite id=" << spr->_id << ". Use markRemove() instead" << std::endl;
                spr->markRemove();

                if ((spr->_type_num > ST_ENEMIES && spr->_type_num < ST_ENEMIES_END) ||
                    (spr->_type_num > ST_REWARDS && spr->_type_num < ST_REWARDS_END))
                {
                    _pnk._removed_sprites.push_front(spr->_id);
                }
#ifdef PNK_DEBUG_COMMON
                DEBUG_PRINT("GSPlay: remove sprite from layer\n");
#endif
            }
            else
            {
#ifdef PNK_DEBUG_COMMON
                DEBUG_PRINT("GSPlay: CAUTION: attempted to remove sprite with shared_ptr = nullptr\n");
#endif
            }
            */
        }
        else if (pe._type == ETG_NEW_THROWN_CRATE
                || pe._type == ETG_NEW_THROWN_BOMB
                || pe._type == ETG_NEW_FIRED_CANNON
                || pe._type == ETG_NEW_DROP_BOMB
                || pe._type == ETG_NEW_DROP_CRATE)
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
#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("GSPlay: event ETG_KING_HIT\n");
#endif
            handleKingHealth(pe);
        }
        else if (pe._type == ETG_KING_LIFE_LOST_SEQ_ENDED)
        {
            // deactivate boss-HUD when restoring from a savepoint
            spHUDLayer hudl = std::static_pointer_cast<HUDLayer>(_pnk.getGear().getLayerByTypename(dang::Layer::LT_HUDLAYER));
            if (hudl != nullptr)
            {
                hudl->deactivateBossHUD();
            }

            // reset health
            _pnk._gamestate.health = HERO_MAX_HEALTH;
            // reset last savepos
            resetRoomFromSave();
        }
        else if (pe._type == ETG_REWARD_HIT)
        {
            dang::SndGear::playRumbleTrack(&dang::poke, 0);

            handleRewardCollected(pe);
        }
        else if (pe._type == ETG_CHANGE_ROOM)
        {
            if (pe._payload != _active_room_index)
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
            if (pe._payload != _active_room_index)
            {
                changeRoom(pe._payload, true);
            }
        }
        else if (pe._type == ETG_CHANGE_LEVEL)
        {
            if (pe._payload != _pnk._gamestate.saved_level)
            {
                changeLevel(pe._payload);
            }
        }
        else if (pe._type == ETG_SAVEPOINT_TRIGGERED)
        {
            _pnk._gamestate.saved_room = _active_room_index;
            saveGamestate();
        }
    }

    void GSPlay::handleNewThrowie(PnkEvent& pe)
    {
        if (pe._type == ETG_NEW_THROWN_CRATE || pe._type == ETG_NEW_DROP_CRATE)
        {
            spCraties crate = SpriteFactory::CrateFromProto(std::static_pointer_cast<Craties>(_hives["crate"]), pe._pos, pe._to_the_left);

            // movement sequence
            float velx;
            switch (_pnk._gamestate.saved_level)
            {
                default:
                case 1:
                case 2:
                case 3:
                    velx = pe._type == ETG_NEW_THROWN_CRATE ? CRATE_VEL : CRATE_DROP_VEL;
                    break;
                case 4:
                    velx = pe._type == ETG_NEW_THROWN_CRATE ? CRATE_VEL2 : CRATE_DROP_VEL;
                    break;
            }
            velx = pe._to_the_left ? -velx : velx;
//            float velx = pe._type == ETG_NEW_THROWN_CRATE ? CRATE_VEL : CRATE_DROP_VEL;
//            velx = pe._to_the_left ? -velx : velx;
            dang::spTwVel twv1 = std::make_shared<dang::TwVel>(dang::Vector2F(velx, -6), _pnk._gravity, 600, &dang::Ease::InQuad, 1, false, 100);
            crate->addTween(twv1);

            _csl->addSprite((dang::spColSpr)crate);
        }
        else if (pe._type == ETG_NEW_THROWN_BOMB || pe._type == ETG_NEW_DROP_BOMB)
        {
            spBombies bomb = SpriteFactory::BombFromProto(std::static_pointer_cast<Bombies>(_hives["bomb"]), pe._pos, pe._to_the_left);

            // movement sequence
            float velx;
            switch (_pnk._gamestate.saved_level)
            {
                default:
                case 1:
                case 2:
                    velx = pe._type == ETG_NEW_THROWN_BOMB ? BOMB_VEL : BOMB_DROP_VEL;
                    break;
                case 3:
                    velx = pe._type == ETG_NEW_THROWN_BOMB ? BOMB_VEL2 : BOMB_DROP_VEL;
                    break;
                case 4:
                    velx = pe._type == ETG_NEW_THROWN_BOMB ? BOMB_VEL3 : BOMB_DROP_VEL;
                    break;
            }
            velx = pe._to_the_left ? -velx : velx;
            dang::spTwVel twv1 = std::make_shared<dang::TwVel>(dang::Vector2F(velx, -4), _pnk._gravity, 600, &dang::Ease::InQuad, 1, false, 100);
            bomb->addTween(twv1);

            _csl->addSprite((dang::spColSpr)bomb);
        }
        else if (pe._type == ETG_NEW_FIRED_CANNON)
        {
            spCannonball ball = SpriteFactory::CannonballFromProto(std::static_pointer_cast<Cannonball>(_hives["cannonball"]), pe._pos, pe._to_the_left);
            _csl->addSprite((dang::spColSpr)ball);

            spMoodies mood = SpriteFactory::CannonmuzzleFromProto(std::static_pointer_cast<Moodies>(_hives["cannonmuzzle"]), pe._pos, pe._to_the_left);
            _csl->addSprite((dang::spColSpr)mood);

            dang::SndGear::playSfx(cannon_fire_22050_mono, cannon_fire_22050_mono_length, _pnk._prefs.volume_sfx);
        }
    }

    void GSPlay::handleExplodingThrowie(PnkEvent& pe)
    {
        if (pe._type == ETG_BOMB_EXPLODES || pe._type == ETG_CANNONBALL_EXPLODES)
        {
            spMoodiesThatHurt proto = std::static_pointer_cast<MoodiesThatHurt>(_hives["explosion"]);
            assert(proto != nullptr);
            spMoodiesThatHurt boom = std::make_shared<MoodiesThatHurt>(*proto);
            boom->setPosX(pe._pos.x - 16);
            boom->setPosY(pe._pos.y - 16);
            boom->init();
            boom->_anim_m_standard->setFinishedCallback(std::bind(&Moodies::markRemove, boom.get()));
            _csl->addSprite((dang::spColSpr)boom);

            dang::SndGear::playSfx(bomb_explode_22050_mono, bomb_explode_22050_mono_length, _pnk._prefs.volume_sfx);
            dang::SndGear::playRumbleTrack(&dang::explosion, 0);
        }
        else if (pe._type == ETG_CRATE_EXPLODES)
        {
            // no explosion when crates explode, only sound
            dang::SndGear::playSfx(crate_explode_22050_mono, crate_explode_22050_mono_length, _pnk._prefs.volume_sfx);
        }
    }

    void GSPlay::handleNewPoof(PnkEvent& pe)
    {
        spMoodies proto = std::static_pointer_cast<Moodies>(_hives["poof"]);
        assert(proto != nullptr);
        spMoodies poof = std::make_shared<Moodies>(*proto);
        poof->setPos(pe._pos);
        poof->init();
        poof->_anim_m_standard->setFinishedCallback(std::bind(&Moodies::markRemove, poof.get()));

        _csl->addSprite((dang::spColSpr)poof);
    }

    void GSPlay::handleKingHealth(PnkEvent& pe)
    {
        if (_pnk._gamestate.invincible)
        {
            return;
        }

        // get current health (and yes, we want signed to go below 0!)
        int8_t health = _pnk._gamestate.health;

        switch (pe._payload)
        {
            case ST_PIG_NORMAL:         health -= DAMAGE_PIG_NORMAL;        break;
            case ST_PIG_BOMB:           health -= DAMAGE_PIG_BOMB;          break;
            case ST_PIG_CRATE:          health -= DAMAGE_PIG_CRATE;         break;
            case ST_FLYING_BOMB:        health -= DAMAGE_FLYING_BOMB;       break;
            case ST_FLYING_CRATE:       health -= DAMAGE_FLYING_CRATE;      break;
            case ST_FLYING_CANNONBALL:  health -= DAMAGE_FLYING_CANNONBALL; break;
            case ST_CANNON:             health -= DAMAGE_CANNON;            break;
            case ST_EXPLOSION:          health -= DAMAGE_EXPLOSION;         break;
            case ST_PIG_BOSS:           health -= DAMAGE_FROM_PIGBOSS;           break;
        }

        if (health <= 0)
        {
            handleKingLoosesLife();
        }
        else
        {
            if(_pnk._gamestate.health != health)
            {
                dang::SndGear::playSfx(king_damage_22050, king_damage_22050_length, _pnk._prefs.volume_sfx);
                dang::SndGear::playRumbleTrack(&dang::double_knock, 0);
            }
#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("GSPlay: health=%i\n", (uint32_t) _pnk._gamestate.health);
#endif
        }
        _pnk._gamestate.health = health;
    }

    void GSPlay::handleKingLoosesLife()
    {
        _pnk._gamestate.lives -= 1;
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSPlay: handleKingLoosesLife, lives=%i\n", (uint32_t) _pnk._gamestate.lives);
#endif
        printf("GSPlay: handleKingLoosesLife, lives=%i\n", (uint32_t) _pnk._gamestate.lives);

        if(_pnk._gamestate.lives <= 0)
        {
            _spr_hero->gameOver();
            showGameOverInfo();

            // reset lives count before storing current game status to disc
//            _pnk._gamestate.lives = HERO_MAX_LIVES;
//            _pnk._gamestate.health = HERO_MAX_HEALTH;
        }
        else
        {
            // this is done in the event-handling function (after the life-lost-animation)
//            _pnk._gamestate.health = HERO_MAX_HEALTH;

            // life lost sequence of hero
            _spr_hero->lifeLost();
        }

        dang::SndGear::playSfx(lifelost_22050_mono, lifelost_22050_mono_length, _pnk._prefs.volume_sfx);

    }

    void GSPlay::resetRoomFromSave()
    {
        _active_room_index = _pnk._gamestate.saved_room;

        dang::TmxExtruder ext(_tmx, &_pnk.getGear());
        _active_room = ext.getZone(_active_room_index);
        dang::Vector2F sp = ext.getPassage(_active_room_index, -1);
        _spr_hero->setPos(sp);
        _warp = true;

        _pnk.getGear().setActiveWorld(_active_room.x - 16, _active_room.y - 16, _active_room.w + 32, _active_room.h + 32);

    }

    void GSPlay::handleRewardCollected(PnkEvent& pe)
    {
        switch (pe._payload)
        {
            case ST_COIN_SILVER:
                addScore(REWARD_POINT_COIN_SILVER);
                break;
            case ST_COIN_GOLD:
                addScore(REWARD_POINT_COIN_GOLD);
                break;
            case ST_GEM_BLUE:
                addScore(REWARD_POINT_GEM_BLUE);
                break;
            case ST_GEM_GREEN:
                addScore(REWARD_POINT_GEM_GREEN);
                break;
            case ST_GEM_RED:
                addScore(REWARD_POINT_GEM_RED);
                break;
            case ST_POTION_BLUE:
                addHealth(REWARD_POINT_POTION_BLUE);
                break;
            case ST_POTION_RED:
                addHealth(REWARD_POINT_POTION_RED);
                break;
            case ST_POTION_GREEN:
                addHealth(REWARD_POINT_POTION_GREEN);
                break;
            case ST_PIG_REWARD:
                addScore(REWARD_POINT_PIG_REWARD);
                break;
                // Default gets nothing
            default:
                break;
        }
    }

    void GSPlay::addScore(uint8_t score)
    {
        _pnk._gamestate.score += score;

        if(_pnk._gamestate.score > _pnk._gamestate.high_score)
        {
            _pnk._gamestate.high_score = _pnk._gamestate.score;
        }

        dang::SndGear::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length, _pnk._prefs.volume_sfx);
    }

    void GSPlay::addHealth(uint8_t healthGain)
    {
        uint8_t h = _pnk._gamestate.health;

        h += healthGain;

        // make sure to not have more health than possible in game logic
        if(h > HERO_MAX_HEALTH)
        {
            h = HERO_MAX_HEALTH;
        }

        _pnk._gamestate.health = h;
        dang::SndGear::playSfx(health_22050_mono, health_22050_mono_length, _pnk._prefs.volume_sfx);
    }


    void GSPlay::updateVpPos()
    {
        // viewport follows hero within room
        dang::Vector2F pos = _spr_hero->getPos() + _spr_hero->getSize() / 2.0f;

        if (pos.x < _active_room.left() + 160)
        {
            _vp_pos.x = _active_room.left() + 160;
        }
        else if (pos.x > _active_room.right() - 160)
        {
            _vp_pos.x = _active_room.right() - 160;
        }
        else
        {
            _vp_pos.x = pos.x;
        }

        if (pos.y < _active_room.top() + 120)
        {
            _vp_pos.y = _active_room.top() + 120;
        }
        else if (pos.y > _active_room.bottom() - 120)
        {
            _vp_pos.y = _active_room.bottom() - 120;
        }
        else
        {
            _vp_pos.y = pos.y;
        }
    }

    void GSPlay::changeRoom(int32_t room_nr, bool warp)
    {
        dang::TmxExtruder ext(_tmx, &_pnk.getGear());
        _active_room = ext.getZone(room_nr);
        _pnk.getGear().setActiveWorld(_active_room.x - 16, _active_room.y - 16, _active_room.w + 32, _active_room.h + 32);

//        _active_room = &_screenplay->_acts[room_nr];

        if (warp)
        {
            dang::Vector2F sp = ext.getPassage(room_nr, _active_room_index);
            _spr_hero->setPos(sp);
            _warp = true;
        }

        _active_room_index = room_nr;

        dang::SndGear::playSfx(teleport_22050_mono, teleport_22050_mono_length, _pnk._prefs.volume_sfx);
    }

    void GSPlay::changeLevel(int8_t level_nr)
    {
        if (level_nr == 5)
        {
            _endScene = true;
            return;
        }
        else if (level_nr < 1)
        {
            level_nr = 1;
        }
        else if (level_nr > 4)
        {
            level_nr = 4;
        }

        _pnk.getGear().fade(FADE_COL, FADE_STEP, [=](){
#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("Changing level to %d\n\r", level_nr);
#endif
            freeCurrentLevel();

            // reset current level and room to new ones
            _pnk._gamestate.saved_room = 0;
            _pnk._gamestate.saved_level = level_nr;
            _pnk._removed_sprites.clear();

            saveGamestate();

            loadLevel(level_nr);
        });
    }

    void GSPlay::checkCheatActivation()
    {
        // inspector gadget for snes, debug menu
        if(_pnk.cheatKeyStream == "XXDLRULL")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("Cheat activated: Back to last room.\r\n");
#endif
            userIsCheating();
            changeRoom(_active_room_index - 1, true);
        }
        else if(_pnk.cheatKeyStream == "XXDLRURR")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("Cheat activated: Forward to next room.\r\n");
#endif
            userIsCheating();
            changeRoom(_active_room_index + 1, true);
        }
        else if(_pnk.cheatKeyStream == "XXDLRUUU")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("Cheat activated: Up to the next level.\r\n");
#endif
            userIsCheating();
            changeLevel(_pnk._gamestate.saved_level + 1);
        }
        else if(_pnk.cheatKeyStream == "XXDLRUDD")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("Cheat activated: Down to the last level.\r\n");
#endif
            userIsCheating();
            changeLevel(_pnk._gamestate.saved_level - 1);
        }
        else if(_pnk.cheatKeyStream == "XXULRDRL")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("Cheat activated: Invincible.\r\n");
#endif
            userIsCheating();
            _pnk._gamestate.invincible = true;
//            saveGamestate();
        }
    }

    void GSPlay::userIsCheating()
    {
        _pnk._gamestate.has_cheated = true;

        spHUDLayer hudl = std::static_pointer_cast<HUDLayer>(_pnk.getGear().getLayerByTypename(dang::Layer::LT_HUDLAYER));
        if(hudl == nullptr)
        {
            return;
        }

        hudl->changeCheatSprite();
    }


    void GSPlay::startBossBattle()
    {
        // activate boss
        _pnk._gamestate.boss_health = BOSS_MAX_HEALTH;
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
        // remove all remaining piggies
        _csl->markRemoveSpritesByTypeNum(ST_PIG_NORMAL);
        _csl->markRemoveSpritesByTypeNum(ST_PIG_CRATE);
        _csl->markRemoveSpritesByTypeNum(ST_PIG_BOMB);
        _csl->markRemoveSpritesByTypeNum(ST_PIG_CANNON);

        // remove also rewards
        _csl->markRemoveSpritesByTypeNum(ST_COIN_SILVER);
        _csl->markRemoveSpritesByTypeNum(ST_COIN_GOLD);
        _csl->markRemoveSpritesByTypeNum(ST_GEM_BLUE);
        _csl->markRemoveSpritesByTypeNum(ST_GEM_GREEN);
        _csl->markRemoveSpritesByTypeNum(ST_GEM_RED);
        _csl->markRemoveSpritesByTypeNum(ST_POTION_BLUE);
        _csl->markRemoveSpritesByTypeNum(ST_POTION_RED);
        _csl->markRemoveSpritesByTypeNum(ST_POTION_GREEN);

        // show doors
        dang::spImgSprLayer mood = std::static_pointer_cast<dang::ImgSprLayer>(_pnk.getGear().getLayerByName(_screenplay->_l_mood_name));
        if(mood != nullptr)
        {
            auto spr = mood->getImgSprByTypeNum(ST_MOOD_DOOR);
            if(spr != nullptr)
            {
                dang::spFullImgSpr fis = std::static_pointer_cast<dang::FullImgSpr>(spr);
                fis->setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{1, 2, 3}, 900, dang::Ease::Linear, 1)));
            }
        }

        auto spr = _csl->getSpriteByTypeNum(ST_LEVEL_TRIGGER);
        if(spr != nullptr)
        {
            auto lvlTrigger = std::static_pointer_cast<LevelTrigger>(spr);
            lvlTrigger->activateTrigger();
        }

        dang::SndGear::playSfx(victory_22050_mono, victory_22050_mono_length, _pnk._prefs.volume_sfx);

        // show end text
        switch (_pnk._gamestate.saved_level)
        {
            case 1:
            default:
                showInfoLayer(false, 10000, str_lvl1_end);
                break;
            case 2:
                showInfoLayer(false, 10000, str_lvl2_end);
                break;
            case 3:
                showInfoLayer(false, 10000, str_lvl3_end);
                break;
        }
    }

    void GSPlay::handleBossHit(PnkEvent& pe)
    {
        int8_t health = _pnk._gamestate.boss_health;

        // the boss can't be more dead than dead
        if (health <= 0)
        {
            return;
        }

        switch(_pnk._gamestate.saved_level)
        {
            case 1:
            default:
                health -= DAMAGE_TO_PIGBOSS;
                break;
            case 2:
                health -= DAMAGE_TO_PIGBOSS1;
                break;
            case 3:
                health -= DAMAGE_TO_PIGBOSS2;
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

        // some fancy stuff
        spMoodies mood = SpriteFactory::CannonmuzzleFromProto(std::static_pointer_cast<Moodies>(_hives["cannonmuzzle"]), pe._pos, pe._to_the_left);
        mood->setPosX(pe._pos.x);
        mood->setPosY(pe._pos.y - 10);
        mood->setTransform(blit::XYSWAP);

        _csl->addSprite((dang::spColSpr)mood);

    }

    void GSPlay::saveGamestate()
    {
        _pnk.saveCurrentGamestate();
    }


    void GSPlay::showInfoLayer(bool pause, uint32_t ttl, const std::string_view &message)
    {
        _csl->setActive(!pause);
        _txtl->setText(message);
        _txtl->setTtl(ttl, std::bind(&GSPlay::hideInfoLayer, this, std::placeholders::_1));
        _txtl->setActive(true);
        _txtl->setVisible(true);
    }

    void GSPlay::showGameOverInfo()
    {
        _csl->setActive(true);
        _txtl->setText(str_game_over);
        _txtl->setTtl(10000, std::bind(&GSPlay::leaveTheGameCallback, this, std::placeholders::_1));
        _txtl->setActive(true);
        _txtl->setVisible(true);
    }

    void GSPlay::hideInfoLayer(blit::Button btn)
    {
        _csl->setActive(true);
        _txtl->setActive(false);
        _txtl->setVisible(false);
    }

    void GSPlay::leaveTheGameCallback(blit::Button btn)
    {

        _leaveGame = true;
    }

    dang::BTNode::Status GSPlay::NTheroInSightH(dang::FullColSpr& s, uint32_t dt)
    {
        if (_spr_hero->isInNormalState())
        {
            float ret = _csl->aaLoSH(s, *_spr_hero.get());

            if (ret != 0)
            {
                s.getNTreeState()->_payload["aaLoSH"] = ret;
                return dang::BTNode::Status::SUCCESS;
            }
        }

        return dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status GSPlay::NTheroInSight(dang::FullColSpr& s, uint32_t dt)
    {
        if (_spr_hero->isInNormalState())
        {
            float ret = _csl->loSHR(s, *_spr_hero.get());

            if (ret != 0)
            {
                s.getNTreeState()->_payload["LoS"] = ret;
                return dang::BTNode::Status::SUCCESS;
            }
        }

        return dang::BTNode::Status::FAILURE;
    }

    dang::BTNode::Status GSPlay::NTheroPos(dang::FullColSpr& s, uint32_t dt)
    {
        if (_spr_hero->isInNormalState())
        {
            dang::Vector2F pos = _spr_hero->getCSPosition();
            dang::Vector2F hr = _spr_hero->getHotrect().center();
            s.getNTreeState()->_payload["hero_x"] = pos.x + hr.x;
            s.getNTreeState()->_payload["hero_y"] = pos.y + hr.y;
            return dang::BTNode::Status::SUCCESS;
        }

        return dang::BTNode::Status::FAILURE;

    }

}

