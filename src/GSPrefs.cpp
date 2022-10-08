// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game


#include "SettingsLayer.h"

#include "GSPrefs.h"
#include "GSHome.h"

#include "rsrc/gfx/hud_ui.png.h"
#include "rsrc/gfx/castle_tiles.png.h"
#include "rsrc/prefs_1.tmx.hpp"

#include <Gear.hpp>
#include <fonts/barcadebrawl.h>
#include <libs/DANG/src/layer/MessageLayer.hpp>

#include <32blit.hpp>

#include <cassert>
#include <rsrc/game_strings.hpp>


namespace pnk
{
    extern PigsnKings _pnk;

    std::shared_ptr<GameState> pnk::GSPrefs::update(dang::Gear& gear, uint32_t time)
    {
        if (blit::buttons.pressed & (BTN_BACK | BTN_EXIT))
        {
            return GameState::_gs_home;
        }

        if (blit::buttons.pressed & BTN_DELETE && _stl->getSelectedPrefs() == PigsnKings::GAMESAVESLOT)
        {
            _stl->setActive(false);
            _txtl->setText(str_prefs_really_delete);
            _txtl->setTtl(0, [&](blit::Button btn) {

                if (btn == BTN_OK)
                {
                    _pnk.resetGameslot(_pnk._prefs.currentGameSaveSlot);
                    _stl->refreshTempGamestateFromSave(_pnk._prefs.currentGameSaveSlot);
                }
                _stl->setActive(true);
                _txtl->setActive(false);
                _txtl->setVisible(false);
            });
            _txtl->setActive(true);
            _txtl->setVisible(true);

        }



        return GameState::_gs_prefs;
    }

    void GSPrefs::enter(dang::Gear& gear, uint32_t time)
    {
        _tmx = &prefs_1_level;
        dang::TmxExtruder txtr(_tmx, &gear);
        dang::RectF vp = {0, 0, 320, 240};

        gear.initLevel(_tmx, vp);
//        gear.setActiveWorldSize(vp.w, vp.h);

        txtr.getImagesheets();

        dang::spTileLayer tl = txtr.getTileLayer(tmx_bg_layer_name, true);
        dang::spImgSprLayer dl = txtr.getImgSprLayer(tmx_deco_layer_name, true, true, false);

        // settings layer
        dang::PointF p{0, 0};
        _stl = std::make_shared<SettingsLayer>(p, 2, "", true, true);
        assert(_stl != nullptr);
        gear.addLayer(_stl);

        // create text layser
        _txtl = std::make_shared<dang::MessageLayer>(barcadebrawl, p, 10, "", true, false);
        assert(_txtl != nullptr);
        _txtl->setButtons(BTN_OK, BTN_CANCEL);
        gear.addLayer(_txtl);

    }

    void GSPrefs::exit(dang::Gear& gear, uint32_t time)
    {
        // load the selected gamestate
        _pnk.loadCurrentGameState();
        gear.removeImagesheets();
        gear.removeLayers();
    }
}
