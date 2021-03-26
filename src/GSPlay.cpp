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

#include "tracks/gocryogo.h"

#include "Hero.h"
#include "Bubble.h"
#include "Enemy.h"
#include "actors/npc/HenchPig.h"
#include "actors/npc/Throwies.h"
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


        updateVpPos();
        gear.follow(_vp_pos);

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
        _active_act_index = _pnk._prefs.active_room;
        changeRoom(_active_act_index);

        _last_time = blit::now();

        dang::RectF vp = {0, 0, 320, 240};
        gear.initLevel(_tmx, vp);
        gear.setActiveWorldSize(vp.w + 16, vp.h + 16);

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
            else if (so.type == SpriteFactory::T_ROOM_TRIGGER)      { spr = SpriteFactory::RoomTrigger(so); }
            else if (so.type == SpriteFactory::T_PIG_NORMAL)        { spr = SpriteFactory::NormalPig(txtr, so, is); }
// temporary disable PigBomb and PigCrate
//            else if (so.type == SpriteFactory::T_PIG_BOMB)          { spr = SpriteFactory::PigBomb(txtr, so, is); }
//            else if (so.type == SpriteFactory::T_PIG_BOX)           { spr = SpriteFactory::PigCrate(txtr, so, is); }
            else if (so.type == SpriteFactory::T_PIG_BOMB)          { spr = SpriteFactory::NormalPig(txtr, so, is); }
            else if (so.type == SpriteFactory::T_PIG_BOX)           { spr = SpriteFactory::NormalPig(txtr, so, is); }

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
                else if (so.type == SpriteFactory::T_CRATE_PROTO)
                {
                    spCollisionSprite sprc = SpriteFactory::Crate(txtr, so, is, false);
                    assert(sprc != nullptr);
                    _hives["crate"] = sprc;
                }
                else
                {
                        std::cout << "sprite type unknown. Id=" << so.id << ", type=" << so.type << std::endl;
                }
            }
        }

        // create HUD layer
        spHUDLayer hudl = std::make_shared<HUDLayer>();
        if (!_screenplay->_l_hud_name.empty()) txtr.fillHUDLayer(hudl, _screenplay->_l_hud_name, gear, true, true);


        // set viewport to active room
        updateVpPos();
        gear.setViewportPos(_vp_pos - dang::Vector2F(160, 120));

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

//         _pnk._prefs.active_room = _active_act_index;

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
            }
            else
            {
                // TODO if it is stale, we should retry? or wait? or...
                std::cout << "attempted to remove stale sprite" << std::endl;
            }
        }
        else if (pe._type == ETG_NEW_THROWN_CRATE)
        {
            spThrowies crate_proto = std::dynamic_pointer_cast<Throwies>(_hives["crate"]);
            assert(crate_proto != nullptr);
            spThrowies crate = std::make_shared<Throwies>(*crate_proto);
            crate->setPos(pe._pos);
            crate->_to_the_left = pe._to_the_left;
            crate->init();
            _csl->addCollisionSprite(crate);
        }
        else if (pe._type == ETG_CHANGE_ROOM)
        {
            if (pe._pos.x != _active_act_index)
            {
                _active_act_index = pe._pos.x;
                changeRoom(_active_act_index);
            }
        }
    }

    void GSPlay::updateVpPos()
    {
        // viewport follows hero within room
        dang::Vector2F pos = _spr_hero->getPos() + _spr_hero->getSize() / 2.0f;

        if (pos.x < _room_extent.left() + 160)
        {
            _vp_pos.x = _room_extent.left() + 160;
        }
        else if (pos.x > _room_extent.right() - 160)
        {
            _vp_pos.x = _room_extent.right() - 160;
        }
        else
        {
            _vp_pos.x = pos.x;
        }

        if (pos.y < _room_extent.top() + 120)
        {
            _vp_pos.y = _room_extent.top() + 120;
        }
        else if (pos.y > _room_extent.bottom() - 120)
        {
            _vp_pos.y = _room_extent.bottom() - 120;
        }
        else
        {
            _vp_pos.y = pos.y;
        }
    }

    void GSPlay::changeRoom(uint32_t room_nr)
    {
        assert(room_nr < _screenplay->_acts.size());

        _active_act = &_screenplay->_acts[room_nr];
        // initialize room size
        _room_extent.x = _active_act->_extent.x * _tmx.w.tileWidth;
        _room_extent.y = _active_act->_extent.y * _tmx.w.tileHeight;
        _room_extent.w = _active_act->_extent.w * _tmx.w.tileWidth;
        _room_extent.h = _active_act->_extent.h * _tmx.w.tileHeight;

    }



}

