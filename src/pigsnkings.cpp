#include <iostream>
#include <cassert>

#include "dang.hpp"
#include "Imagesheet.h"
#include "tween/TwAnim.h"
#include "TileLayer.h"
#include "SpriteLayer.h"
#include "Sprite.h"
#include "CollisionSprite.h"
#include "CollisionSpriteLayer.h"
#include "Hero.h"
#include "pigsnkings.hpp"
#include "pnk_globals.h"
#include "PnkEvent.h"
#include "Bubble.h"
#include "GameState.h"
#include "GSIntro.h"
#include "GSHome.h"
#include "GSPlay.h"

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
     * static const variable for gravity
     */
    const dang::Vector2F PigsnKings::_gravity = {0, 8.0};

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
    void PigsnKings::set_spritesheet_cb(spImagesheet is)
    {
        if (_pnk._active_imagesheet != is.get())
        {
            blit::SpriteSheet* sp = _pnk._spritesheets[is.get()];
            assert(sp != nullptr);
            blit::screen.sprites = sp;
        }
    }


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
        _gear.set_spritesheet_cb = PigsnKings::set_spritesheet_cb;
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

        renderText();
    }

    void PigsnKings::renderText()
    {
        // the text is only for development purpose
        blit::screen.pen = blit::Pen(255, 255, 255);
        blit::screen.text(_gs->getInfotext(), blit::minimal_font, blit::Point(1, 1));
    }

    void PigsnKings::loadSpriteSheet(spImagesheet is)
    {
        const uint8_t* data = is->getData();
        assert(data != nullptr);
        blit::SpriteSheet* ssh = blit::SpriteSheet::load(data);
        assert(ssh != nullptr);
        _spritesheets[is.get()] = ssh;
    }

    void PigsnKings::removeSpriteSheet(spImagesheet is)
    {
        blit::SpriteSheet* ssh = _spritesheets[is.get()];
        if (ssh != nullptr)
        {
            _spritesheets.erase(is.get());
            delete ssh;
        }
    }

}

/**
 * Entry points of the 32blit-engine
 */
void init() { pnk::_pnk.init(); }
void render(uint32_t time) { pnk::_pnk.render(time); }
void update(uint32_t time) { pnk::_pnk.update(time); }

