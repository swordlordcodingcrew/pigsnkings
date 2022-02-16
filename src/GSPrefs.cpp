// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game


#include "GSPrefs.h"
#include "GSHome.h"
#include "SettingsLayer.h"
#include <cassert>

#include "rsrc/gfx/hud_ui.png.h"
#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/prefs_1.tmx.hpp"

#include <Gear.hpp>
#include <32blit.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    std::shared_ptr<GameState> pnk::GSPrefs::update(dang::Gear& gear, uint32_t time)
    {
        if (blit::buttons.pressed & (BTN_BACK | BTN_EXIT))
        {
            return GameState::_gs_home;
        }

        return GameState::_gs_prefs;
    }

    void GSPrefs::enter(dang::Gear& gear, uint32_t time)
    {
        _tmx = &prefs_1_level;
        dang::TmxExtruder txtr(_tmx, &gear);
        dang::RectF vp = {0, 0, 320, 240};

        gear.initLevel(_tmx, vp);
        gear.setActiveWorldSize(vp.w, vp.h);

        txtr.getImagesheets();

        dang::spTileLayer tl = txtr.getTileLayer(tmx_bg_layer_name, true);
        dang::spSpriteLayer dl = txtr.getSpriteLayer(tmx_deco_layer_name, true, true, false);

        std::shared_ptr<SettingsLayer> stl = std::make_shared<SettingsLayer>();
        assert(stl != nullptr);
        stl->_z_order = 2;
        gear.addLayer(stl);
    }

    void GSPrefs::exit(dang::Gear& gear, uint32_t time)
    {
        // load the selected gamestate
        _pnk.refreshGamestateFromSave();
        gear.removeImagesheets();
        gear.removeLayers();
    }
}
