// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <fonts/hud_font_big.h>
#include <fonts/hud_font_small.h>
#include "HUDLayer.hpp"
#include "Sprite.hpp"
#include "Gear.hpp"
#include "GameState.h"

namespace pnk
{
    HUDLayer::HUDLayer() : BaseHUDLayer()
    {
        foregroundColour = blit::Pen(63, 56, 81, 255);
        backgroundColour = blit::Pen(152, 152, 152, 200);
        healthColour = blit::Pen(78, 110, 197, 255);
    }

    void HUDLayer::updateInternal(uint32_t dt, const dang::Gear &gear)
    {

    }

    void HUDLayer::renderInternal(const dang::Gear &gear)
    {
        std::string score =  std::to_string(_pnk._prefs.score);
        std::string prefixedScore = std::string(5 - score.length(), '0') + score;

        std::string lives =  std::to_string(_pnk._prefs.lives);
        std::string prefixedLives = std::string(2 - lives.length(), '0') + lives;

        blit::screen.pen = backgroundColour;
        blit::screen.text(prefixedScore, hud_font_small, blit::Point(54 + shadowOffset, 10 + shadowOffset), true, blit::TextAlign::left);
        blit::screen.text(prefixedLives, hud_font_small, blit::Point(36 + shadowOffset, 44 + shadowOffset), true, blit::TextAlign::center_h);

        blit::screen.pen = foregroundColour;
        blit::screen.text(prefixedScore, hud_font_small, blit::Point(54, 10), true, blit::TextAlign::left);
        blit::screen.text(prefixedLives, hud_font_small, blit::Point(36, 44), true, blit::TextAlign::center_h);

        // health bar
        // TODO: make sure it does not grow bigger than full size
        blit::screen.pen = healthColour;
        float width = _pnk._prefs.health * 0.3;
        blit::screen.rectangle(blit::Rect(58, 36, width, 2));
    }
}
