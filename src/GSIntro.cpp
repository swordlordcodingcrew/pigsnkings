// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "GSIntro.h"

#include "pnk_globals.h"
#include "GSHome.h"
#include "pigsnkings.hpp"
#include "rsrc/gfx/sl_shield_32blit.png.h"

#include <Gear.hpp>
#include <Imagesheet.hpp>
#include <layer/SimpleImageLayer.hpp>
#include <snd/SndGear.hpp>
#include <sfx/cheat_22050_mono.h>

#include <cassert>

namespace pnk
{
    extern PigsnKings _pnk;

    std::shared_ptr<GameState> pnk::GSIntro::update(dang::Gear& gear, uint32_t time)
    {
        _duration += 10;    // update is called every 10 ms

        // validate if magic event triggered
        // user decides to delete (reset) all game slots
        if (blit::buttons.pressed & blit::Button::X && blit::buttons.pressed & blit::Button::Y)
        {
            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
            _pnk.resetPrefsGameslot();
            _pnk.resetAllGameslots();

            return GameState::_gs_home;
        }
        else if (_duration > 1000)
        {
            return GameState::_gs_home;
        }

        return GameState::_gs_intro;
    }

    void GSIntro::enter(dang::Gear& gear, uint32_t time)
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSIntro: entering\n");
#endif
        gear.setWorld({0,0,320, 240});
        gear.setActiveWorldSize(320, 240);
        gear.setViewport({0,0,320, 240});

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSIntro: viewport set\n");
#endif

        dang::spImagesheet is = std::make_shared<dang::Imagesheet>("sl_shield", &img_sl_shield);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSIntro: imagesheet loaded");
#endif
        std::shared_ptr<dang::SimpleImageLayer> sil = std::make_shared<dang::SimpleImageLayer>(is);
        assert(sil != nullptr);
        gear.addLayer(sil);

        // will later be used to upgrade gamesave and pref format
        _pnk.initEmptyGameslots();

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("GSIntro: layer set up\n");
#endif
    }

    void GSIntro::exit(dang::Gear& gear, uint32_t time)
    {
        // remove spritesheets
        gear.removeImagesheets();

        // clear gear
        gear.removeLayers();

        _duration = 0;
    }
}
