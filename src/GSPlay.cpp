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

        // viewport follows room center
        dang::Vector2F heropos = _spr_hero->getPos() + _spr_hero->getSize() / 2.0f;

        if (heropos.x > gear.getViewport().x + gear.getViewport().w + _room_buffer && _room_center.x + gear.getViewport().w < gear.getWorld().w)
        {
            _room_center.x += gear.getViewport().w;
        }
        else if (heropos.x < gear.getViewport().x - _room_buffer && _room_center.x - gear.getViewport().x >= gear.getWorld().x)
        {
            _room_center.x -= gear.getViewport().w;
        }

        if (heropos.y > gear.getViewport().y + gear.getViewport().h + _room_buffer && _room_center.y + gear.getViewport().h < gear.getWorld().h)
        {
            _room_center.y += gear.getViewport().h;
        }
        else if (heropos.y < gear.getViewport().y - _room_buffer && _room_center.y - gear.getViewport().h >= gear.getWorld().y)
        {
            _room_center.y -= gear.getViewport().h;
        }
        gear.follow(_room_center);

        return GameState::_gs_play;
    }

    void GSPlay::enter(dang::Gear &gear, uint32_t time)
    {
        _tmx = init_pnk_32_lvl1();
        dang::TmxExtruder tmx_ext(&_tmx);

        dang::RectF vp = {0, 0, 320, 240};
        gear.initLevel(_tmx, vp);
        gear.setActiveWorldSize(vp.w, vp.h);

        // extrude and add the imagesheets
        spImagesheet is = tmx_ext.extrudeImagesheet(img_name_bg);
        gear.addImagesheet(is);
        _pnk.loadSpriteSheet(is);

        _obj_is = tmx_ext.extrudeImagesheet(img_name_obj);
        gear.addImagesheet(_obj_is);
        _pnk.loadSpriteSheet(_obj_is);

        // create background Tilelayer
        spTileLayer tl = tmx_ext.extrudeTileLayer(bg_name, gear);
        gear.addLayer(tl);

        // create spritelayer with collision detection
        _csl = tmx_ext.extrudeCollisionSpriteLayer(playfield_name);
        gear.addLayer(_csl);

        // create sprites
        const dang::tmx_objectlayer* ola = tmx_ext.getTmxObjectLayer(playfield_name);
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
                std::shared_ptr<Enemy> en = std::make_shared<Enemy>(so, is);
                en->init();
                spr = en;
                _csl->addCollisionSprite(spr);
            }
            else if (so.type == "hero")
            {
                _spr_hero = std::make_shared<Hero>(so, is);
                _spr_hero->activateState();

/*                // add eventreceived() as receiver of events
                std::function<void (dang::Event&)> func = std::bind(&Hero::gameEventReceived, _spr_hero, std::placeholders::_1);
                _subref_hero = pnk::_pnk._dispatcher.registerSubscriber(func, EF_APPL);
*/

                spr = _spr_hero;
                _csl->addCollisionSprite(spr);
            }
            else if (so.type == "bubble")
            {
                // create prototype bubble to be copied, when hero is bubbling
                _bubble_prototype = so;
            }
/*            else
            {
                spr->_visible = false;
                spr->_imagesheet = nullptr;
            }
*/
        }

        // not very nice way to define the room
        _room_center = {vp.w/2, 960 - 120};
        gear.setViewportPos(_room_center - dang::Vector2F(160, 120));
//        dang::RectF r1 = gear.getActiveWorld();

        std::function<void (dang::Event&)> func = std::bind(&GSPlay::gameEventReceived, this, std::placeholders::_1);
        _sub_ref = _pnk._dispatcher.registerSubscriber(func, EF_GAME);

    }

    void GSPlay::exit(dang::Gear &gear, uint32_t time)
    {
        _pnk._dispatcher.removeSubscriber(_sub_ref);
        // remove spritesheets
        spImagesheet is = gear.getImagesheet(img_name_bg);
        _pnk.removeSpriteSheet(is);

        is = gear.getImagesheet(img_name_obj);
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
            std::shared_ptr<Bubble> _new_bubble = std::make_shared<Bubble>(_bubble_prototype, _obj_is);
            _new_bubble->setPos(pe._pos);
            _new_bubble->_to_the_left = pe._to_the_left;
            _new_bubble->init();
            _csl->addCollisionSprite(_new_bubble);
            std::cout << "new bubble event" << std::endl;
        }
        else if (pe._type == ETG_REMOVE_SPRITE)
        {
            _csl->removeSprite(pe._spr.lock());
            std::cout << "remove sprite event" << std::endl;
        }
    }
}

