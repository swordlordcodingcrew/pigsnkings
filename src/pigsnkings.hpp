#pragma once

#include "pnk_globals.h"
#include "TmxExtruder.hpp"
#include "32blit.hpp"
#include "Gear.hpp"
#include "Dispatcher.hpp"
#include "Event.hpp"

// forward declarations
namespace dang
{
    struct tmx_level;
    class Event;
    class Imagesheet;
}

using spImagesheet = std::shared_ptr<dang::Imagesheet>;

namespace pnk
{
    // forward declarations
    class GameState;

    class PigsnKings
    {
    public:
        PigsnKings();
        ~PigsnKings();

        static const dang::Vector2F _gravity;

        // global event dispatcher
        dang::Dispatcher _dispatcher;

        void init();
        void update(uint32_t time);
        void render(uint32_t time);

        void removeImagesheets();



        enum e_selection
        {
            ABOUT = 0,
            PLAY,
            PREFS,
            ENDOF_SELECTION
        };

        enum e_prefs
        {
            TRACKS = 0,
            SFX,
            VOLUME,
            DIFFICULTY,
            ENDOF_PREFS
        };

        enum e_saveslots
        {
            PREFERENCES = 0,
            GAMESTATE_1,
            GAMESTATE_2,
            GAMESTATE_3,
            GAMESTATE_4,
            ENDOF_SAVESLOTS
        };

        struct prefs
        {
            float volume_track{.5f};
            float volume_sfx{.5f};
            bool invincible{false}; // this is global, since can be set without having loaded a gamestate from a slot
            uint8_t selectedModule{PLAY};
        };

        struct gamestate
        {
            // which module is set in the GSHome (where is the candle)
            uint8_t  lives{3};
            uint8_t  health{100}; // percent
            uint32_t score{0};
            uint32_t active_level{1};
            uint32_t active_room{0};
        };

        gamestate       _gamestate; // state of current game
        prefs           _prefs; // world preferences

    protected:

        // the gear
        dang::Gear _gear;

        // the state
        std::shared_ptr<GameState>   _gs;

        // for calculating diffs
        uint32_t _last_time{0};

#ifdef PNK_DEBUG
        // test stuff
        uint32_t _cycles{0};
#endif

#ifdef PNK_SND_DEBUG
        static uint32_t _last_mod_time;
#endif
    };
}

