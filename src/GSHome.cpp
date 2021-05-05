// (c) 2019-21 by SwordLord - the coding crew
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

//#include "rsrc/gfx/tiles_bg_png.h"
#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/gfx/hud_ui.png.h"
#include "rsrc/gfx/castle_decoration_tiles.png.h"
#include "rsrc/gfx/king.png.h"
#include "rsrc/gfx/pig.png.h"
#include "rsrc/gfx/pnk_logo.png.h"
#include "rsrc/gfx/menus.png.h"

#include "rsrc/main_1.tmx.hpp"
#include "tracks/gocryogo.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <sfx/pig_squeal_22050_mono.h>

namespace pnk
{
    std::shared_ptr<GameState> GSHome::update(dang::Gear &gear, uint32_t time)
    {
        // if button x is pressed load the selected state
        if (blit::buttons.pressed & blit::Button::X)
        {
            if (_pnk._prefs.selectedModule == _pnk.PLAY)
            {
                return _gs_play;
            }
            else if (_pnk._prefs.selectedModule == _pnk.PREFS)
            {
                return _gs_prefs;
            }
            else if (_pnk._prefs.selectedModule == _pnk.ABOUT)
            {
                return _gs_about;
            }
        }

        // move selection
        if (blit::buttons.pressed & blit::Button::DPAD_DOWN)
        {
            _btns.at(_pnk._prefs.selectedModule).btn->removeAnimation(true);
            _btns.at(_pnk._prefs.selectedModule).btn->_img_index = _btns.at(_pnk._prefs.selectedModule).img_index;
            _pnk._prefs.selectedModule = ++_pnk._prefs.selectedModule % _pnk.ENDOF_SELECTION;

            positionCandles();
        }
        else if (blit::buttons.pressed & blit::Button::DPAD_UP)
        {
            _btns.at(_pnk._prefs.selectedModule).btn->removeAnimation(true);
            _btns.at(_pnk._prefs.selectedModule).btn->_img_index = _btns.at(_pnk._prefs.selectedModule).img_index;
            if(_pnk._prefs.selectedModule == 0) {
                _pnk._prefs.selectedModule = _pnk.ENDOF_SELECTION - 1;
            }
            else {
                _pnk._prefs.selectedModule = --_pnk._prefs.selectedModule % _pnk.ENDOF_SELECTION;
            }

            positionCandles();
        }

        return _gs_home;
    }

    void GSHome::positionCandles()
    {
        if(_sprLeftCandle != nullptr)
        {
            _sprLeftCandle->setPosY(_btns.at(_pnk._prefs.selectedModule).btn->getPosY());
        }
        if(_sprRightCandle != nullptr)
        {
            _sprRightCandle->setPosY(_btns.at(_pnk._prefs.selectedModule).btn->getPosY());
        }
    }

    void GSHome::enter(dang::Gear &gear, uint32_t time)
    {
        blit::debugf("entering\r\n");

        // set up music
        // Setup channel

        // set up state
        _tmx = &main_1_level;
        dang::TmxExtruder tmx_ext(_tmx);

        blit::debugf("extruded\r\n");

        dang::RectF vp = {0, 0, 320, 240};

        blit::debugf("init level\r\n");
        gear.initLevel(_tmx, vp);

        blit::debugf("set active world size\r\n");
        gear.setActiveWorldSize(vp.w, vp.h);

        blit::debugf("init image sheets\r\n");
        tmx_ext.getImagesheets(gear);

        blit::debugf("image sheets initialised\r\n");

        // create background Tilelayer
        spTileLayer tl = tmx_ext.getTileLayer(tmx_bg_layer_name, gear, true);

        blit::debugf("tile layer\r\n");

        spSpriteLayer dl = tmx_ext.getSpriteLayer(tmx_deco_layer_name, gear, false, true);

        blit::debugf("sprite layer\r\n");

        // create spritelayer w/o collision detection/resolution
        spSpriteLayer sl = tmx_ext.getSpriteLayer(tmx_obj_layer_name, gear, false, true);

        blit::debugf("auto layers done\r\n");

        for (size_t j = 0; j < dl->_tmx_layer->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = dl->_tmx_layer->spriteobjects + j;

            spImagesheet is = gear.getImagesheet(so->tileset);
            spSprite spr = std::make_shared<dang::Sprite>(so, is);
            spr->_visible = true;
            spr->_imagesheet = is;
            if (so->type == "candle")
            {
                spr->setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{5, 6, 7, 15, 16, 17}, 700, dang::Ease::Linear, -1)));

                if(so->name == "leftcandle")
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

        blit::debugf("buttons\r\n");

        // make sure to resize the buttons array to the correct size
        _btns.resize(_pnk.ENDOF_SELECTION, {nullptr, nullptr, 0});

        // create sprites
        for (size_t j = 0; j < sl->_tmx_layer->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = sl->_tmx_layer->spriteobjects + j;

            spImagesheet is = gear.getImagesheet(so->tileset);
            spSprite spr{nullptr};
            // buttons
            if(so->type == "button")
            {
                spr = std::make_shared<dang::Sprite>(so, is);
                spr->_visible = true;
                spr->_imagesheet = is;
                if (so->type == "button" && so->name == "play")
                {
                    _btns.at(_pnk.PLAY).btn = spr;
                    _btns.at(_pnk.PLAY).anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{2, 1, 0, 1}, 700, dang::Ease::Linear, -1));
                    _btns.at(_pnk.PLAY).img_index = 1;
                }
                else if (so->type == "button" && so->name == "prefs")
                {
                    _btns.at(_pnk.PREFS).btn = spr;
                    _btns.at(_pnk.PREFS).anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{2, 1, 0, 1}, 700, dang::Ease::Linear, -1));
                    _btns.at(_pnk.PREFS).img_index = 2;
                }
                else if (so->type == "button" && so->name == "about")
                {
                    _btns.at(_pnk.ABOUT).btn = spr;
                    _btns.at(_pnk.ABOUT).anim = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{2, 1, 0, 1}, 700, dang::Ease::Linear, -1));
                    _btns.at(_pnk.ABOUT).img_index = 0;
                }
            }
            else if (so->name == "piggie")
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
            else if (so->name == "hero")
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
                twPos->setFinishedCallback(std::bind(&GSHome::playOink, this));
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

        positionCandles();

        // first screen of tmx
        gear.setViewportPos({0, 8});

        blit::debugf("entered\r\n");

    }

    void GSHome::exit(dang::Gear &gear, uint32_t time)
    {
        _btns.clear();
        _sprLeftCandle.reset();
        _sprRightCandle.reset();

        _pnk.removeImagesheets();

        // clear gear
        gear.removeLayers();
    }

    void GSHome::playOink()
    {
        // TODO: only play it sometimes
        //PigsnKings::playSfx(pig_squeal_22050, pig_squeal_22050_length);
    }
}