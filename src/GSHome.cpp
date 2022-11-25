// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game

#include "GSHome.h"
#include "pigsnkings.hpp"

#include "GSPlay.h"
#include "GSPrefs.h"
#include "GSAbout.h"
#include "GSEndScene.hpp"

#include "32blit.hpp"

#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/gfx/hud_ui.png.h"
#include "rsrc/gfx/castle_decoration_tiles.png.h"
#include "rsrc/gfx/king.png.h"
#include "rsrc/gfx/pig.png.h"
#include "rsrc/gfx/pnk_logo.png.h"
#include "rsrc/gfx/menus.png.h"
#include "rsrc/main_1.tmx.hpp"
#include "tracks/etheric_xm.h"
#include "sfx/pig_squeal_22050_mono.h"
#include "sfx/cheat_22050_mono.h"

// DANG includes
#include <Gear.hpp>
#include <Imagesheet.hpp>
#include <layer/ImgSprLayer.hpp>
#include <sprite/ImgSpr.hpp>
#include <sprite/FullImgSpr.hpp>
#include <Rand.hpp>
#include <layer/TileLayer.hpp>
#include <tween/Ease.hpp>
#include <tween/TwAnim.hpp>
#include <tween/TwPos.hpp>
#include <tween/TwSequence.hpp>
#include <tween/TwNull.hpp>
#include <snd/SndGear.hpp>
#include <tracks/paperbird.h>

#include <cassert>
#include <malloc.h>

namespace pnk
{
    extern PigsnKings _pnk;

    std::shared_ptr<GameState> GSHome::update(dang::Gear &gear, uint32_t time)
    {
        updateCheatKeyStream(blit::buttons.pressed);
        // checkCheatActivation(); // not used, there are no cheats in home

        if (blit::buttons.pressed & BTN_OK)
        {
            if (_pnk._prefs.selectedModule == _pnk.PLAY)
            {
                return _gs_play;
//                return _gs_end_scene;
            }
            else if (_pnk._prefs.selectedModule == _pnk.PREFS)
            {
                return _gs_prefs;
            }
            else if (_pnk._prefs.selectedModule == _pnk.ABOUT)
            {
                return _gs_about;
//                return _gs_end_scene;
            }
        }

        // move selection
        if (blit::buttons.pressed & blit::Button::DPAD_DOWN)
        {
            _btns.at(_pnk._prefs.selectedModule).btn->removeAnimation(true);
            _btns.at(_pnk._prefs.selectedModule).btn->setImgIndex(_btns.at(_pnk._prefs.selectedModule).img_index);
            ++_pnk._prefs.selectedModule;
            _pnk._prefs.selectedModule = _pnk._prefs.selectedModule % _pnk.ENDOF_SELECTION;

            positionCandles();
        }
        else if (blit::buttons.pressed & blit::Button::DPAD_UP)
        {
            _btns.at(_pnk._prefs.selectedModule).btn->removeAnimation(true);
            _btns.at(_pnk._prefs.selectedModule).btn->setImgIndex(_btns.at(_pnk._prefs.selectedModule).img_index);
            if(_pnk._prefs.selectedModule == 0) {
                _pnk._prefs.selectedModule = _pnk.ENDOF_SELECTION - 1;
            }
            else {
                --_pnk._prefs.selectedModule;
                _pnk._prefs.selectedModule = _pnk._prefs.selectedModule % _pnk.ENDOF_SELECTION;
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

    void GSHome::enter(dang::Gear& gear, uint32_t time)
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: entering\n");
        // set up music
        DEBUG_PRINT("GSHome: load track (%d)\n", mallinfo().uordblks);
#endif
        if (!dang::SndGear::modPlaying())
        {
            dang::SndGear::playMod(paperbird_mod, paperbird_mod_length, _pnk._prefs.volume_track);
        }

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: track loaded (%d)\n", mallinfo().uordblks);
#endif
        // set up state
        _tmx = &main_1_level;
        dang::TmxExtruder txtr(_tmx, &gear);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: extruded\n");
#endif
        dang::RectF vp = {0, 0, 320, 240};

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: init level\n");
#endif
        gear.initLevel(_tmx, vp);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: set active world size\n");
#endif
//        gear.setActiveWorldSize(vp.w+1000, vp.h+1000);
        gear.setActiveWorld(-500, -500, vp.w+1000, vp.h+1000);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: init image sheets\n");
#endif

        txtr.getImagesheets();

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: image sheets initialised\n");
#endif

        // create background Tilelayer
        dang::spTileLayer tl = txtr.getTileLayer(tmx_bg_layer_name, true);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: tile layer\n");
#endif

        dang::spImgSprLayer dl = txtr.getImgSprLayer(tmx_deco_layer_name, false, true, false);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: sprite layer\n");
#endif

        // create spritelayer w/o collision detection/resolution
        dang::spImgSprLayer sl = txtr.getImgSprLayer(tmx_obj_layer_name, false, true, false);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: auto layers done\n");
#endif
        assert(dl != nullptr);
        for (size_t j = 0; j < dl->tmxLayer()->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = dl->tmxLayer()->spriteobjects + j;

            dang::spImagesheet is = gear.getImagesheet(so->tileset);

            if (so->type == "candle")
            {
                dang::spFullImgSpr spr = std::make_shared<dang::FullImgSpr>(so, is);
                auto flickering = txtr.getAnimation(is, "flicker");
                spr->setAnimation(flickering);

                if(so->name == "leftcandle")
                {
                    _sprLeftCandle = spr;
                }
                else
                {
                    _sprRightCandle = spr;
                }
                sl->addSprite((dang::spImgSpr)spr);
            }
            else
            {
                dang::spImgSpr spr = std::make_shared<dang::ImgSpr>(so, is);
                sl->addSprite(spr);
            }
        }

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: buttons\n");
#endif

        // make sure to resize the buttons array to the correct size
        _btns.resize(_pnk.ENDOF_SELECTION, {nullptr, nullptr, 0});

        // make sure to have that animation of king chasing piggie set up
        initTweens();

        // create sprites
        for (size_t j = 0; j < sl->tmxLayer()->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = sl->tmxLayer()->spriteobjects + j;

            dang::spImagesheet is = gear.getImagesheet(so->tileset);
            dang::spFullImgSpr spr{nullptr};
            // buttons
            if(so->type == "button")
            {
                spr = std::make_shared<dang::FullImgSpr>(so, is);
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
                spr = std::make_shared<dang::FullImgSpr>(so, is);
                spr->setTransform(blit::SpriteTransform::HORIZONTAL);
                spr->setAnimation(_animPig);

                // piggie starting off-screen to the left
                spr->setPosX(_startPosPigR.x);
                spr->addTween(_twpPigR);

                _pig = spr;

            }
            else if (so->name == "hero")
            {
                spr = std::make_shared<dang::FullImgSpr>(so, is);
                spr->setAnimation(_animKing);

                // hero starting off-screen to the left
                spr->setPosX(_startPosKingR.x);
                spr->addTween(_twpKingR);

                _king = spr;
            }

            // and add it to the collection
            if(spr != nullptr){
                sl->addSprite((dang::spImgSpr)spr);
            }
        }

        positionCandles();

        // first screen of tmx
        gear.setViewportPos({0, 8});

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSHome: entered\n");
#endif
    }

    void GSHome::exit(dang::Gear &gear, uint32_t time)
    {
        // empty out gear
        gear.removeImagesheets();
        gear.removeLayers();
//        gear.removeNTrees();

        _btns.clear();
        _sprLeftCandle.reset();
        _sprRightCandle.reset();

        _king.reset();
        _pig.reset();

        _animKing.reset();
        _animPig.reset();

        _twpKingL.reset();
        _twpKingR.reset();
        _twpPigL.reset();
        _twpPigR.reset();

        _tmx = nullptr;

        _pnk.savePrefs();

    }

    void GSHome::playOink()
    {
        uint32_t val = dang::Rand::get(uint32_t(0), uint32_t(100));
        if (val == 1)
        {
            dang::SndGear::playSfx(pig_squeal_22050, pig_squeal_22050_length, _pnk._prefs.volume_sfx);
        }
    }
    
    void GSHome::initTweens()
    {
        // animations are always the same, no matter if running left or right.
        _animPig = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{11, 12, 13, 14, 15, 16}, 600, dang::Ease::Linear, -1);
        _animKing = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{6, 7, 8, 9, 10, 11}, 600, dang::Ease::Linear, -1);

        // paths for left to right, right to left, for both king and piggie
        _twpPigR = std::make_shared<dang::TwPos>(_endPosPigR, 3000, dang::Ease::Linear, 0, false, 1000);
        _twpKingR = std::make_shared<dang::TwPos>(_endPosKingR, 3000, dang::Ease::Linear, 0, false, 1000);

        _twpPigL = std::make_shared<dang::TwPos>(_endPosPigL, 3000, dang::Ease::Linear, 0, false, 1000);
        _twpKingL = std::make_shared<dang::TwPos>(_endPosKingL, 3000, dang::Ease::Linear, 0, false, 1000);

        // replacing old animation, repositioning sprite and re-adding new tween (after resetting it)
        // first for the king
        _twpKingR->setFinishedCallback([&](){
            playOink();
            _king->setPosX(_startPosKingL.x);
            _king->setTransform(blit::SpriteTransform::HORIZONTAL);
            _twpKingL->reset();
            _king->removeTweens(true);
            _king->addTween(_twpKingL);
        });
        _twpKingL->setFinishedCallback([&](){
            playOink();
            _king->setPosX(_startPosKingR.x);
            _king->setTransform(blit::SpriteTransform::NONE);
            _twpKingR->reset();
            _king->removeTweens(true);
            _king->addTween(_twpKingR);
        });

        // and for the pig the same
        _twpPigR->setFinishedCallback([&](){
            _pig->setPosX(_startPosPigL.x);
            _pig->setTransform(blit::SpriteTransform::NONE);
            _twpPigL->reset();
            _pig->removeTweens(true);
            _pig->addTween(_twpPigL);

        });
        _twpPigL->setFinishedCallback([&](){
            _pig->setPosX(_startPosPigR.x);
            _pig->setTransform(blit::SpriteTransform::HORIZONTAL);
            _twpPigR->reset();
            _pig->removeTweens(true);
            _pig->addTween(_twpPigR);
        });
    }

}