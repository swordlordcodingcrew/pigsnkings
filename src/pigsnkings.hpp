#pragma once

#include "pnk_globals.h"

#include <32blit.hpp>

#include <DangFwdDecl.h>
#include <Vector2T.hpp>
#include <Dispatcher.hpp>
#include <Gear.hpp>
//#include <TmxExtruder.hpp>
//#include <Event.hpp>


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

        bool isCheating();

        void removeImagesheets();
        void refreshGamestateFromSave();

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
            GAMESAVESLOT,
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
            uint8_t currentGameSaveSlot{1}; // 1-4
        };

        struct gamestate
        {
            // which module is set in the GSHome (where is the candle)
            uint8_t  lives{3};
            uint8_t  health{100}; // percent
            uint32_t score{0};
            uint32_t active_level{2};
            uint32_t active_room{0};
            bool     has_cheated{false};
        };

        gamestate       _gamestate; // state of current game
        prefs           _prefs; // world preferences
        std::string     cheatKeyStream = "12345678";

        dang::Gear& getGear();

    protected:

        // the gear
        dang::Gear _gear;

        // the state
        std::shared_ptr<GameState>   _gs;

        // for calculating diffs
        uint32_t _last_time{0};

#ifdef PNK_DEBUG_MEM
        int _mem{0};
#endif

#ifdef PNK_DEBUG
        // test stuff
        uint32_t _cycles{0};
#endif

#ifdef PNK_SND_DEBUG
        static uint32_t _last_mod_time;
#endif
    };
}

