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

        static const dang::Vector2F _gravity;

        // global event dispatcher
        dang::Dispatcher _dispatcher;

        void init();
        void update(uint32_t time);
        void render(uint32_t time);

        void initImageSheets(dang::TmxExtruder& tmex);
        void initImageSheet(spImagesheet is);
        void removeImagesheets();

        static uint8_t playSfx(const uint8_t *sfx, const uint32_t len);
        static uint8_t playSfx(const uint8_t* sfx, const uint32_t len, float volume);
        static void playMod(const uint8_t *mod, const uint32_t len);
        static void playMod(const uint8_t* mod, const uint32_t len, float volume);
        static void stopMod();

        void initGameVars();
        void addScore(uint16_t points);
        void addHealth(uint8_t health);
        void addLives(uint8_t lives);
        void removeHealth(uint8_t health);
        void removeLives(uint8_t lives);

        enum e_selection
        {
            ABOUT = 0,
            PLAY,
            PREFS,
            _ESIZE
        };

        struct prefs
        {
            float volume_snd{0.7};
            float volume_sfx{1};

            // which module is set in the GSHome (where is the candle)
            uint8_t selectedModule{PLAY};

            uint8_t  lives{3};
            uint8_t  health{10};
            uint32_t score{0};
            uint32_t active_level{1};
            uint32_t active_room{0};
        };

        prefs   _prefs;

    protected:
        // the gear
        dang::Gear _gear;

        // callback functions for DANG
        static void blit_sprite_cb(dang::RectU sr, dang::Vector2I p, uint8_t t);
        static void set_surface_cb(dang::spImagesheet is);
        static void line_cb(dang::Vector2I sp, dang::Vector2I dp, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        static void sfx_buff_cb(blit::AudioChannel &channel);
        static void mod_buff_cb(blit::AudioChannel &channel);

        // the state
        std::shared_ptr<GameState>   _gs;

        // for calculating diffs
        uint32_t _last_time{0};

        // test stuff
        uint32_t _cycles{0};
        void renderInfoText();

        // surface depot
//        std::map<const dang::Imagesheet*, blit::Surface*> __surfaces;
//        std::map<std::string, blit::Surface*> _surfaces;
        const dang::Imagesheet* _active_imagesheet{nullptr};

#ifdef PNK_SND_DEBUG
        static uint32_t _last_mod_time;
#endif
    };
}

