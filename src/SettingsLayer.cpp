// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>

#include <fonts/hud_font_big.h>
#include <fonts/hud_font_small.h>
#include <rsrc/gfx/castle_tiles.png.h>
#include <rsrc/gfx/hud_ui.png.h>
#include <sfx/coin_22050_mono.h>
#include <libs/DANG/src/snd/SndGear.hpp>
#include <rsrc/gfx/king.png.h>

#include "Gear.hpp"
#include "SettingsLayer.h"
#include "Layer.hpp"
#include "Imagesheet.hpp"


namespace pnk
{
    extern PigsnKings _pnk;

    SettingsLayer::SettingsLayer() : Layer(dang::Layer::LT_UNDEFINED)
    {
        backgroundColour = blit::Pen(255, 255, 255, 255);
        foregroundColour = blit::Pen(0, 0, 0, 255);
        highlightColour = blit::Pen(221, 107, 6, 255);

        // clear the background
        blit::screen.pen = backgroundColour;
        displayRect = blit::Rect(0, 0, blit::screen.bounds.w, blit::screen.bounds.h);
        blit::screen.rectangle(displayRect);

        _prefs.resize(PigsnKings::ENDOF_PREFS, {"unset", 1, STEP_10});

        _prefs.at(PigsnKings::TRACKS).caption = "Music";
        _prefs.at(PigsnKings::TRACKS).curVal = _pnk._prefs.volume_track;

        _prefs.at(PigsnKings::SFX).caption = "Sound FX";
        _prefs.at(PigsnKings::SFX).curVal = _pnk._prefs.volume_sfx;

        _prefs.at(PigsnKings::GAMESAVESLOT).caption = "Gameslot";
        _prefs.at(PigsnKings::GAMESAVESLOT).curVal = _pnk._prefs.currentGameSaveSlot;
        _prefs.at(PigsnKings::GAMESAVESLOT).type = GAMESLOT; // 1-4

        _is_castle = std::make_shared<dang::Imagesheet>("gfx_levels_castle_tiles", &gfx_levels_castle_tiles, 12, 8);
        _is_hud = std::make_shared<dang::Imagesheet>("hud_ui", &hud_ui, 15, 7);
        _is_king = std::make_shared<dang::Imagesheet>("gfx_king", &gfx_king, 28, 1);

        refreshTempGamestateFromSave(_pnk._prefs.currentGameSaveSlot);
    }

    SettingsLayer::~SettingsLayer()
    {
        _is_castle.reset();
        _is_hud.reset();
        _is_king.reset();

        // this is a hack. we set play as the default option in the stored settings
        // otherwise the default would always be prefs
        // which is annoying as hell. I know, I tried this :)
        _pnk._prefs.selectedModule = _pnk.PLAY;
        blit::write_save(_pnk._prefs, _pnk.PREFERENCES);
        _pnk._prefs.selectedModule = _pnk.PREFS;

        // reload the currently selected gamestate
        _pnk.refreshGamestateFromSave();
    }

    void SettingsLayer::update(uint32_t dt, const dang::Gear &gear)
    {
        // ----------------------
        // up and down
        if (blit::buttons.pressed & blit::Button::DPAD_DOWN)
        {
            _selectedPref = ++_selectedPref % _pnk.ENDOF_PREFS;
        }
        else if (blit::buttons.pressed & blit::Button::DPAD_UP)
        {
            if(_selectedPref == 0) {
                _selectedPref = _pnk.ENDOF_PREFS - 1;
            }
            else {
                _selectedPref = --_selectedPref % _pnk.ENDOF_PREFS;
            }
        }

        // ----------------------
        // left and right
        if (blit::buttons.pressed & blit::Button::DPAD_LEFT)
        {
            auto& pref = _prefs.at(_selectedPref);

            // check for different pref types, handle them accordingly
            if(pref.type == STEP_10)
            {
                pref.curVal > 0.1f ? (_prefs.at(_selectedPref).curVal -= .1f) : (_prefs.at(_selectedPref).curVal = 0);

                // check sfx vs tracks
                if(pref.caption == "Music")
                {
                    _pnk._prefs.volume_track = pref.curVal;
                    // change volume
                    dang::SndGear::changeModVolume(_pnk._prefs.volume_track);
                    // make some noise to show user how this sounds
                    dang::SndGear::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length, _pnk._prefs.volume_track);
                }
                else
                {
                    _pnk._prefs.volume_sfx = pref.curVal;
                    // make some noise to show user how this sounds
                    dang::SndGear::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length, _pnk._prefs.volume_sfx);
                }
            }
            else if(pref.type == GAMESLOT)
            {
                pref.curVal > 1 ? (_prefs.at(_selectedPref).curVal -= 1) : (_prefs.at(_selectedPref).curVal = 4);
                _pnk._prefs.currentGameSaveSlot = pref.curVal;
                refreshTempGamestateFromSave(pref.curVal);
            }
        }
        else if (blit::buttons.pressed & blit::Button::DPAD_RIGHT)
        {
            auto& pref = _prefs.at(_selectedPref);

            // check prefs type to handle accordingly
            if(pref.type == STEP_10)
            {
                pref.curVal < 0.9f ? (_prefs.at(_selectedPref).curVal += .1f) : (_prefs.at(_selectedPref).curVal = 1);

                // check sfx vs tracks
                if(pref.caption == "Music")
                {
                    _pnk._prefs.volume_track = pref.curVal;
                    // change volume
                    dang::SndGear::changeModVolume(_pnk._prefs.volume_track);
                    // make some noise to show user how this sounds
                    dang::SndGear::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length, _pnk._prefs.volume_track);
                }
                else
                {
                    _pnk._prefs.volume_sfx = pref.curVal;
                    // make some noise to show user how this sounds
                    dang::SndGear::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length, _pnk._prefs.volume_sfx);
                }
            }
            else if(pref.type == GAMESLOT)
            {
                pref.curVal < 4 ? (_prefs.at(_selectedPref).curVal += 1) : (_prefs.at(_selectedPref).curVal = 1);
                _pnk._prefs.currentGameSaveSlot = pref.curVal;
                refreshTempGamestateFromSave(pref.curVal);
            }
        }
    }

    void SettingsLayer::render(const dang::Gear& gear)
    {
        paintBackground(gear);

        uint8_t i = 0;

        const uint8_t y_dist = 32;

        for(auto& pref : _prefs)
        {
            i == _selectedPref ? blit::screen.pen = foregroundColour : blit::screen.pen = backgroundColour;

            blit::screen.text(pref.caption, hud_font_small, blit::Point(49, 50 + (i * y_dist)), true, blit::TextAlign::left);

            if(pref.type == STEP_10)
            {
                paintSlider(gear, 140, 50 + (i * y_dist), pref.curVal);
            }
            else if(pref.type == GAMESLOT)
            {
                paintGameslot(gear, 140, 50 + (i * y_dist), pref.curVal);
            }

            i++;
        }
    }

    void SettingsLayer::paintSlider(const dang::Gear& gear, uint8_t x, uint8_t y, float val)
    {
        blit::Rect sr = _is_hud->getBlitRect(61);
        blit::Point dp = {x, y};
        blit::screen.blit(_is_hud->getSurface(), sr, dp, 0);

        sr = _is_hud->getBlitRect(62);

        for(int i = 1; i < 6; i++)
        {
            dp = {x + (i * 16), y};
            blit::screen.blit(_is_hud->getSurface(), sr, dp, 0);
        }

        sr = _is_hud->getBlitRect(63);
        dp = {x + (6 * 16), y};
        blit::screen.blit(_is_hud->getSurface(), sr, dp, 0);

        // the slider
        sr = _is_hud->getBlitRect(60);
        dp = {static_cast<int32_t>(x + (val * 100)), y};
        blit::screen.blit(_is_hud->getSurface(), sr, dp, 0);
    }

    void SettingsLayer::paintGameslot(const dang::Gear& gear, uint8_t x, uint8_t y, uint8_t val)
    {
        y = y - 10;

        blit::Rect sr = _is_king->getBlitRect(27);
        blit::Rect sr_selected = _is_king->getBlitRect(26);

        blit::screen.pen = backgroundColour;

        for(uint8_t i = FIRST_GAME_SAVE_SLOT; i <= LAST_GAME_SAVE_SLOT; i++)
        {
            blit::Point dp = {x + ((i-1) * 30), y};

            if(val == i)
            {
                blit::screen.blit(_is_king->getSurface(), sr_selected, dp, 0);
            }
            else
            {
                blit::screen.blit(_is_king->getSurface(), sr, dp, 0);
            }

            // add 10 we lost before + a few until we are above the head of the king
            blit::screen.text(std::to_string(i), hud_font_small, blit::Point((x + (30 * (i-1))) + 15, y + 10 + 9), true, blit::TextAlign::left);
        }

        y += 40;

        blit::screen.text("Highscore: " + std::to_string(_temp_gamestate.high_score), hud_font_small, blit::Point(x, y), true, blit::TextAlign::left);

        y += 18;

        std::string s1{"<3: " + std::to_string(_temp_gamestate.lives) + ", Cheater: " + (_temp_gamestate.has_cheated || _temp_gamestate.invincible ? "Y":"N")};
        blit::screen.text(s1, hud_font_small, blit::Point(x, y), true, blit::TextAlign::left);

        y += 18;

        std::string s2{"Score: " + std::to_string(_temp_gamestate.score)};
        blit::screen.text(s2, hud_font_small, blit::Point(x, y), true, blit::TextAlign::left);

        y += 18;

        std::string s3{"Level: " + std::to_string(_temp_gamestate.active_level) + ", Room: " + std::to_string(_temp_gamestate.active_room)};
        blit::screen.text(s3, hud_font_small, blit::Point(x, y), true, blit::TextAlign::left);

    }
        // paint the background
    void SettingsLayer::paintBackground(const dang::Gear& gear)
    {
        // back wall
        blit::Rect sr = _is_castle->getBlitRect(61);
        for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                blit::Point dp = {i * 32, j * 32};
                blit::screen.blit(_is_castle->getSurface(), sr, dp, 0);
            }
        }

        // top side
        sr = _is_castle->getBlitRect(25);
        for(int i = 0; i < 10; i++)
        {
            blit::Point dp = {i * 32, -8};
            blit::screen.blit(_is_castle->getSurface(), sr, dp, 0);
        }

        // bottom side
        sr = _is_castle->getBlitRect(1);
        for(int i = 0; i < 10; i++)
        {
            blit::Point dp = {i * 32, 216};
            blit::screen.blit(_is_castle->getSurface(), sr, dp, 0);
        }

        // left side
        sr = _is_castle->getBlitRect(14);
        for(int j = 1; j < 8; j++)
        {
            blit::Point dp = {0, (j * 32) - 8};
            blit::screen.blit(_is_castle->getSurface(), sr, dp, 0);
        }

        // right side
        sr = _is_castle->getBlitRect(12);
        for(int j = 1; j < 8; j++)
        {
            blit::Point dp = {288, (j * 32) - 8};
            blit::screen.blit(_is_castle->getSurface(), sr, dp, 0);
        }

        // once every corner
        // top left
        sr = _is_castle->getBlitRect(4);
        blit::Point dp = {0, - 8};
        blit::screen.blit(_is_castle->getSurface(), sr, dp, 0);

        // top right
        sr = _is_castle->getBlitRect(5);
        dp = {288, - 8};
        blit::screen.blit(_is_castle->getSurface(), sr, dp, 0);

        // bottom left
        sr = _is_castle->getBlitRect(16);
        dp = {0, 216};
        blit::screen.blit(_is_castle->getSurface(), sr, dp, 0);

        // bottom right
        sr = _is_castle->getBlitRect(17);
        dp = {288, 216};
        blit::screen.blit(_is_castle->getSurface(), sr, dp, 0);
    }

    void SettingsLayer::refreshTempGamestateFromSave(const uint8_t slot)
    {
        // loading the gamestate from the prefs
        blit::read_save(_temp_gamestate, slot);
    }
}
