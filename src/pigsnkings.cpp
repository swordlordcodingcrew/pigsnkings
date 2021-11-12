// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the Pigs and Kings game

#include <fonts/hud_font_small.h>
#include "pigsnkings.hpp"
#include "src/actors/throwies/Bubble.h"
#include "GameState.h"
#include "GSIntro.h"
#include "GSHome.h"
#include "GSPlay.h"
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
        // we decided to do savepoints using triggers
    }

    void PigsnKings::init()
    {
        DEBUG_PRINT("pigsnkings: init PNK\n");

        blit::set_screen_mode(blit::ScreenMode::hires);

        DEBUG_PRINT("pigsnkings: screen mode set\n");

        DEBUG_PRINT("pigsnkings: seed the random generator\n");
        dang::Rand::seed();

        // loading preferences, if there are none, write defaults
        if(!blit::read_save(_prefs, PREFERENCES))
        {
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
        if(_prefs.currentGameSaveSlot < FIRST_GAME_SAVE_SLOT || _prefs.currentGameSaveSlot > LAST_GAME_SAVE_SLOT)
        {
            _prefs.currentGameSaveSlot = FIRST_GAME_SAVE_SLOT;
        }

        DEBUG_PRINT("pigsnkings: prefs loaded\n");

        // loading the gamestate from the prefs
        if(!blit::read_save(_gamestate, _prefs.currentGameSaveSlot))
        {
            blit::write_save(_gamestate, _prefs.currentGameSaveSlot);
        }

        DEBUG_PRINT("pigsnkings: game states loaded\n");
    }

    void PigsnKings::saveCurrentGamestate()
    {
        // sanity check for current gamesave slot
        if(_prefs.currentGameSaveSlot < FIRST_GAME_SAVE_SLOT || _prefs.currentGameSaveSlot > LAST_GAME_SAVE_SLOT)
        {
            _prefs.currentGameSaveSlot = FIRST_GAME_SAVE_SLOT;
        }

        DEBUG_PRINT("pigsnkings: saving gamestate\n");

        // saving the gamestate
        blit::write_save(_gamestate, _prefs.currentGameSaveSlot);

        DEBUG_PRINT("pigsnkings: game state saved\n");
    }

    void PigsnKings::initEmptyGameslots()
    {
        for (uint8_t i = FIRST_GAME_SAVE_SLOT; i <= LAST_GAME_SAVE_SLOT; i++)
        {
            gamestate ngs = {};
            if(!blit::read_save(ngs, i))
            {
                DEBUG_PRINT("pigsnkings: new gamesave written at %d\n", i);
                blit::write_save(ngs, i);
            }
        }
    }

    void PigsnKings::resetAllGameslots()
    {
        for (uint8_t i = FIRST_GAME_SAVE_SLOT; i <= LAST_GAME_SAVE_SLOT; i++)
        {
            gamestate ngs = {};
            blit::write_save(ngs, i);
        }

        // reload current gamestate
        refreshGamestateFromSave();
    }

    void PigsnKings::refreshPrefsFromSave()
    {
        blit::read_save(_prefs, PREFERENCES);
    }

    void PigsnKings::resetPrefsGameslot()
    {
        prefs prfs = {};
        blit::write_save(prfs, 0);

        // reload prefs
        refreshPrefsFromSave();
    }

    void PigsnKings::update(uint32_t time)
    {
        // first globally handle events
        _dispatcher.publishEvents();

        if (!_gear.fading())
        {
            _new_gs = _current_gs->update(_gear, time);
        }

        if (_new_gs != _current_gs)
        {
            _gear.fade(FADE_COL, FADE_STEP, [=](){
                _current_gs->exit(_gear, time);
                _current_gs = _new_gs;
                _current_gs->enter(_gear, time);

            });
        }

        // update is called every 10 ms. if using (time - _last_time) debugging or pausing the game causes unwanted sideeffects
        _gear.update(10);
//        _gear.update(time - _last_time);

        _last_time = time;
    }

    void PigsnKings::render(uint32_t time)
    {
        // no need to clear the screen
        // have the engine render the game
        _gear.render(time);


#ifdef PNK_DEBUG_MEM
        // show amount of memory used
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
        return _gamestate.invincible || _gamestate.has_cheated;
    }
}

/**
 * Entry points of the 32blit-engine
 */
void init() { pnk::_pnk.init(); }
void render(uint32_t time) { pnk::_pnk.render(time); }
void update(uint32_t time) { pnk::_pnk.update(time); }

