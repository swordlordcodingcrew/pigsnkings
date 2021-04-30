// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>

#include <fonts/hud_font_big.h>
#include <fonts/hud_font_small.h>
#include <rsrc/gfx/castle_tiles.png.h>
#include <rsrc/gfx/hud_ui.png.h>
#include <sfx/coin_22050_mono.h>

#include "Gear.hpp"
#include "SettingsLayer.h"
#include "Layer.hpp"
#include "Imagesheet.hpp"

#include "tinyraytracer.h" // see https://github.com/ssloy/tinyraytracer
#include "type.h"
#include "mesh_monkey.h"

namespace pnk
{
    SettingsLayer::SettingsLayer() : Layer(dang::Layer::LT_UNDEFINED)
    {
        backgroundColour = blit::Pen(255, 255, 255, 255);
        foregroundColour = blit::Pen(0, 0, 0, 255);
        highlightColour = blit::Pen(221, 107, 6, 255);

        // clear the background
        blit::screen.pen = backgroundColour;
        displayRect = blit::Rect(0, 0, blit::screen.bounds.w, blit::screen.bounds.h);
        blit::screen.rectangle(displayRect);

        _prefs.resize(_pnk.ENDOF_PREFS, {"unset", 1, SLIDER});

        _prefs.at(_pnk.TRACKS).caption = "Music";
        _prefs.at(_pnk.TRACKS).curVal = _pnk._gamevars.volume_snd;

        _prefs.at(_pnk.SFX).caption = "Sound FX";
        _prefs.at(_pnk.SFX).curVal = _pnk._gamevars.volume_sfx;

        _prefs.at(_pnk.VOLUME).caption = "Volume";
        _prefs.at(_pnk.VOLUME).curVal = 1; // blit persist.volume

        _is_castle = std::make_shared<dang::Imagesheet>("gfx_levels_castle_tiles", &gfx_levels_castle_tiles, 12, 8);
        _is_hud = std::make_shared<dang::Imagesheet>("hud_ui", &hud_ui, 15, 7);
    }

    void SettingsLayer::update(uint32_t dt, const dang::Gear &gear)
    {
        // move selection
        if (blit::buttons.pressed & blit::Button::DPAD_DOWN)
        {
            _selectedPref = ++_selectedPref % _pnk.ENDOF_PREFS;

            //positionCandles();
        }
        else if (blit::buttons.pressed & blit::Button::DPAD_UP)
        {
            if(_selectedPref == 0) {
                _selectedPref = _pnk.ENDOF_PREFS - 1;
            }
            else {
                _selectedPref = --_selectedPref % _pnk.ENDOF_PREFS;
            }

            //positionCandles();
        }

        if (blit::buttons.pressed & blit::Button::DPAD_LEFT)
        {
            //positionCandles();
            auto& pref = _prefs.at(_selectedPref);
            if(pref.curVal > 0.1)
            {
                _prefs.at(_selectedPref).curVal -= .1;
            }
            else
            {
                _prefs.at(_selectedPref).curVal = 0;
            }

            _pnk._gamevars.volume_sfx = pref.curVal;
            PigsnKings::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length);
        }
        else if (blit::buttons.pressed & blit::Button::DPAD_RIGHT)
        {
            //positionCandles();
            auto& pref = _prefs.at(_selectedPref);
            if(pref.curVal < 0.9)
            {
                _prefs.at(_selectedPref).curVal += .1;
            }
            else
            {
                _prefs.at(_selectedPref).curVal = 1;
            }

            _pnk._gamevars.volume_sfx = pref.curVal;
            PigsnKings::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length);
        }
    }

    void SettingsLayer::render(const dang::Gear& gear)
    {
        paintBackground(gear);

        uint8_t i = 0;

        for(auto& pref : _prefs)
        {
            if(i == _selectedPref)
            {
                blit::screen.pen = foregroundColour;
            }
            else
            {
                blit::screen.pen = backgroundColour;
            }
            blit::screen.text(pref.caption, hud_font_small, blit::Point(49, 50 + (i * 20)), true, blit::TextAlign::left);

            if(pref.type == SLIDER)
            {
                paintSlider(gear, 140, 50 + (i * 20), pref.curVal);
                //blit::screen.text(pref.caption, hud_font_small, blit::Point(180, 50 + (i * 20)), true, blit::TextAlign::right);
            }

            i++;
        }
    }

    void SettingsLayer::paintSlider(const dang::Gear& gear, uint8_t x, uint8_t y, float val)
    {
//        61-63

        blit::screen.sprites = _is_hud->getSurface();

        blit::Rect sr = _is_hud->getBlitRect(61);
        blit::Point dp = {x, y};
        blit::screen.blit_sprite(sr, dp, 0);

        sr = _is_hud->getBlitRect(62);

        for(int i = 1; i < 6; i++)
        {
            dp = {x + (i * 16), y};
            blit::screen.blit_sprite(sr, dp, 0);
        }

        sr = _is_hud->getBlitRect(63);
        dp = {x + (6 * 16), y};
        blit::screen.blit_sprite(sr, dp, 0);

        // the slider
        sr = _is_hud->getBlitRect(60);
        dp = {x + (val * 100), y};
        blit::screen.blit_sprite(sr, dp, 0);
    }

    // paint the background
    void SettingsLayer::paintBackground(const dang::Gear& gear)
    {
        blit::screen.sprites = _is_castle->getSurface();

        // back wall
        blit::Rect sr = _is_castle->getBlitRect(61);
        for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                blit::Point dp = {i * 32, j * 32};
                blit::screen.blit_sprite(sr, dp, 0);
            }
        }

        // top side
        sr = _is_castle->getBlitRect(25);
        for(int i = 0; i < 10; i++)
        {
            blit::Point dp = {i * 32, -8};
            blit::screen.blit_sprite(sr, dp, 0);
        }

        // bottom side
        sr = _is_castle->getBlitRect(1);
        for(int i = 0; i < 10; i++)
        {
            blit::Point dp = {i * 32, 216};
            blit::screen.blit_sprite(sr, dp, 0);
        }

        // left side
        sr = _is_castle->getBlitRect(14);
        for(int j = 1; j < 8; j++)
        {
            blit::Point dp = {0, (j * 32) - 8};
            blit::screen.blit_sprite(sr, dp, 0);
        }

        // right side
        sr = _is_castle->getBlitRect(12);
        for(int j = 1; j < 8; j++)
        {
            blit::Point dp = {288, (j * 32) - 8};
            blit::screen.blit_sprite(sr, dp, 0);
        }

        // once every corner
        // top left
        sr = _is_castle->getBlitRect(4);
        blit::Point dp = {0, - 8};
        blit::screen.blit_sprite(sr, dp, 0);

        // top right
        sr = _is_castle->getBlitRect(5);
        dp = {288, - 8};
        blit::screen.blit_sprite(sr, dp, 0);

        // bottom left
        sr = _is_castle->getBlitRect(16);
        dp = {0, 216};
        blit::screen.blit_sprite(sr, dp, 0);

        // bottom right
        sr = _is_castle->getBlitRect(17);
        dp = {288, 216};
        blit::screen.blit_sprite(sr, dp, 0);
    }
}
