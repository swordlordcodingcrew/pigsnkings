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

#include "rsrc/tiles_bg_png.h"
#include "rsrc/pnk_sprites_png.h"
#include "rsrc/pnk_32_lvl1.tmx.hpp"

#include "tracks/gocryogo.h"

#include "Hero.h"
#include "Bubble.h"
#include "Enemy.h"
#include "pnk_globals.h"
#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "LevelFlow.h"
#include "SpriteFactory.hpp"

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
            if (_spawned >= _active_room_flow->number_of_spawns)
            {
                // end of room
                // TODO: goto next room
                _room_transition = true;
                spLayer l = gear.getLayerByName(_lvl_flow->_l_obj_id);
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
                spEnemy en = SpriteFactory::NormalPig(_prototypes[_active_room_flow->spawn_spr_with_id], _iss_for_prototypes[_active_room_flow->spawn_spr_with_id]);
                en->setWalk(E_WALK_VEL);
                _csl->addCollisionSprite(en);
                _spawned++;
                _spawn_ready = false;
            }
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

        // choose level acc. to pnk
        switch(_pnk._prefs.active_level)
        {
            case 1:
            default:
                _lvl_flow = std::make_shared<Level1Flow>();
                _tmx = init_pnk_32_lvl1();
                break;
            case 2:
                // other levels?
                //_lvl_flow = std::make_shared<Level2Flow>();
                //_tmx = init_pnk_32_lvl2();
                break;
        }

        dang::TmxExtruder tmx_ext(&_tmx);

        // choose room acc. to prefs
        _active_room_flow = &_lvl_flow->_roomflows[_pnk._prefs.active_room];

        _last_time = blit::now();
        _spawn_ready = true;
        _spawned = 0;

        dang::RectF vp = {0, 0, 320, 240};
        gear.initLevel(_tmx, vp);
        gear.setActiveWorldSize(vp.w, vp.h);

        // init imagesheets
        _pnk.initImageSheets(tmx_ext);

        // create background Tilelayer
        spTileLayer tl = tmx_ext.extrudeTileLayer(_lvl_flow->_l_bg_id, gear);
        gear.addLayer(tl);

        // create spritelayer with collision detection
        _csl = tmx_ext.extrudeCollisionSpriteLayer(_lvl_flow->_l_obj_id);
        gear.addLayer(_csl);

        // create sprites
        const dang::tmx_objectlayer* ola = tmx_ext.getTmxObjectLayer(_lvl_flow->_l_obj_id);
        for (const dang::tmx_spriteobject& so : ola->so)
        {
            bool orphaned = true;
            spImagesheet is = gear.getImagesheet(_tmx.tilesets[so.tileset].name);
            spCollisionSprite spr;
            if      (so.type == SpriteFactory::T_HOTRECT)           { spr = SpriteFactory::Hotrect(so);         orphaned = false; }
            else if (so.type == SpriteFactory::T_HOTRECT_PLATFORM)  { spr = SpriteFactory::HotrectPlatform(so); orphaned = false; }
            else if (so.type == SpriteFactory::T_NORMAL_PIG)        { spr = SpriteFactory::NormalPig(so, is);   orphaned = false; }
            else if (so.type == SpriteFactory::T_KING)
            {
                _spr_hero = SpriteFactory::King(so, is);
                spr = _spr_hero;
                orphaned = false;
            }
            _csl->addCollisionSprite(spr);

            if (so.type == SpriteFactory::T_BUBBLE_PROTO)
            {
                _prototypes[so.id] = so;
                _iss_for_prototypes[so.id] = is;
                orphaned = false;
            }

            if (orphaned) std::cout << "sprite type unknown. Id=" << so.id << ", type=" << so.type << std::endl;

        }

        // set viewport to active room
        _active_room_center = _active_room_flow->room_center;
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

    void GSPlay::gameEventReceived(dang::Event &e)
    {
        PnkEvent& pe = static_cast<PnkEvent&>(e);
        if (pe._type == ETG_NEW_BUBBLE)
        {
            dang::tmx_spriteobject so = _prototypes[30];
            dang::spImagesheet is = _iss_for_prototypes[so.id];
            so.x = (int32_t)pe._pos.x;
            so.y = (int32_t)pe._pos.y;
            spBubble _new_bubble = SpriteFactory::Bubble(so, is, pe._to_the_left);
//            _new_bubble->setPos(pe._pos);
//            _new_bubble->_to_the_left = pe._to_the_left;
//            _new_bubble->setMovement();
            _csl->addCollisionSprite(_new_bubble);
//            std::cout << "new bubble event" << std::endl;
        }
        else if (pe._type == ETG_REMOVE_SPRITE)
        {
            std::shared_ptr<dang::Sprite> spr = pe._spr.lock();
            if (spr != nullptr)
            {
                _csl->removeSprite(pe._spr.lock());
                std::cout << "remove sprite event" << std::endl;
                if (spr->_id == _active_room_flow->spawn_spr_with_id)
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

