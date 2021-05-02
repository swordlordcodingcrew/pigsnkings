// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the Pigs and Kings game

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
        blit::debug("init PNK\n");

        blit::set_screen_mode(blit::ScreenMode::hires);

        blit::debug("screen mode set\n");

        // loading preferences, if there are none, write defaults
        if(!blit::read_save(_prefs, PREFERENCES)) {
            blit::write_save(_prefs, PREFERENCES);
        }

        blit::debug("prefs loaded\n");

        // TODO loading gamestates (there are four.. not sure which one to read, probably first until user chooses differently)
        if(!blit::read_save(_gamestate, GAMESTATE_1)) {
            blit::write_save(_gamestate, GAMESTATE_1);
        }

        blit::debug("game states loaded\n");

        _gs = GameState::_gs_intro;
        _gs->enter(_gear, 0);

        blit::debug("initial module loaded\n");

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
        // TODO: do we need to clear the screen?
        blit::screen.pen = blit::Pen(255, 255, 255, 255);
        blit::screen.clear();

        _gear.render(time);

#ifdef PNK_DEBUG
        renderInfoText();
#endif
    }

#ifdef PNK_DEBUG
    void PigsnKings::renderInfoText()
    {
        // the text is only for development purpose
        blit::screen.pen = blit::Pen(255, 255, 255);
        blit::screen.text(_gs->getInfotext(), blit::minimal_font, blit::Point(1, 1));
    }
#endif
    void PigsnKings::removeImagesheets()
    {
        _gear.removeImagesheets();
    }

    uint8_t PigsnKings::playSfx(const uint8_t *sfx, const uint32_t len)
    {
        return playSfx(sfx, len, _pnk._prefs.volume_sfx);
    }

    uint8_t PigsnKings::playSfx(const uint8_t *sfx, const uint32_t len, float volume)
    {
        if (volume > 1) volume = 1;
        if (volume < 0) volume = 0;

        uint8_t nr = dang::SndGear::setSfx(sfx, len);
        blit::channels[nr].waveforms = blit::Waveform::WAVE; // Set type to WAVE
        blit::channels[nr].wave_buffer_callback = &PigsnKings::sfx_buff_cb;  // Set callback address
        blit::channels[nr].volume = volume * 0xffff;
        blit::channels[nr].user_data = (void*)nr;
        blit::channels[nr].trigger_attack();
        return nr;
    }

    void PigsnKings::sfx_buff_cb(blit::AudioChannel &channel)
    {
        uint8_t nr = (uint64_t)channel.user_data;
        if (dang::SndGear::fillWaveBufferWithSfx(channel.wave_buffer, nr) > 0)
        {
            channel.off();        // Stop playback of this channel.
            channel.waveforms = 0;
            channel.wave_buffer_callback = nullptr;
            channel.volume = 0x7fff;
            channel.user_data = nullptr;
        }

    }

    void PigsnKings::playMod(const uint8_t *mod, const uint32_t len)
    {
        playMod(mod, len, _pnk._prefs.volume_track);
    }

    void PigsnKings::playMod(const uint8_t* mod, const uint32_t len, float volume)
    {
        dang::SndGear::setMod(gocryogo_mod, gocryogo_mod_length);

        #ifdef PNK_SND_DEBUG
        if (dang::SndGear::mod_set)
        {
            blit::debug("module loaded\n");
        }
        else
        {
            blit::debug("the data is not recognised as a module.\n");
        }
        #endif

        blit::channels[dang::SndGear::getMusicChan()].waveforms = blit::Waveform::WAVE; // Set type to WAVE
        blit::channels[dang::SndGear::getMusicChan()].wave_buffer_callback = &PigsnKings::mod_buff_cb;  // Set callback address
        blit::channels[dang::SndGear::getMusicChan()].volume = volume * 0xffff;
        blit::channels[dang::SndGear::getMusicChan()].trigger_attack();

    }

#ifdef PNK_SND_DEBUG
    uint32_t PigsnKings::_last_mod_time = 0;
#endif

    void PigsnKings::mod_buff_cb(blit::AudioChannel &channel)
    {
        if (dang::SndGear::fillWaveBufferWithMod(channel.wave_buffer) > 0)
        {
            channel.off();        // Stop playback of this channel.
        }

#ifdef PNK_SND_DEBUG
        if (blit::now() - PigsnKings::_last_mod_time > 1000)
        {
            PigsnKings::_last_mod_time = blit::now();
            std::stringstream stream;
            stream << "wave_buffer=";
            for (int i = 0; i < 64; i++)
            {
                stream << std::hex << channel.wave_buffer[i] << " ";
            }
            stream << std::endl;
            //blit::debug(stream.str());
        }
#endif
    }

    void PigsnKings::stopMod()
    {
        blit::channels[dang::SndGear::getMusicChan()].off();
        blit::channels[dang::SndGear::getMusicChan()].wave_buffer_callback = nullptr;
    }

}

/**
 * Entry points of the 32blit-engine
 */
void init() { pnk::_pnk.init(); }
void render(uint32_t time) { pnk::_pnk.render(time); }
void update(uint32_t time) { pnk::_pnk.update(time); }

