// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

// DANG includes
#include "Gear.hpp"
#include "Imagesheet.hpp"
#include "Sprite.hpp"
#include "SpriteLayer.hpp"
#include "TileLayer.hpp"
#include "Layer.hpp"
#include "tween/Ease.hpp"
#include "tween/TwAnim.hpp"

#include "GSHome.h"
#include "GSPlay.h"

#include "32blit.hpp"

#include "rsrc/tiles_bg_png.h"
#include "rsrc/menus_png.h"
#include "rsrc/pnk_32_menu.tmx.hpp"

#include <cassert>

namespace pnk
{

    std::shared_ptr<GameState> GSHome::update(dang::Gear &gear, uint32_t time)
    {
        // if button x is pressed load the selected state
        if (blit::buttons.pressed & blit::Button::X)
        {
            if (_sel == PLAY)
            {
                return _gs_play;
            }
            else if (_sel == PREFS)
            {
                // return _gs_prefs;
            }
            else if (_sel == ABOUT)
            {
                // return _gs_about;
            }
        }

        // move selection
        if (blit::buttons.pressed & blit::Button::DPAD_DOWN)
        {
            _btns.at(_sel).btn->removeAnimation(true);
            _btns.at(_sel).btn->_img_index = _btns.at(_sel).img_index;
            _sel = ++_sel % _ESIZE;
            _btns.at(_sel).btn->setAnimation(_btns.at(_sel).anim);
        }
        else if (blit::buttons.pressed & blit::Button::DPAD_UP)
        {
            _btns.at(_sel).btn->removeAnimation(true);
            _btns.at(_sel).btn->_img_index = _btns.at(_sel).img_index;
            _sel = --_sel % _ESIZE;
            _btns.at(_sel).btn->setAnimation(_btns.at(_sel).anim);
        }

        return _gs_home;
    }

    void GSHome::enter(dang::Gear &gear, uint32_t time)
    {
        // set up state
        _lvl = init_pnk_32_menu();
        dang::TmxExtruder tmx_ext(&_lvl);

        dang::RectF vp = {0, 0, 320, 240};
        gear.initLevel(_lvl, vp);
        gear.setActiveWorldSize(vp.w, vp.h);

        // extrude and add imagesheets & spritesheets
        spImagesheet is = tmx_ext.extrudeImagesheet(img_name_bg);
        gear.addImagesheet(is);
        _pnk.loadSurface(is);

        is = tmx_ext.extrudeImagesheet(img_name_spr);
        gear.addImagesheet(is);
        _pnk.loadSurface(is);

        // create background Tilelayer
        spTileLayer tl = tmx_ext.extrudeTileLayer(tmx_bg_layer_name, gear);
        assert(tl != nullptr);
        gear.addLayer(tl);

        // create spritelayer w/o collision detection/resolution
        spSpriteLayer sl = tmx_ext.extrudeSpriteLayer(tmx_obj_layer_name);
        assert(sl != nullptr);
        gear.addLayer(sl);

        // create sprites
        _btns.resize(_ESIZE, {nullptr, nullptr, 0});
        const dang::tmx_objectlayer* ola = tmx_ext.getTmxObjectLayer(tmx_obj_layer_name);
        for (const dang::tmx_spriteobject& so : ola->so)
        {
            spImagesheet is = gear.getImagesheet(_lvl.tilesets[so.tileset].name);
            spSprite spr = std::make_shared<dang::Sprite>(so, is);
            spr->_visible = true;
            spr->_imagesheet = is;
            if (so.type == "play")
            {
                _btns.at(PLAY).btn = spr;
                _btns.at(PLAY).anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{2, 1, 0, 1}, 700, std::unique_ptr<dang::Ease>(new dang::EaseLinear), -1));
                _btns.at(PLAY).img_index = 0;
                spr->setAnimation(_btns.at(PLAY).anim);
            }
            else if (so.type == "prefs")
            {
                _btns.at(PREFS).btn = spr;
                _btns.at(PREFS).anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{5, 4, 3, 4}, 700, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), -1));
                _btns.at(PREFS).img_index = 3;
            }
            else if (so.type == "about")
            {
                _btns.at(ABOUT).btn = spr;
                _btns.at(ABOUT).anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{8, 7, 6, 7}, 700, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), -1));
                _btns.at(ABOUT).img_index = 6;
            }
            sl->addSprite(spr);
        }


        // first screen of tmx
        gear.setViewportPos({0, 0});

    }

    void GSHome::exit(dang::Gear &gear, uint32_t time)
    {
        // remove spritesheets
        spImagesheet is = gear.getImagesheet(img_name_bg);
        _pnk.removeSurface(is);

        is = gear.getImagesheet(img_name_spr);
        _pnk.removeSurface(is);

        // clear gear
        gear.removeImagesheets();
        gear.removeLayers();

    }
}