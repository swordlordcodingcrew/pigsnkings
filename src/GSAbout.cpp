// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game


#include "GSAbout.h"
#include "GSHome.h"
#include "ScrolltextLayer.h"
#include "pigsnkings.hpp"
#include "tracks/u4iasmok.h"

#include <32blit.hpp>

#include <Gear.hpp>
#include <snd/SndGear.hpp>

#include <cassert>
#include <sfx/cheat_22050_mono.h>

namespace pnk
{
    extern PigsnKings _pnk;

    std::shared_ptr<GameState> pnk::GSAbout::update(dang::Gear& gear, uint32_t time)
    {
        updateCheatKeyStream(blit::buttons.pressed);
        checkCheatActivation();

        if (blit::buttons.pressed & (BTN_BACK | BTN_EXIT))
        {
            return GameState::_gs_home;
        }

        return GameState::_gs_about;
    }

    void GSAbout::enter(dang::Gear& gear, uint32_t time)
    {
        blit::debugf("entering\r\n");

        dang::SndGear::playMod(u4iasmok_mod, u4iasmok_mod_length, _pnk._prefs.volume_track);

        gear.setWorld({0,0,320, 240});
        gear.setActiveWorldSize(320, 240);
        gear.setViewport({0,0,320, 240});

        std::shared_ptr<ScrolltextLayer> stl = std::make_shared<ScrolltextLayer>();
        assert(stl != nullptr);
        gear.addLayer(stl);

        blit::debugf("entered\r\n");
    }

    void GSAbout::exit(dang::Gear& gear, uint32_t time)
    {
        blit::debugf("leaving\r\n");

        gear.removeLayers();
        dang::SndGear::stopMod();

        blit::debugf("left\r\n");
    }

    void GSAbout::checkCheatActivation()
    {
        // inspector gadget for snes, debug menu
        if(_pnk.cheatKeyStream == "BBAALRLR")
        {
            // handled this cheat, reset stream
            _pnk.cheatKeyStream[7] = '8';

            DEBUG_PRINT("Cheat activated: Deleting savefiles.\r\n");
            blit::debugf("deleted\r\n");

            dang::SndGear::playSfx(cheat_22050_mono, cheat_22050_mono_length, _pnk._prefs.volume_sfx);
            _pnk.resetPrefsGameslot();
            _pnk.resetAllGameslots();
        }
    }
}
