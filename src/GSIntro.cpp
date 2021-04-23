// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include <Gear.hpp>
#include <Imagesheet.hpp>
#include <Sprite.hpp>
#include <SpriteLayer.hpp>
#include <32blit.hpp>

#include "GSIntro.h"
#include "GSHome.h"
#include "rsrc/gfx/sl_shield_32blit.png.h"


namespace pnk
{

    std::shared_ptr<GameState> pnk::GSIntro::update(dang::Gear& gear, uint32_t time)
    {
        if (time - _last_time > 1000)
        {
            return GameState::_gs_home;
        }

        return GameState::_gs_intro;

    }

    void GSIntro::enter(dang::Gear& gear, uint32_t time)
    {
        blit::debug("entering intro");

        gear.setWorld({0,0,320, 240});
        gear.setActiveWorldSize(320, 240);
        gear.setViewport({0,0,320, 240});

        blit::debug("vieport set");

//        dang::SizeU size(104, 200);
        spImagesheet is = std::make_shared<dang::Imagesheet>("sl_shield", &img_sl_shield);
        gear.addImagesheet(is);

        blit::debug("imagesheet loaded");

        spSprite sp = std::make_shared<dang::Sprite>();
        sp->setImagesheet(is);
        sp->setSize(is->getImagesheetSizeW(), is->getImagesheetSizeH());
        sp->setPos((gear.getViewport().br() - sp->getSize()) / 2.0f);

        blit::debug("sprite set up");

        spSpriteLayer sl = std::make_shared<dang::SpriteLayer>();
        sl->addSprite(sp);
        gear.addLayer(sl);
        _last_time = blit::now();

        blit::debug("layer set up");

    }

    void GSIntro::exit(dang::Gear& gear, uint32_t time)
    {
        // remove spritesheets
//        spImagesheet is = gear.getImagesheet("sl_shield");
        _pnk.removeImagesheets();

        // clear gear
//        gear.removeImagesheets();
        gear.removeLayers();

    }

}
