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
#include "snd/SndGear.hpp"
#include "tween/TwPos.hpp"
#include "tween/TwSequence.hpp"

#include "GSHome.h"
#include "GSPlay.h"
#include "GSPrefs.h"
#include "GSAbout.h"

#include "32blit.hpp"

#include "rsrc/tiles_bg_png.h"
#include "rsrc/castle_tiles.png.h"
#include "rsrc/hud_ui.png.h"
#include "rsrc/castle_decoration_tiles.png.h"
#include "rsrc/king.png.h"
#include "rsrc/pig.png.h"
#include "rsrc/pnk_logo.png.h"
#include "rsrc/menus.png.h"

#include "rsrc/main_1.tmx.hpp"
#include "tracks/gocryogo.h"

#include <cassert>
#include <iostream>
#include <sstream>

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
                return _gs_prefs;
            }
            else if (_sel == ABOUT)
            {
                return _gs_about;
            }
        }

        // move selection
        if (blit::buttons.pressed & blit::Button::DPAD_DOWN)
        {
            _btns.at(_sel).btn->removeAnimation(true);
            _btns.at(_sel).btn->_img_index = _btns.at(_sel).img_index;
            _sel = ++_sel % _ESIZE;
            //_btns.at(_sel).btn->setAnimation(_btns.at(_sel).anim);

            if(_sprLeftCandle != nullptr)
            {
                _sprLeftCandle->setPosY(_btns.at(_sel).btn->getPosY());
            }
            if(_sprRightCandle != nullptr)
            {
                _sprRightCandle->setPosY(_btns.at(_sel).btn->getPosY());
            }
        }
        else if (blit::buttons.pressed & blit::Button::DPAD_UP)
        {
            _btns.at(_sel).btn->removeAnimation(true);
            _btns.at(_sel).btn->_img_index = _btns.at(_sel).img_index;
            if(_sel == 0) {
                _sel = _ESIZE - 1;
            }
            else {
                _sel = --_sel % _ESIZE;
            }
            //_btns.at(_sel).btn->setAnimation(_btns.at(_sel).anim);
            if(_sprLeftCandle != nullptr)
            {
                _sprLeftCandle->setPosY(_btns.at(_sel).btn->getPosY());
            }
            if(_sprRightCandle != nullptr)
            {
                _sprRightCandle->setPosY(_btns.at(_sel).btn->getPosY());
            }
        }

        return _gs_home;
    }

    void GSHome::enter(dang::Gear &gear, uint32_t time)
    {
        // set up music
        // Setup channel

        // set up state
        _lvl = init_main_1();
        dang::TmxExtruder tmx_ext(&_lvl);

        dang::RectF vp = {0, 0, 320, 240};
        gear.initLevel(_lvl, vp);
        gear.setActiveWorldSize(vp.w, vp.h);

        _pnk.initImageSheets(tmx_ext);

        // create background Tilelayer
        spTileLayer tl = tmx_ext.getTileLayer(tmx_bg_layer_name, gear, true);

        spSpriteLayer dl = tmx_ext.getSpriteLayer(tmx_deco_layer_name, gear, false, true);

        // create spritelayer w/o collision detection/resolution
        spSpriteLayer sl = tmx_ext.getSpriteLayer(tmx_obj_layer_name, gear, false, true);

//        const dang::tmx_objectlayer* ol = tmx_ext.getTmxObjectLayer(tmx_deco_layer_name);
        for (const dang::tmx_spriteobject& so : tmx_ext.getSOList(dl))
        {
            spImagesheet is = gear.getImagesheet(_lvl.tilesets[so.tileset].name);
            spSprite spr = std::make_shared<dang::Sprite>(so, is);
            spr->_visible = true;
            spr->_imagesheet = is;
            if (so.type == "candle")
            {
                spr->setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{5, 6, 7, 15, 16, 17}, 700, dang::Ease::Linear, -1)));

                if(so.name == "leftcandle")
                {
                    _sprLeftCandle = spr;
                }
                else
                {
                    _sprRightCandle = spr;
                }
            }
            sl->addSprite(spr);
        }

        // create sprites
        _btns.resize(_ESIZE, {nullptr, nullptr, 0});
  //      const dang::tmx_objectlayer* ola = tmx_ext.getTmxObjectLayer(tmx_obj_layer_name);
        for (const dang::tmx_spriteobject& so : tmx_ext.getSOList(sl))
        {
            spImagesheet is = gear.getImagesheet(_lvl.tilesets[so.tileset].name);
            spSprite spr{nullptr};
            // buttons
            if(so.type == "button"){
                spr = std::make_shared<dang::Sprite>(so, is);
                spr->_visible = true;
                spr->_imagesheet = is;
                if (so.type == "button" && so.name == "play")
                {
                    _btns.at(PLAY).btn = spr;
                    _btns.at(PLAY).anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{2, 1, 0, 1}, 700, dang::Ease::Linear, -1));
                    _btns.at(PLAY).img_index = 1;
                    //spr->setAnimation(_btns.at(PLAY).anim);
                }
                else if (so.type == "button" && so.name == "prefs")
                {
                    _btns.at(PREFS).btn = spr;
                    _btns.at(PREFS).anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{2, 1, 0, 1}, 700, dang::Ease::Linear, -1));
                    _btns.at(PREFS).img_index = 2;
                }
                else if (so.type == "button" && so.name == "about")
                {
                    _btns.at(ABOUT).btn = spr;
                    _btns.at(ABOUT).anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{2, 1, 0, 1}, 700, dang::Ease::Linear, -1));
                    _btns.at(ABOUT).img_index = 0;
                }
            }
            else if (so.name == "piggie")
            {
                spr = std::make_shared<dang::Sprite>(so, is);
                spr->_visible = true;
                spr->_imagesheet = is;
                spr->_transform = blit::SpriteTransform::HORIZONTAL;

                // run piggie run
                spTwAnim anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{6, 7, 8, 9, 10, 11}, 600, dang::Ease::Linear, -1));
                spr->setAnimation(anim);

                // get a sequence and make sure that it is looping forever
                spTwSeq tw_seq_anim = std::make_shared<dang::TwSequence>();
                tw_seq_anim->loops(-1);

                // piggie starting off screen to the left
                spr->setPosX(-32);
                // and moving off screen to the right (on Y set in Tiled)
                dang::Vector2F  _move_to{320, spr->getPosY()};

                // total duration of 4000
                std::shared_ptr<dang::TwPos> twPos = std::make_shared<dang::TwPos>(_move_to, 3000, dang::Ease::Linear, 0, false);
                // make sure to tell the twPos who is its sprite (to initialise the starting pos) or it will take 0,0 as base...
                twPos->init(spr.get());
                tw_seq_anim->addTween(twPos);

                spTwNull nullTw = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 0));
                tw_seq_anim->addTween(nullTw);

                spr->addTween(tw_seq_anim);
            }
            else if (so.name == "hero")
            {
                spr = std::make_shared<dang::Sprite>(so, is);
                spr->_visible = true;
                spr->_imagesheet = is;

                // run hero, run
                spTwAnim anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{6, 7, 8, 9, 10, 11}, 600, dang::Ease::Linear, -1));
                spr->setAnimation(anim);

                // hero starting off screen to the left
                spr->setPosX(-32);
                // and moving off screen to the right (on Y set in Tiled)
                dang::Vector2F  _move_to{320, spr->getPosY()};

                // get a sequence and make sure that it is looping forever
                spTwSeq tw_seq_anim = std::make_shared<dang::TwSequence>();
                tw_seq_anim->loops(-1);

                // total duration of 4000
                spTwNull pauseBefore = std::make_shared<dang::TwNull>(dang::TwNull(600, dang::Ease::Linear, 0));
                tw_seq_anim->addTween(pauseBefore);

                std::shared_ptr<dang::TwPos> twPos = std::make_shared<dang::TwPos>(_move_to, 2600, dang::Ease::Linear, 0, false);
                // make sure to tell the twPos who is its sprite (to initialise the starting pos) or it will take 0,0 as base...
                twPos->init(spr.get());
                tw_seq_anim->addTween(twPos);

                spTwNull pauseAfter = std::make_shared<dang::TwNull>(dang::TwNull(800, dang::Ease::Linear, 0));
                tw_seq_anim->addTween(pauseAfter);

                spr->addTween(tw_seq_anim);
            }

            // and add it to the collection
            if(spr != nullptr){
                sl->addSprite(spr);
            }
        }

        // first screen of tmx
        gear.setViewportPos({0, 8});
    }

    void GSHome::exit(dang::Gear &gear, uint32_t time)
    {
        _pnk.removeImagesheets();

        // clear gear
        gear.removeLayers();
    }
}