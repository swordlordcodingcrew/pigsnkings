// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include <Gear.h>
#include <Imagesheet.h>
#include <Sprite.h>
#include <SpriteLayer.h>
#include <TileLayer.h>
#include <Layer.h>
#include <cassert>
#include <tween/Ease.h>
#include <tween/TwAnim.h>
#include <CollisionSprite.h>
#include <iostream>
#include <memory>

#include "GSPlay.h"
#include "GSHome.h"

#include "rsrc/tiles_bg_png.h"
#include "rsrc/players_png.h"
#include "rsrc/pnk_32_lvl1.tmx.hpp"

#include "Hero.h"
#include "Bubble.h"
#include "Enemy.h"
#include "pnk_globals.h"
#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "LevelFlow.h"

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
                std::shared_ptr<Enemy> en = std::make_shared<Enemy>(_prototypes[_active_room_flow->spawn_spr_with_id], _obj_is);
                en->init();
                en->setVelX(-en->getVel().x);
                en->_transform = blit::SpriteTransform::HORIZONTAL;
                _csl->addCollisionSprite(en);
                _spawned++;
                _spawn_ready = false;
            }
        }

        if (_room_transition)
        {
            // viewport follows room center
            dang::Vector2F heropos = _spr_hero->getPos() + _spr_hero->getSize() / 2.0f;

            if (heropos.x > gear.getViewport().x + gear.getViewport().w + _room_buffer && _active_room_center.x + gear.getViewport().w < gear.getWorld().w)
            {
                _active_room_center.x += gear.getViewport().w;
            }
            else if (heropos.x < gear.getViewport().x - _room_buffer && _active_room_center.x - gear.getViewport().x >= gear.getWorld().x)
            {
                _active_room_center.x -= gear.getViewport().w;
            }

            if (heropos.y > gear.getViewport().y + gear.getViewport().h + _room_buffer && _active_room_center.y + gear.getViewport().h < gear.getWorld().h)
            {
                _active_room_center.y += gear.getViewport().h;
            }
            else if (heropos.y < gear.getViewport().y - _room_buffer && _active_room_center.y - gear.getViewport().h >= gear.getWorld().y)
            {
                _active_room_center.y -= gear.getViewport().h;
            }
            gear.follow(_active_room_center);

        }

        return GameState::_gs_play;
    }

    void GSPlay::enter(dang::Gear &gear, uint32_t time)
    {
        // choose level acc. to pnk
        switch(_pnk._active_level)
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

        // choose room acc. to pmk
        _active_room_flow = &_lvl_flow->_roomflows[_pnk._active_room];

        _last_time = blit::now();
        _spawn_ready = true;
        _spawned = 0;

        dang::RectF vp = {0, 0, 320, 240};
        gear.initLevel(_tmx, vp);
        gear.setActiveWorldSize(vp.w, vp.h);

        // extrude and add the imagesheets and load spritesheets
        spImagesheet is = tmx_ext.extrudeImagesheet(_lvl_flow->_is_bg_id);
        gear.addImagesheet(is);
        _pnk.loadSpriteSheet(is);

        _obj_is = tmx_ext.extrudeImagesheet(_lvl_flow->_is_obj_id);
        gear.addImagesheet(_obj_is);
        _pnk.loadSpriteSheet(_obj_is);

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
            spImagesheet is = gear.getImagesheet(_tmx.tilesets[so.tileset].name);
            spCollisionSprite spr;
            if (so.type == "hotrect")
            {
                spr = std::make_shared<dang::CollisionSprite>(so, is);
                spr->_visible = false;
                spr->_imagesheet = nullptr;
                spr->setCOType(dang::CollisionSpriteLayer::COT_RIGID);
                _csl->addCollisionSprite(spr);
            }
            else if (so.type == "floor")
            {
                spr = std::make_shared<dang::CollisionSprite>(so, is);
                spr->_visible = false;
                spr->_imagesheet = nullptr;
                spr->setCOType(dang::CollisionSpriteLayer::COT_RIGID);
                spr->_type_num = TN_HOTRECT;
                _csl->addCollisionSprite(spr);
            }
            else if (so.type == "wall")
            {
                spr = std::make_shared<dang::CollisionSprite>(so, is);
                spr->_visible = false;
                spr->_imagesheet = nullptr;
                spr->setCOType(dang::CollisionSpriteLayer::COT_RIGID);
                spr->_type_num = TN_HOTRECT;
                _csl->addCollisionSprite(spr);
            }
            else if (so.type == "enemy")
            {
                _prototypes[so.id] = so;
            }
            else if (so.type == "hero")
            {
                _spr_hero = std::make_shared<Hero>(so, is);
                _spr_hero->activateState();
                spr = _spr_hero;
                _csl->addCollisionSprite(spr);
            }
            else if (so.type == "bubble")
            {
                _prototypes[so.id] = so;
            }
            else
            {
                std::cout << "sprite in tmx not initialized. id=" << so.id << ", type=" << so.type << std::endl;
            }

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

        // remove spritesheets
        spImagesheet is = gear.getImagesheet(_lvl_flow->_is_bg_id);
        _pnk.removeSpriteSheet(is);

        is = gear.getImagesheet(_lvl_flow->_is_obj_id);
        _pnk.removeSpriteSheet(is);

        // clear gear
        gear.removeImagesheets();
        gear.removeLayers();

    }

    void GSPlay::gameEventReceived(dang::Event &e)
    {
        PnkEvent& pe = static_cast<PnkEvent&>(e);
        if (pe._type == ETG_NEW_BUBBLE)
        {
            dang::tmx_spriteobject so = _prototypes[10];
            std::shared_ptr<Bubble> _new_bubble = std::make_shared<Bubble>(so, _obj_is);
            _new_bubble->setPos(pe._pos);
            _new_bubble->_to_the_left = pe._to_the_left;
            _new_bubble->init();
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

