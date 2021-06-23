// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the Pigs and Kings game

#include <malloc.h>
#include <fonts/hud_font_small.h>
#include <iostream>
#include "snd/SndGear.hpp"
#include "Imagesheet.hpp"
#include "tween/TwAnim.hpp"
#include "TileLayer.hpp"
#include "CollisionSprite.hpp"
#include "CollisionSpriteLayer.hpp"
#include "src/actors/hero/Hero.h"
#include "pigsnkings.hpp"
#include "src/actors/throwies/Bubble.h"
#include "GameState.h"
#include "GSIntro.h"
#include "GSHome.h"
#include "GSPlay.h"
#include "sfx/bubble_pop_22050_mono.h"
#include "tracks/gocryogo.h"
#include "rsrc/gfx/sl_shield_32blit.png.h"

using spLayer = std::shared_ptr<dang::Layer>;
using spCollisionSpriteLayer = std::shared_ptr<dang::CollisionSpriteLayer>;
using spSpriteLayer = std::shared_ptr<dang::SpriteLayer>;
using spTileLayer = std::shared_ptr<dang::TileLayer>;
using spCollisionSprite = std::shared_ptr<dang::CollisionSprite>;

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

        // loading preferences, if there are none, write defaults
        if(!blit::read_save(_prefs, PREFERENCES)) {
            blit::write_save(_prefs, PREFERENCES);
        }

        DEBUG_PRINT("pigsnkings: prefs loaded\n");

        // TODO loading gamestates (there are four.. not sure which one to read, probably first until user chooses differently)
        if(!blit::read_save(_gamestate, GAMESTATE_1)) {
            blit::write_save(_gamestate, GAMESTATE_1);
        }

        DEBUG_PRINT("pigsnkings: game states loaded\n");

        _gs = GameState::_gs_intro;
        _gs->enter(_gear, 0);

        DEBUG_PRINT("pigsnkings: initial module loaded\n");

        _last_time = blit::now();
    }

    void PigsnKings::update(uint32_t time)
    {
        // first globally handle events
        _dispatcher.publishEvents();

        std::shared_ptr<GameState> gs = _gs->update(_gear, time);

        if (gs != _gs)
        {
            _gs->exit(_gear, time);
            _gs = gs;
            _gs->enter(_gear, time);
        }

        _gear.update(time - _last_time);

        _last_time = time;
    }

    void PigsnKings::render(uint32_t time)
    {
        // no need to clear the screen
        // have the engine render the game
        _gear.render(time);

        // show amount of memory used

#ifdef PNK_DEBUG_MEM
        blit::screen.text("mem: " + std::to_string(mallinfo().uordblks), hud_font_small, { 5, 5 }, true, blit::TextAlign::top_left);
        if (_mem < mallinfo().uordblks)
        {
            _mem = mallinfo().uordblks;
            std::cout << " heap: " << std::to_string(_mem) << std::endl;
        }
#endif
    }

    void PigsnKings::removeImagesheets()
    {
        _gear.removeImagesheets();
    }


}

/**
 * Entry points of the 32blit-engine
 */
void init() { pnk::_pnk.init(); }
void render(uint32_t time) { pnk::_pnk.render(time); }
void update(uint32_t time) { pnk::_pnk.update(time); }

