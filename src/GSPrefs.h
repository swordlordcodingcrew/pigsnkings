// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "GameState.h"

#include <TmxExtruder.hpp>

#include <memory>

namespace pnk
{
    class SettingsLayer;

    class GSPrefs : public GameState
    {
    public:
        std::shared_ptr<GameState> update(dang::Gear& gear, uint32_t time) override;
        void enter(dang::Gear& gear, uint32_t time) override;
        void exit(dang::Gear& gear, uint32_t time) override;

    private:
        /**
         * rsrc for the home state
         */
        const dang::tmx_level* _tmx{nullptr};

        /**
         * rsrc-names
         */
        const std::string tmx_bg_layer_name{"prefs_bg"};
        const std::string tmx_deco_layer_name{"prefs_mood"};

        /**
         * layers
         */
        std::shared_ptr<SettingsLayer>  _stl{nullptr};
        dang::spMessageLayer            _txtl{nullptr};
    };
}
