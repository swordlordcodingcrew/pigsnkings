#include <TmxExtruder.h>
#include "32blit.hpp"

#pragma once

#include "Gear.h"
#include "Dispatcher.h"
#include "Event.h"


// forward declarations
namespace dang
{
    struct tmx_level;
    class Event;
    class Imagesheet;

    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
}

namespace pnk
{
    // forward declarations
    class GameState;

    class PigsnKings
    {
    public:
        PigsnKings();

        static const dang::Vector2F _gravity;

        uint32_t _score = 0;
        uint32_t _active_level = 1;
        uint32_t _active_room = 0;

        // global event dispatcher
        dang::Dispatcher _dispatcher;

        void init();
        void update(uint32_t time);
        void render(uint32_t time);

        void loadSpriteSheet(dang::spImagesheet is);
        void removeSpriteSheet(dang::spImagesheet is);

    protected:
        // the gear
        dang::Gear _gear;
        // callback functions for DANG
        static void blit_sprite_cb(dang::RectU sr, dang::Vector2I p, uint8_t t);
        static void set_spritesheet_cb(dang::spImagesheet is);

        // the state
        std::shared_ptr<GameState>   _gs;

        // for calculating diffs
        uint32_t _last_time{0};

        // test stuff
        uint32_t _cycles{0};
        void renderText();

        // spritesheet depot
        std::map<const dang::Imagesheet*, blit::SpriteSheet*> _spritesheets;
        const dang::Imagesheet* _active_imagesheet{nullptr};

    };
}

