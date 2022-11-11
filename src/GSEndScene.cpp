// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "pigsnkings.hpp"

#include "GSEndScene.hpp"
#include "GSAbout.h"
#include "GSHome.h"

#include "32blit.hpp"

#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/gfx/castle_decoration_tiles.png.h"
#include "rsrc/gfx/king.png.h"
#include "rsrc/gfx/hammer.png.h"
#include "rsrc/gfx/king_w_hammer.png.h"
#include "rsrc/end_scene.tmx.hpp"
#include "fonts/barcadebrawl.h"
#include "rsrc/game_strings.hpp"

// DANG includes
#include <Gear.hpp>
#include <Imagesheet.hpp>
#include <layer/ImgSprLayer.hpp>
#include <layer/MessageLayer.hpp>
#include "FireworksLayer.h"
#include <TmxExtruder.hpp>
#include <sprite/FullImgSpr.hpp>
#include <tween/TwAnim.hpp>
#include <tween/TwPos.hpp>
#include <tween/TwNull.hpp>
#include <tween/TwPosX.hpp>
#include <tween/TwPosY.hpp>
#include <tween/TwSequence.hpp>

#include <snd/SndGear.hpp>

#include "tracks/dance2.h"
#include "sfx/win_22050_mono.h"

#include <cassert>

namespace pnk
{
    extern PigsnKings _pnk;


    std::shared_ptr<GameState> GSEndScene::update(dang::Gear &gear, uint32_t time)
    {
//        if (blit::buttons.pressed & BTN_OK)
        if (blit::buttons.pressed & (BTN_BACK | BTN_EXIT))
        {
            return _gs_home;
        }
        else if (_slices[_slice_pos].type == FINISH)
        {
            return _gs_about;
        }

        gear.follow(_sprHero->getPos());

        if (_slice_pos != _prev_slice_pos)
        {
            _prev_slice_pos = _slice_pos;
            nextSlice();
        }

        return _gs_end_scene;
    }


    void GSEndScene::enter(dang::Gear &gear, uint32_t time)
    {
        dang::SndGear::stopMod();
        dang::SndGear::playMod(dance2_mod, dance2_mod_length, _pnk._prefs.volume_track);

        // set up state
        _tmx = &end_scene_level;
        dang::TmxExtruder txtr(_tmx, &gear);

        dang::RectF vp = {0, 0, 320, 240};

        gear.initLevel(_tmx, vp);

        gear.setActiveWorld(-500, -500, vp.w+1000, vp.h+1000);

        txtr.getImagesheets();

        // create background Tilelayer
        dang::spTileLayer tl = txtr.getTileLayer(tmx_bg_layer_name, true);

        // create deco layer
        dang::spImgSprLayer dl = txtr.getImgSprLayer(tmx_deco_layer_name, true, true, true);

        // create spritelayer w/o collision detection/resolution
        dang::spImgSprLayer sl = txtr.getImgSprLayer(tmx_obj_layer_name, false, true, false);

        // create sprites
        for (size_t j = 0; j < sl->tmxLayer()->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = sl->tmxLayer()->spriteobjects + j;

            dang::spImagesheet is = gear.getImagesheet(so->tileset);
            dang::spFullImgSpr spr{nullptr};

            if (so->name == "hero")
            {
                _sprHero = std::make_shared<dang::FullImgSpr>(so, is);
                spr = _sprHero;

                _twaWait = txtr.getAnimation(is, "wait");
                _twaWait->delay(1000);
                _twaJump = txtr.getAnimation(is, "jump");
                _twaJump->duration(500);
                _twaWalk = txtr.getAnimation(is, "walk");
            }
            else if (so->name == "hammer")
            {
                _sprHammer = std::make_shared<dang::FullImgSpr>(so, is);
                spr = _sprHammer;
                dang::spTwAnim blink = txtr.getAnimation(is, "blink",dang::Ease::Linear, -1, false, 1000);
                _sprHammer->setAnimation(blink);
            }
            else if (so->name == "hero_w_hammer")
            {
                _sprHero_w_hammer = std::make_shared<dang::FullImgSpr>(so, is);
                spr = _sprHero_w_hammer;
                dang::spTwAnim vici = txtr.getAnimation(is, "vaevictis");
                _sprHero_w_hammer->setAnimation(vici);
                _sprHero_w_hammer->setVisible(false);
            }

            // and add it to the collection
            if(spr != nullptr){
                sl->addSprite((dang::spImgSpr)spr);
            }
        }

        // create text layer
        dang::PointF p{0,0};
        _txtl = std::make_shared<dang::MessageLayer>(barcadebrawl, p, 10, "", false, false);
        _txtl->setButtons(BTN_OK, BTN_CANCEL);
        gear.addLayer(_txtl);

        gear.setViewportPos(_sprHero->getPos());

        nextSlice();

        _txtl->setText(str_lvl5_intro);
        _txtl->setTtl(6000, [&](blit::Button btn)
        {
            _txtl->setActive(false);
            _txtl->setVisible(false);
        });
        _txtl->setActive(true);
        _txtl->setVisible(true);

        std::shared_ptr<FireworksLayer> fwl = std::make_shared<FireworksLayer>();
        assert(fwl != nullptr);
        gear.addLayer(fwl);
    }

    void GSEndScene::exit(dang::Gear &gear, uint32_t time)
    {
        _slice_pos = 0;
        _prev_slice_pos = 0;
        gear.removeImagesheets();
        gear.removeLayers();
    }

    void GSEndScene::nextSlice()
    {
        if (_slice_pos < _slices.size())
        {
            slice& s = _slices[_slice_pos];
            switch (s.type)
            {
                default:
                case WAIT:
                {
                    _sprHero->removeTweens(false);
                    _sprHero->setAnimation(_twaWait);

                    dang::spTwNull tw0 = std::make_shared<dang::TwNull>(s.time);
                    tw0->setFinishedCallback([&](){_slice_pos++;});
                    _sprHero->setTransform(s.to_the_left ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE);
                    _sprHero->addTween(tw0);
                    break;
                }
                case WALK:
                {
                    _sprHero->removeTweens(false);
                    _sprHero->setAnimation(_twaWalk);

                    dang::spTwPos twp = std::make_shared<dang::TwPos>(s.pos, s.time, dang::Ease::Linear);
                    twp->setFinishedCallback([&](){_slice_pos++;});
                    _sprHero->setTransform(s.to_the_left ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE);
                    _sprHero->addTween(twp);
                    break;
                }
                case JUMP:
                {
                    _sprHero->removeTweens(false);
                    _sprHero->setAnimation(_twaJump);

                    dang::spTwSequence tws = std::make_shared<dang::TwSequence>();
                    dang::spTwPosY twpy1 = std::make_shared<dang::TwPosY>(s.pos.y-8, s.time/2, dang::Ease::OutQuad);
                    tws->addTween(twpy1);
                    dang::spTwPosY twpy2 = std::make_shared<dang::TwPosY>(s.pos.y, s.time/2, dang::Ease::InQuad);
                    tws->addTween(twpy2);

                    dang::spTwPosX twpx = std::make_shared<dang::TwPosX>(s.pos.x, s.time, dang::Ease::Linear);
                    twpx->setFinishedCallback([&](){_slice_pos++;});
                    _sprHero->addTween(twpx);
                    _sprHero->addTween(tws);

                    _sprHero->setTransform(s.to_the_left ? blit::SpriteTransform::HORIZONTAL : blit::SpriteTransform::NONE);
                    break;
                }
                case W_HAMMER:
                {
                    _sprHero->removeTweens(false);

                    _sprHero->setVisible(false);
                    _sprHammer->setVisible(false);
                    _sprHero_w_hammer->setVisible(true);

                    dang::spTwNull tw0 = std::make_shared<dang::TwNull>(s.time);
                    tw0->setFinishedCallback([&](){_slice_pos++;});
                    _sprHero_w_hammer->addTween(tw0);

                    // woohooo!
                    dang::SndGear::playSfx(win_22050_mono, win_22050_mono_length, _pnk._prefs.volume_sfx);
                    dang::SndGear::playRumbleTrack(&dang::explosion, 0);
                    break;
                }
            }
        }
    }
}
