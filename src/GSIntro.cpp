// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "pnk_globals.h"
#include "GSIntro.h"
#include "GSHome.h"

#include "rsrc/gfx/sl_shield_32blit.png.h"
#include "pigsnkings.hpp"

#include <Gear.hpp>
#include <Imagesheet.hpp>
#include <Sprite.hpp>
#include <SimpleImageLayer.hpp>

#include <cassert>
#include <snd/SndGear.hpp>
#include <sfx/cheat_22050_mono.h>
#include <sfx/defeat_22050_mono.h>

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
        DEBUG_PRINT("GSIntro: entering\n");

        gear.setWorld({0,0,320, 240});
        gear.setActiveWorldSize(320, 240);
        gear.setViewport({0,0,320, 240});

        DEBUG_PRINT("GSIntro: viewport set\n");

//        dang::SizeU size(104, 200);
        dang::spImagesheet is = std::make_shared<dang::Imagesheet>("sl_shield", &img_sl_shield);

        DEBUG_PRINT("GSIntro: imagesheet loaded");
        std::shared_ptr<dang::SimpleImageLayer> sil = std::make_shared<dang::SimpleImageLayer>(is);
        assert(sil != nullptr);
        gear.addLayer(sil);

        DEBUG_PRINT("GSIntro: layer set up\n");
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
