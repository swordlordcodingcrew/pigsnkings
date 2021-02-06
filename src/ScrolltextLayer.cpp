// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "Gear.hpp"
#include "ScrolltextLayer.h"
#include "Layer.hpp"
#include "Imagesheet.hpp"

namespace pnk
{
    ScrolltextLayer::ScrolltextLayer() : Layer(dang::Layer::LT_UNDEFINED)
    {
        backgroundColour = blit::Pen(255, 255, 255, 255);
        foregroundColour = blit::Pen(0, 0, 0, 255);
        highlightColour = blit::Pen(221, 107, 6, 255);

        // paint houses^h^h^h clear the background
        blit::screen.pen = backgroundColour;
        displayRect = blit::Rect(0, 0, blit::screen.bounds.w, blit::screen.bounds.h);
        blit::screen.rectangle(displayRect);

        resetScrolling();
    }

    void ScrolltextLayer::update(uint32_t dt, const dang::Gear &gear)
    {
        // Todo dont rely on blit::now but use the delta time instead
        if (lastStartTime == 0) {
            lastStartTime = blit::now();
        }

        // calculate starting_y using the speed constant and elapsed time since start time
        startY = blit::screen.bounds.h - upperBorder - ((blit::now() - lastStartTime) / 40); // where 40 is speed in ms
    }

    void ScrolltextLayer::render(const dang::Gear& gear)
    {
        auto y = startY;

        blit::screen.pen = backgroundColour;
        blit::screen.rectangle(displayRect);

        for (auto sLine : credits){

            blit::screen.pen = foregroundColour;

            // handle special char codes in the credits
            if (sLine.size() > 0 && sLine.at(0) == '*') {
                blit::screen.pen = highlightColour;

                // replace the star, don't print it
                sLine.replace(0, 1, "");
            }

            // render & make sure to clip to screen
            if (y >= 0 && y < blit::screen.bounds.h) {

                blit::screen.text(sLine, blit::minimal_font, blit::Point(blit::screen.bounds.w / 2, y), true, blit::TextAlign::center_h);
            }
            y += rowHeight;
        }

        if (y < 0) {
            resetScrolling();
        }
    }

    void ScrolltextLayer::resetScrolling() {
        startY = blit::screen.bounds.h - upperBorder;
        lastStartTime = 0;
    }
}
