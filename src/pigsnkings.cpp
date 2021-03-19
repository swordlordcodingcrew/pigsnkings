// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the Pigs and Kings game

#include <iostream>
#include <cassert>
#include <sstream>

#include "snd/SndGear.hpp"
#include "Imagesheet.hpp"
#include "tween/TwAnim.hpp"
#include "TileLayer.hpp"
#include "CollisionSprite.hpp"
#include "CollisionSpriteLayer.hpp"
#include "Hero.h"
#include "pigsnkings.hpp"
#include "Bubble.h"
#include "GameState.h"
#include "GSIntro.h"
#include "GSHome.h"
#include "GSPlay.h"
#include "sfx/bubble_pop_22050_mono.h"
#include "tracks/gocryogo.h"

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

    // TODO think about having some logic somewhere where health == 0 results in the hero to die
    void PigsnKings::removeHealth(uint8_t health)
    {
        _prefs.health -= health;
        if(_prefs.health <= 0)
        {
            _prefs.health = 0;
        }
    }

    // TODO think about having some logic somewhere where lives == 0 will end the game
    void PigsnKings::removeLives(uint8_t lives)
    {
        _prefs.lives -= lives;

        if(_prefs.lives <= 0)
        {
            _prefs.lives = 0;
        }
    }

    /**
     * static callback functions for DANG
     */
    void PigsnKings::blit_sprite_cb(dang::RectU sr, dang::Vector2I p, uint8_t t)
    {
        blit::screen.blit_sprite({int32_t(sr.x), int32_t(sr.y), int32_t(sr.w), int32_t(sr.h)}, {p.x, p.y}, t);
    }

    /**
     * static callback function for DANG
     */
    void PigsnKings::line_cb(dang::Vector2I sp, dang::Vector2I dp, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        blit::screen.pen = blit::Pen(r, g, b, a);
        blit::screen.line({int32_t(sp.x), int32_t (sp.y)}, {int32_t(dp.x), int32_t (dp.y)});
    }

    /**
     * static callback functions for DANG to set the spritesheet
     */
    void PigsnKings::set_surface_cb(spImagesheet is)
    {
        assert(is->getSurface() != nullptr);
        blit::screen.sprites = static_cast<blit::Surface*>(is->getSurface());

/*        if (_pnk._active_imagesheet != is.get())
        {
            blit::Surface* sp = _pnk.__surfaces[is.get()];
            assert(sp != nullptr);
            blit::screen.sprites = sp;
        }
*/    }


    /**
     * main class
     */
    PigsnKings::PigsnKings()
    {

    }

    void PigsnKings::init()
    {
        blit::set_screen_mode(blit::ScreenMode::hires);

        // set the DANG callback functions for drawing
        _gear.blit_sprite_cb = PigsnKings::blit_sprite_cb;
        _gear.set_surface_cb = PigsnKings::set_surface_cb;
        _gear.line_cb = PigsnKings::line_cb;

        _gs = GameState::_gs_intro;
        _gs->enter(_gear, 0);

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

    void PigsnKings::renderInfoText()
    {
        // the text is only for development purpose
        blit::screen.pen = blit::Pen(255, 255, 255);
        blit::screen.text(_gs->getInfotext(), blit::minimal_font, blit::Point(1, 1));
    }

/*    void PigsnKings::loadSurface(spImagesheet is)
    {
        const uint8_t* data = is->getData();
        assert(data != nullptr);
        blit::Surface* sf = blit::Surface::load(data);
        assert(sf != nullptr);
        __surfaces[is.get()] = sf;
    }
*/
    void PigsnKings::initImageSheets(dang::TmxExtruder& tmex)
    {
        tmex.getImagesheets(_gear);

        // link the surface to the imagesheets
        // assuming that each tileset has its own image
        for (const std::pair<const std::string, spImagesheet>& p : _gear.getImagesheets())
        {
            const uint8_t* data = p.second->getData();
            assert(data != nullptr);
            blit::Surface* sf = blit::Surface::load(data);
            assert(sf != nullptr);
            p.second->setSurface(static_cast<void*> (sf));

        }
    }

    void PigsnKings::initImageSheet(spImagesheet is)
    {
        const uint8_t* data = is->getData();
        assert(data != nullptr);
        blit::Surface* sf = blit::Surface::load(data);
        assert(sf != nullptr);
        is->setSurface(static_cast<void*> (sf));
        _gear.addImagesheet(is);
    }

    void PigsnKings::removeImagesheets()
    {
        for (const std::pair<const std::string, spImagesheet>& p : _gear.getImagesheets())
        {
            blit::Surface* sf = static_cast<blit::Surface*>(p.second->getSurface());
            delete sf;
            p.second->setSurface(nullptr);
        }
        _gear.removeImagesheets();
    }

/*    void PigsnKings::removeSurface(spImagesheet is)
    {
        blit::Surface* sf = __surfaces[is.get()];
        if (sf != nullptr)
        {
            __surfaces.erase(is.get());
            delete sf;
        }
    }
*/
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
        playMod(mod, len, _pnk._prefs.volume_snd);
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
        blit::channels[dang::SndGear::getMusicChan()].volume = volume;
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

    void PigsnKings::initGameVars()
    {
        _prefs.health = 10;
        _prefs.score = 0;
        _prefs.lives = 3;
    }

    void PigsnKings::addScore(uint16_t points)
    {
        _prefs.score += points;
    }

    void PigsnKings::addHealth(uint8_t health)
    {
        _prefs.health += health;
    }

    void PigsnKings::addLives(uint8_t lives)
    {
        _prefs.lives += lives;
    }
}

/**
 * Entry points of the 32blit-engine
 */
void init() { pnk::_pnk.init(); }
void render(uint32_t time) { pnk::_pnk.render(time); }
void update(uint32_t time) { pnk::_pnk.update(time); }

