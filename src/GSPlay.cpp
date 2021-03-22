// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <Gear.hpp>
#include <Imagesheet.hpp>
#include <Sprite.hpp>
#include <SpriteLayer.hpp>
#include <TileLayer.hpp>
#include <Layer.hpp>
#include <cassert>
#include <tween/Ease.hpp>
#include <tween/TwAnim.hpp>
#include <CollisionSprite.hpp>
#include <iostream>
#include <memory>

#include "GSPlay.h"
#include "GSHome.h"

#include "rsrc/gfx/pig_box.png.h"
#include "rsrc/gfx/pig_bomb.png.h"
#include "rsrc/gfx/bubble.png.h"
#include "rsrc/gfx/bubble.png.h"
#include "rsrc/gfx/items.png.h"
#include "rsrc/gfx/king.png.h"
#include "rsrc/gfx/castle_decoration_tiles.png.h"
#include "rsrc/gfx/pig.png.h"
#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/gfx/hud_ui.png.h"
#include "rsrc/level_1.tmx.hpp"

#include "tracks/gocryogo.h"

#include "Hero.h"
#include "Bubble.h"
#include "Enemy.h"
#include "actors/npc/HenchPig.h"
#include "pnk_globals.h"
#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "ScreenPlay.h"
#include "SpriteFactory.hpp"
#include "HUDLayer.hpp"

namespace pnk
{
    std::shared_ptr<GameState> GSPlay::update(dang::Gear &gear, uint32_t time)
    {
        if (blit::buttons.pressed & blit::Button::HOME)
        {
            return GameState::_gs_home;
        }
        else if (blit::buttons.pressed & blit::Button::MENU)
        {
//            return GameState::_gs_prefs;
        }

        // flow stuff
        if (_spawn_ready)
        {
            if (_spawned >= _active_act->number_of_spawns)
            {
                // end of room
                // TODO: goto next room
                _room_transition = true;
                spLayer l = gear.getLayerByName(_screenplay->_l_obj_name);
                if (l != nullptr)
                {
                    spCollisionSpriteLayer csl = std::static_pointer_cast<dang::CollisionSpriteLayer>(l);
                    csl->removeSpriteById(24);
                }

//                return GameState::_gs_home;
            }
            else if (blit::now() - _last_time > _spawn_delay)
            {
                // still enemies to go
/*                dang::TmxExtruder txtr = dang::TmxExtruder(&_tmx);
                spEnemy en = SpriteFactory::NormalPig(txtr, _prototypes[_active_room_flow->spawn_spr_with_id], _iss_for_prototypes[_active_room_flow->spawn_spr_with_id]);
                en->setWalk(E_WALK_VEL);
                _csl->addCollisionSprite(en);
                _spawned++;
                _spawn_ready = false;
*/            }
        }

        if (_room_transition)
        {
            bool triggered{false};

            // viewport follows room center
            dang::Vector2F heropos = _spr_hero->getPos() + _spr_hero->getSize() / 2.0f;

            if (heropos.x > gear.getViewport().x + gear.getViewport().w + _room_buffer && _active_room_center.x + gear.getViewport().w < gear.getWorld().w)
            {
                _active_room_center.x += gear.getViewport().w;
                triggered = true;
            }
            else if (heropos.x < gear.getViewport().x - _room_buffer && _active_room_center.x - gear.getViewport().x >= gear.getWorld().x)
            {
                _active_room_center.x -= gear.getViewport().w;
                triggered = true;
            }

            if (heropos.y > gear.getViewport().y + gear.getViewport().h + _room_buffer && _active_room_center.y + gear.getViewport().h < gear.getWorld().h)
            {
                _active_room_center.y += gear.getViewport().h;
                triggered = true;
            }
            else if (heropos.y < gear.getViewport().y - _room_buffer && _active_room_center.y - gear.getViewport().h >= gear.getWorld().y)
            {
                _active_room_center.y -= gear.getViewport().h;
                triggered = true;
            }

            if (triggered)
            {
                // TODO: make transition to new room
            }
        }
        gear.follow(_active_room_center);

        return GameState::_gs_play;
    }

    void GSPlay::enter(dang::Gear &gear, uint32_t time)
    {
        PigsnKings::playMod(gocryogo_mod, gocryogo_mod_length);

        initGameVars();

        // choose level acc. to pnk
        switch(_pnk._prefs.active_level)
        {
            case 1:
            default:
                _screenplay = std::make_shared<L1SP>();
                _tmx = init_level_1();
                break;
        }

        dang::TmxExtruder txtr(&_tmx);

        // choose room acc. to prefs
        _active_act = &_screenplay->_acts[_pnk._prefs.active_room];

        _last_time = blit::now();
        _spawn_ready = true;
        _spawned = 0;

        dang::RectF vp = {0, 0, 320, 240};
        gear.initLevel(_tmx, vp);
        gear.setActiveWorldSize(vp.w, vp.h);

        // init imagesheets
        _pnk.initImageSheets(txtr);

        // create background Tilelayer
        txtr.getTileLayer(_screenplay->_l_bg_name, gear, true);

        // create mood Tilelayer
        if (!_screenplay->_l_mood_name.empty()) txtr.getSpriteLayer(_screenplay->_l_mood_name, gear, true, true);

        // create Spritelayer with collision detection
        _csl = txtr.getCollisionSpriteLayer(_screenplay->_l_obj_name, gear, false, true);

        // create sprites
        for (const dang::tmx_spriteobject& so : txtr.getSOList(_csl))
        {
            spImagesheet is = gear.getImagesheet(_tmx.tilesets[so.tileset].name);
            spCollisionSprite spr = nullptr;
            if      (so.type == SpriteFactory::T_HOTRECT)           { spr = SpriteFactory::Hotrect(so); }
            else if (so.type == SpriteFactory::T_HOTRECT_PLATFORM)  { spr = SpriteFactory::HotrectPlatform(so); }
            else if (so.type == SpriteFactory::T_PIG_NORMAL)        { spr = SpriteFactory::NormalPig(txtr, so, is); }
            else if (so.type == SpriteFactory::T_PIG_BOMB)          { spr = SpriteFactory::PigBomb(txtr, so, is); }
            else if (so.type == SpriteFactory::T_PIG_BOX)           { spr = SpriteFactory::PigCrate(txtr, so, is); }
            else if (so.type == SpriteFactory::T_COIN_SILVER)       { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so.type == SpriteFactory::T_COIN_GOLD)         { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so.type == SpriteFactory::T_GEM_BLUE)          { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so.type == SpriteFactory::T_GEM_GREEN)         { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so.type == SpriteFactory::T_GEM_RED)           { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so.type == SpriteFactory::T_POTION_BLUE)       { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so.type == SpriteFactory::T_POTION_RED)        { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so.type == SpriteFactory::T_POTION_GREEN)      { spr = SpriteFactory::Reward(txtr, so, is); }
            else if (so.type == SpriteFactory::T_KING)
            {
                _spr_hero = SpriteFactory::King(txtr, so, is);
                spr = _spr_hero;
            }
            else
            {
                std::cerr << "type unknown: " << so.type << std::endl;
            }

            if (spr != nullptr)
            {
                _csl->addCollisionSprite(spr);
            }
            else
            {
                if (so.type == SpriteFactory::T_BUBBLE_PROTO)
                {
                    spCollisionSprite sprc = SpriteFactory::Bubble(txtr, so, is, false);
                    assert(sprc != nullptr);
                    _hives["bubble"] = sprc;
                }
                else
                {
                        std::cout << "sprite type unknown. Id=" << so.id << ", type=" << so.type << std::endl;
                }
            }
        }

        // create HUD layer
        //if (!_lvl_flow->_l_hud_name.empty()) txtr.getSpriteLayer(_lvl_flow->_l_hud_name, gear, true, true);
        spHUDLayer hudl = std::make_shared<HUDLayer>();
        if (!_screenplay->_l_hud_name.empty()) txtr.fillHUDLayer(hudl, _screenplay->_l_hud_name, gear, true, true);

        // set viewport to active room
        _active_room_center = _active_act->room_center;
        gear.setViewportPos(_active_room_center - dang::Vector2F(160, 120));

        // add event callback
        std::function<void (dang::Event&)> func = std::bind(&GSPlay::gameEventReceived, this, std::placeholders::_1);
        _sub_ref = _pnk._dispatcher.registerSubscriber(func, EF_GAME);

    }

    void GSPlay::exit(dang::Gear &gear, uint32_t time)
    {
        // remove callback
        _pnk._dispatcher.removeSubscriber(_sub_ref);

        _pnk.removeImagesheets();

        // clear gear
        gear.removeLayers();

        PigsnKings::stopMod();

    }

    void GSPlay::initGameVars()
    {
        _pnk.initGameVars();
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
        }
        else if (pe._type == ETG_REMOVE_SPRITE)
        {
            std::shared_ptr<dang::Sprite> spr = pe._spr.lock();
            if (spr != nullptr)
            {
                _csl->removeSprite(pe._spr.lock());
                std::cout << "remove sprite event" << std::endl;
                if (spr->_id == _active_act->spawn_spr_with_id)
                {
                    _last_time = blit::now();
                    _spawn_ready = true;
                }
            }
            else
            {
                std::cout << "attempted to remove stale sprite" << std::endl;
            }
        }
    }
}

