// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <fonts/hud_font_big.h>
#include <fonts/hud_font_small.h>
#include "HUDLayer.hpp"
#include "Sprite.hpp"
#include "Gear.hpp"

namespace pnk
{
    HUDLayer::HUDLayer() : BaseHUDLayer()
    {
        foregroundColour = blit::Pen(63, 56, 81, 255);
        backgroundColour = blit::Pen(152, 203, 216, 200);
        healthColour = blit::Pen(78, 110, 197, 255);
    }

    void HUDLayer::updateInternal(uint32_t dt, const dang::Gear &gear)
    {

    }

    void HUDLayer::renderInternal(const dang::Gear &gear)
    {
        uint8_t smallOffset = 1;

        blit::screen.pen = backgroundColour;
        //blit::screen.text("1-1", hud_font_big, blit::Point(blit::screen.bounds.w / 2 + smallOffset, 20 + smallOffset), true, blit::TextAlign::center_h);

        blit::screen.text("09324", hud_font_small, blit::Point(54 + smallOffset, 10 + smallOffset), true, blit::TextAlign::left);
        blit::screen.text("02", hud_font_small, blit::Point(34 + smallOffset, 44 + smallOffset), true, blit::TextAlign::center_h);

        blit::screen.pen = healthColour;
        blit::screen.rectangle(blit::Rect(58, 36, 30, 2));

        blit::screen.pen = foregroundColour;
        //blit::screen.text("1-1", hud_font_big, blit::Point(blit::screen.bounds.w / 2, 20), true, blit::TextAlign::center_h);

        blit::screen.text("09324", hud_font_small, blit::Point(54, 10), true, blit::TextAlign::left);
        blit::screen.text("02", hud_font_small, blit::Point(34, 44), true, blit::TextAlign::center_h);

        //blit::screen.text("0345", hud_font_small, blit::Point(blit::screen.bounds.w / 2, 45), true, blit::TextAlign::center_h);
    }
}
