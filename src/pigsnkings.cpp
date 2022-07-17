// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the Pigs and Kings game

#include "pigsnkings.hpp"
#include "actors/throwies/Bubble.h"
#include "GameState.h"
#include "GSIntro.h"
#include "GSHome.h"
#include "GSPlay.h"
#include "rsrc/gfx/sl_shield_32blit.png.h"
#include "fonts/hud_font_small.h"
#include "actors/hero/Hero.h"

#include <Rand.hpp>
#include <Imagesheet.hpp>
#include <tween/TwAnim.hpp>

#include <cstring>
#include <cassert>
#include <malloc.h>

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
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("pigsnkings: init PNK\n");
#endif
        blit::set_screen_mode(blit::ScreenMode::hires);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("pigsnkings: screen mode set\n");
        DEBUG_PRINT("pigsnkings: seed the random generator\n");
#endif
        dang::Rand::seed();

        // loading preferences, if there are none, write defaults
        if(!blit::read_save(_prefs, PREFERENCES))
        {
            blit::write_save(_prefs, PREFERENCES);
        }

        refreshGamestateFromSave();

        _current_gs = GameState::_gs_intro;
        _current_gs->enter(_gear, 0);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("pigsnkings: initial module loaded\n");
#endif
        _last_update_time = blit::now();
    }

    void PigsnKings::refreshGamestateFromSave()
    {
        // sanity check for current gamesave slot
        if(_prefs.currentGameSaveSlot < FIRST_GAME_SAVE_SLOT || _prefs.currentGameSaveSlot > LAST_GAME_SAVE_SLOT)
        {
            _prefs.currentGameSaveSlot = FIRST_GAME_SAVE_SLOT;
        }

        _removed_sprites.clear();

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("pigsnkings: prefs loaded\n");
#endif
        struct {
            uint8_t version{0};
            uint32_t size{0};
        } header;

        if (blit::read_save(header, _prefs.currentGameSaveSlot))
        {
            if (header.version == 2)
            {

                char*   save = new(std::nothrow) char[header.size];
                if (save != nullptr)
                {
                    std::memset(save, 0xFF, header.size);
                    assert(header.size >= sizeof(gamestate));

                    if (blit::read_save(save, header.size, _prefs.currentGameSaveSlot))
                    {
                        std::memcpy(&_gamestate, save, sizeof(gamestate));

                        uint32_t num_ids = (header.size - sizeof(gamestate)) / 2;

                        for (size_t cnt = 0; cnt < num_ids; ++cnt)
                        {
                            assert(sizeof(gamestate) + cnt*2 < header.size);
                            uint16_t id{0};
                            std::memcpy(&id, save + sizeof(gamestate) + cnt*2, sizeof(uint16_t));
                            _removed_sprites.push_back(id);
                        }

                    }

                    delete [] save;
                }
            }
            else
            {
                _gamestate.save_size = sizeof(gamestate);
                blit::write_save(_gamestate, _prefs.currentGameSaveSlot);
            }
        }
        else
        {
            _gamestate.save_size = sizeof(gamestate);
            blit::write_save(_gamestate, _prefs.currentGameSaveSlot);
        }


#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("pigsnkings: game states loaded\n");
#endif
    }

    void PigsnKings::saveCurrentGamestate()
    {
        // sanity check for current gamesave slot
        if(_prefs.currentGameSaveSlot < FIRST_GAME_SAVE_SLOT || _prefs.currentGameSaveSlot > LAST_GAME_SAVE_SLOT)
        {
            _prefs.currentGameSaveSlot = FIRST_GAME_SAVE_SLOT;
        }

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("pigsnkings: saving gamestate\n");
#endif
        uint32_t size = sizeof(gamestate) + _removed_sprites.size() * sizeof(uint16_t);
        _gamestate.save_size = size;
        char*   save = new(std::nothrow) char[size];

        if (save != nullptr)
        {
            std::memcpy(save, &_gamestate, sizeof(gamestate));
            size_t cnt{0};
            for (uint16_t id : _removed_sprites)
            {
                assert(sizeof(gamestate) + cnt < size);
                std::memcpy(save + sizeof(gamestate) + cnt, &id, sizeof(uint16_t));
                cnt += sizeof(uint16_t);
            }

            // saving the gamestate
            blit::write_save(save, size, _prefs.currentGameSaveSlot);

            delete [] save;
        }
        else
        {
            _gamestate.save_size = sizeof(gamestate);
            blit::write_save(_gamestate, _prefs.currentGameSaveSlot);
        }

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("pigsnkings: game state saved\n");
#endif
    }

    bool PigsnKings::loadGameState(uint8_t slot, gamestate& gs)
    {
        bool ret{true};

        // sanity check for current gamesave slot
        if(slot < FIRST_GAME_SAVE_SLOT || slot > LAST_GAME_SAVE_SLOT)
        {
            slot = FIRST_GAME_SAVE_SLOT;
        }

        struct {
            uint8_t version{0};
            uint32_t size{0};
        } header;

        if (blit::read_save(header, slot))
        {
            if (header.version == 2)
            {
                blit::read_save(gs, slot);
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = false;
        }

        return ret;
    }

    void PigsnKings::initEmptyGameslots()
    {
        for (uint8_t i = FIRST_GAME_SAVE_SLOT; i <= LAST_GAME_SAVE_SLOT; i++)
        {
            gamestate ngs = {};
            ngs.save_size = sizeof(gamestate);
            if(!blit::read_save(ngs, i))
            {
#ifdef PNK_DEBUG_COMMON
                DEBUG_PRINT("pigsnkings: new gamesave written at %d\n", i);
#endif
                blit::write_save(ngs, i);
            }
        }
    }

    void PigsnKings::resetAllGameslots()
    {
        for (uint8_t i = FIRST_GAME_SAVE_SLOT; i <= LAST_GAME_SAVE_SLOT; i++)
        {
            gamestate ngs = {};
            ngs.save_size = sizeof(gamestate);
            blit::write_save(ngs, i);
        }

        // reload current gamestate
        refreshGamestateFromSave();
    }

    void PigsnKings::resetGameslot(uint8_t slot)
    {
        if (slot < FIRST_GAME_SAVE_SLOT || slot > LAST_GAME_SAVE_SLOT)
        {
            return;
        }

        gamestate ngs = {};
        ngs.save_size = sizeof(gamestate);
        blit::write_save(ngs, slot);
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
#ifdef PNK_DEBUG_MEM
        // show amount of memory used
        _prev_mem = _mem;
        _mem = mallinfo().uordblks / 1024;
        if (_mem > 11000)
        {
            DEBUG_PRINT("ALARM - max mem of 11000k reached: %uk\n", _mem);
        }
        else if (_mem != _prev_mem)
        {
            DEBUG_PRINT("heap=%uk, delta=%ik\n", _mem, int32_t(_mem - _prev_mem));
        }
#endif
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

        _dt_update_time = time - _last_update_time;
        // update is called every 10 ms. if using (time - _last_time) debugging or pausing the game causes unwanted sideeffects
        _gear.update(10);
//        _gear.update(time - _last_update_time);


#ifdef PNK_DEBUG_FPS
        _last_render_ticks = _current_render_ticks;
        _current_render_ticks = blit::now();
        blit::screen.alpha = 255;
        blit::screen.pen = blit::Pen(255, 255, 255, 200);
        blit::screen.rectangle(blit::Rect(1, 240 - 10, 100, 9));
        blit::screen.pen = blit::Pen(255, 0, 0);
        std::string fms = "ups=" + std::to_string(int32_t(1000.0f / (_dt_update_time))) + " fps=" + std::to_string(int32_t(1000.0f / (_current_render_ticks - _last_render_ticks)));
//        std::string fms = std::to_string(int32_t(1000.0f / (_current_ticks - _last_ticks)));
        blit::screen.text(fms, blit::minimal_font, blit::Rect(3, 240 - 9, 100, 16));

#endif

        _last_update_time = time;
    }

    void PigsnKings::render(uint32_t time)
    {
        // no need to clear the screen
        // have the engine render the game
        _gear.render(time);

#ifdef PNK_DEBUG_MEM
        blit::screen.pen = {0,0,0};
        // show amount of memory used
        blit::screen.text("mem: " + std::to_string(_mem), hud_font_small, { 5, 5 }, true, blit::TextAlign::top_left);
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

