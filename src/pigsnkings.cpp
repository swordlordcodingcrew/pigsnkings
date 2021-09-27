// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the Pigs and Kings game

#include <fonts/hud_font_small.h>
#include "pigsnkings.hpp"
#include "src/actors/throwies/Bubble.h"
#include "GameState.h"
#include "GSIntro.h"
#include "GSHome.h"
#include "GSPlay.h"
#include "sfx/bubble_pop_22050_mono.h"
#include "tracks/gocryogo.h"
#include "rsrc/gfx/sl_shield_32blit.png.h"
#include "src/actors/hero/Hero.h"

#include <Rand.hpp>
#include <snd/SndGear.hpp>
#include <Imagesheet.hpp>
#include <tween/TwAnim.hpp>
#include <TileLayer.hpp>
#include <CollisionSprite.hpp>
#include <CollisionSpriteLayer.hpp>

#include <malloc.h>
#include <iostream>

namespace pnk
{
    /**
     * Main instance of the game
     */
    PigsnKings _pnk;

    /**
     * static variable
     */
    const dang::Vector2F PigsnKings::_gravity = {0, 8.0};

    /**
     * main class
     */
    PigsnKings::PigsnKings()
    {

    }

    PigsnKings::~PigsnKings()
    {
        removeImagesheets();

        // TODO save gamestates
        // the question is if this makes sense here or
        // if we place savepoints througout the level
    }

    void PigsnKings::init()
    {
        DEBUG_PRINT("pigsnkings: init PNK\n");

        blit::set_screen_mode(blit::ScreenMode::hires);

        DEBUG_PRINT("pigsnkings: screen mode set\n");

        DEBUG_PRINT("pigsnkings: seed the random generator\n");
        dang::Rand::seed();

        // loading preferences, if there are none, write defaults
        if(!blit::read_save(_prefs, PREFERENCES)) {
            blit::write_save(_prefs, PREFERENCES);
        }

        refreshGamestateFromSave();

        _current_gs = GameState::_gs_intro;
        _current_gs->enter(_gear, 0);

        DEBUG_PRINT("pigsnkings: initial module loaded\n");

        _last_time = blit::now();
    }

    void PigsnKings::refreshGamestateFromSave()
    {
        // sanity check for current gamesave slot
        if(_prefs.currentGameSaveSlot < 1 || _prefs.currentGameSaveSlot > 4)
        {
            _prefs.currentGameSaveSlot = 1;
        }

        DEBUG_PRINT("pigsnkings: prefs loaded\n");

        // loading the gamestate from the prefs
        if(!blit::read_save(_gamestate, _prefs.currentGameSaveSlot))
        {
            blit::write_save(_gamestate, _prefs.currentGameSaveSlot);
        }

        DEBUG_PRINT("pigsnkings: game states loaded\n");
    }

    void PigsnKings::update(uint32_t time)
    {
        // first globally handle events
        _dispatcher.publishEvents();

        if (_fade_out)
        {
            if (_fade_colour.a < 255)
            {
                _fade_colour.a = _fade_colour.a + _fade_step > 255 ? 255 : _fade_colour.a + _fade_step;
                _gear.update(10);
            }
            else
            {
                _current_gs->exit(_gear, time);
                _current_gs = _new_gs;
                _current_gs->enter(_gear, time);
                _fade_out = false;
                _fade_in = true;
            }
        }
        else if (_fade_in)
        {
            if (_fade_colour.a > 0)
            {
                _fade_colour.a = _fade_colour.a - _fade_step < 0 ? 0 : _fade_colour.a - _fade_step;
                _gear.update(10);
            }
            else
            {
                _fade_in = false;
            }
        }
        else
        {
            _new_gs = _current_gs->update(_gear, time);

            if (_new_gs != _current_gs)
            {
                _fade_out = true;
            }

            // update is called every 10 ms. if using (time - _last_time) debugging or pausing the game causes unwanted sideeffects
            _gear.update(10);
//        _gear.update(time - _last_time);

        }

        _last_time = time;
    }

    void PigsnKings::render(uint32_t time)
    {
        // no need to clear the screen
        // have the engine render the game
        _gear.render(time);

        if (_fade_out || _fade_in)
        {
            blit::screen.pen = _fade_colour;
            blit::screen.rectangle({0,0, blit::screen.bounds.w, blit::screen.bounds.h});
        }

        // show amount of memory used

#ifdef PNK_DEBUG_MEM
        blit::screen.text("mem: " + std::to_string(mallinfo().uordblks), hud_font_small, { 5, 5 }, true, blit::TextAlign::top_left);
        if (_mem != mallinfo().uordblks / 1024)
        {
            std::cout << "heap=" << std::to_string(_mem) << "k, delta=" << (mallinfo().uordblks/1024 - _mem) << "k" << std::endl;
            _mem = mallinfo().uordblks / 1024;
        }
#endif
    }

    void PigsnKings::removeImagesheets()
    {
        _gear.removeImagesheets();
    }

    dang::Gear& PigsnKings::getGear()
    {
        return _gear;
    }

    bool PigsnKings::isCheating()
    {
        return _prefs.invincible || _gamestate.has_cheated;
    }
}

/**
 * Entry points of the 32blit-engine
 */
void init() { pnk::_pnk.init(); }
void render(uint32_t time) { pnk::_pnk.render(time); }
void update(uint32_t time) { pnk::_pnk.update(time); }

