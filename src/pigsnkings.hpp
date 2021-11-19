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

        // game saves
        void refreshGamestateFromSave();
        void saveCurrentGamestate();
        void initEmptyGameslots(); // save1-4
        void resetAllGameslots(); // save1-4
        void refreshPrefsFromSave();
        void resetPrefsGameslot(); // save0

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
            uint8_t version{1}; // of the prefs for upgrading
            float volume_track{.5f};
            float volume_sfx{.5f};
            uint8_t selectedModule{PLAY}; // which module is set in the GSHome (where is the candle)
            uint8_t currentGameSaveSlot{1}; // 1-4
        };

        struct gamestate
        {
            uint8_t version{1}; // of the gamestate for upgrading
            int8_t  lives{HERO_MAX_LIVES};
            int8_t  health{HERO_MAX_HEALTH}; // percent
            uint32_t score{0};
            uint32_t high_score{0};
            uint16_t active_level{1};
            uint16_t active_room{0};
            int8_t  boss_health{BOSS_MAX_HEALTH}; // percent
            bool    has_cheated{false}; // persistent
            bool    invincible{false}; // this is global, since can be set without having loaded a gamestate from a slot
        };

        gamestate       _gamestate; // state of current game
        prefs           _prefs; // world preferences
        std::string     cheatKeyStream = "12345678";

        dang::Gear& getGear();

    protected:

        // the gear
        dang::Gear _gear;

        // the state
        std::shared_ptr<GameState>   _current_gs{nullptr};
        std::shared_ptr<GameState>   _new_gs{nullptr};

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

#ifdef PNK_DEBUG_FPS
        uint32_t _last_ticks{0};
        uint32_t _current_ticks{0};
#endif
    };

}

