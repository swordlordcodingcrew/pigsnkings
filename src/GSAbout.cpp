// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <Gear.hpp>
#include <32blit.hpp>

#include "GSAbout.h"
#include "GSHome.h"
#include "ScrolltextLayer.h"

#include "tracks/u4iasmok.h"

#include <cassert>

namespace pnk
{
    std::shared_ptr<GameState> pnk::GSAbout::update(dang::Gear& gear, uint32_t time)
    {
        if (blit::buttons.pressed & blit::Button::X)
        {
            return GameState::_gs_home;
        }

        return GameState::_gs_about;
    }

    void GSAbout::enter(dang::Gear& gear, uint32_t time)
    {
        blit::debugf("entering\r\n");

        PigsnKings::playMod(u4iasmok_mod, u4iasmok_mod_length);

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
        PigsnKings::stopMod();

        blit::debugf("left\r\n");
    }
}
