// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <fonts/barcadebrawl.h>
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

        // TODO: start track!!

        int i;
        float rad;

        // precalculate sin lookup table
        for (i = 0; i < 512; i++)
        {
            // 360 / 512 * degree to rad, 360 degrees spread over 512 values to be able to use AND 512-1 instead of using modulo 360
            rad =  ((float)i * 0.703125) * 0.0174532;

            // fixed point math using 1024 as base
            aSin[i] = sin(rad) * 1024;
            aCos[i] = cos(rad) + 1024;
        }

        // precalculate palette
        for (i = 0; i < 64; ++i)
        {
            /* Orig
            colors[i] = blit::Pen(i << 2, 255 - ((i << 2) + 1), 0, alpha);
            colors[i+64] = blit::Pen(255, (i << 2) + 1, 0, alpha);
            colors[i+128] = blit::Pen(255 - ((i << 2) + 1), 255 - ((i << 2) + 1), 0, alpha);
            colors[i+192] = blit::Pen(0, (i << 2) + 1, 0, alpha);
            */

            // add a shade of blue
            colors[i] = blit::Pen(i << 2, 255 - ((i << 2) + 1), 0, alpha);
            colors[i+64] = blit::Pen(255, (i << 2) + 1, 0, alpha);
            colors[i+128] = blit::Pen(255 - ((i << 2) + 1), 255 - ((i << 2) + 1), i << 2, alpha);
            colors[i+192] = blit::Pen(0, (i << 2) + 1, 255, alpha);

            /*
             alternate version with rotation but no pre calculation
              function colorMap3(px, offset, v) {
                 px[offset  ] = 255*(.5+.5*Math.sin(Math.PI*v));
                 px[offset+1] = 255*(.5+.5*Math.sin(Math.PI*v+2*Math.PI/3));
                 px[offset+2] = 255*(.5+.5*Math.sin(Math.PI*v+4*Math.PI/3));
                 px[offset+3] = 255;
               }

             // surface, plasmamap, colourmap
             drawAnimated("plasma4d", function(x, y, time) {
                  var v = Math.sin((x*10+time));
                  v += Math.sin(10*(x*Math.sin(time/2)+y*Math.cos(time/3))+time);
                  var cx = x + .5 * Math.sin(time/5.0);
                  var cy = y + .5 * Math.cos(time/3.0);
                  v += Math.sin(Math.sqrt(100*(cx*cx+cy*cy)+1)+time);
                  return v/2;
              }, colorMapGrey);
             */
        }

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

        // clear screen
        blit::screen.pen = backgroundColour;
        blit::screen.rectangle(displayRect);

        // draw plasma
        uint16_t i,j;
        uint8_t index;
        int plasma;

        // move one step
        tpos4 = pos4;
        tpos3 = pos3;

        for (i = 0; i < blit::screen.bounds.h; ++i)
        {
            tpos1 = pos1 + 5;
            tpos2 = pos2 + 3;

            tpos3 &= 511;
            tpos4 &= 511;

            for (j = 0; j < blit::screen.bounds.w; ++j)
            {
                tpos1 &= 511;
                tpos2 &= 511;

                // plasma calculation
                //plasma = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aSin[tpos4];
                // add a bit of cos effect
                plasma = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aCos[tpos4];

                // fixed point multiplication but optimized
                // so basically it says (plasma * (64 * 1024) / (1024 * 1024)),
                // plasma is already multiplied by 1024
                index = 128 + (plasma >> 4);

                blit::screen.pen = colors[index];
                blit::screen.pixel(blit::Point(j, i));

                tpos1 += 5;
                tpos2 += 3;
            }

            // move position
            tpos4 += 3;
            tpos3 += 1;
        }

        // move plasma
        pos1 +=9;
        pos3 +=8;

        // scrolling text
        for (auto sLine : credits){

            blit::screen.pen = foregroundColour;

            // handle special char codes in the credits
            if (sLine.size() > 0 && sLine.at(0) == '*') {
                blit::screen.pen = highlightColour;

                // remove the star, don't print it
                sLine.replace(0, 1, "");
            }

            // render & make sure to clip to screen
            if (y >= 0 && y < blit::screen.bounds.h) {

                blit::screen.text(sLine, barcadebrawl, blit::Point(blit::screen.bounds.w / 2, y), true, blit::TextAlign::center_h);
            }
            y += rowHeight;
        }

        if (y < 0) {
            resetScrolling();
        }
    }

    void ScrolltextLayer::resetScrolling()
    {
        startY = blit::screen.bounds.h - upperBorder;
        lastStartTime = 0;
    }
}
