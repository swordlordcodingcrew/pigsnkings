// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <Gear.hpp>
#include <32blit.hpp>

#include "GSPrefs.h"
#include "GSHome.h"
#include "SettingsLayer.h"

#include <cassert>

namespace pnk
{
    std::shared_ptr<GameState> pnk::GSPrefs::update(dang::Gear& gear, uint32_t time)
    {
        if (blit::buttons.pressed & blit::Button::X)
        {
            return GameState::_gs_home;
        }

        return GameState::_gs_prefs;
    }

    void GSPrefs::enter(dang::Gear& gear, uint32_t time)
    {
        gear.setWorld({0,0,320, 240});
        gear.setActiveWorldSize(320, 240);
        gear.setViewport({0,0,320, 240});

        std::shared_ptr<SettingsLayer> stl = std::make_shared<SettingsLayer>();
        assert(stl != nullptr);
        gear.addLayer(stl);
    }

    void GSPrefs::exit(dang::Gear& gear, uint32_t time)
    {
        gear.removeLayers();
    }
}
