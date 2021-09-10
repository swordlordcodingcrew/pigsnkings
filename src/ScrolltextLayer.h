// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework
#pragma once

#include <list>

#include "Layer.hpp"
#include "32blit.hpp"

namespace pnk
{
class ScrolltextLayer : public dang::Layer
    {
    public:
        ScrolltextLayer();
        ~ScrolltextLayer() override;

        void    update(uint32_t dt, const dang::Gear& gear) override;
        void    render(const dang::Gear& gear) override;

    protected:
        explicit ScrolltextLayer(Layer::E_TYPE type) : Layer(type) {};

        void resetScrolling();
        void paintFrame(const dang::Gear& gear);

        std::shared_ptr<dang::Imagesheet> is;


    // static as well
        uint16_t pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0, tpos1, tpos2, tpos3, tpos4;
        int aSin[512];
        int aCos[512];

        uint8_t alpha = 50;
        blit::Pen colors[256];

        blit::Pen backgroundColour;
        blit::Pen foregroundColour;
        blit::Pen highlightColour;

        blit::Rect displayRect;

        int32_t startY = 0;
        uint32_t lastStartTime = 0;

        const int upperBorder = 24;
        const int rowHeight = 12;

        const std::vector<std::string> credits = {
                "",
                "*Pigs n Kings",
                "(c) 2020-2021 by",
                "SwordLord - the coding crew",
                "",
                "",
                "Once upon a time,",
                "there was a King.",
                "With a mighty Hammer",
                "to reign his lands.",
                "",
                "Then came a pig",
                "and took the Hammer away.",
                "",
                "Darkness fell over the kingdom.",
                "",
                "When the kings mother",
                "heard the King swear against",
                "that **** pig,",
                "she washed the Kings mouth",
                "with soap.",
                "",
                "So now the King blows Bubbles.",
                "",
                "",
                "And hunts those pigs",
                "through his castle.",
                "",
                "",
                "",
                "",
                "",
                "Pigs n Kings is made with the",
                "*mighty DANG game framework",
                "",
                "",
                "*Head Looney",
                "LordFilu (smelly)",
                "",
                "",
                "*Head of Pig Training",
                "LordEidi",
                "",
                "",
                "*GFX",
                "Pixel Frog",
                "(Thanks for great sprites!)",
                "",
                "LordLensflare",
                "",
                "",
                "*Tracks",
                "U4ia (woopwoop)"
                "",
                "",
                "*SFX",
                "LordMeep",
                "",
                "LordBoing",
                "",
                "",
                "*Quality Assurance",
                "..."
        };
    };
}

