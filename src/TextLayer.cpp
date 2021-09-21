// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "TextLayer.hpp"
#include "fonts/barcadebrawl.h"

#include <engine/engine.hpp>

namespace pnk
{

    TextLayer::TextLayer() : Layer(dang::Layer::LT_UNDEFINED)
    {

    }

    TextLayer::~TextLayer()
    {

    }

    void TextLayer::update(uint32_t dt, const dang::Gear &gear)
    {

    }

    void TextLayer::render(const dang::Gear &gear)
    {
        blit::screen.pen = blit::Pen(0, 0, 0, 255);;

        for (size_t i = 0; i < _text.size(); ++i)
        {
            blit::screen.text(_text[i], barcadebrawl, blit::Point(blit::screen.bounds.w / 2, 60 + i * 10), true, blit::TextAlign::center_h);
        }

    }

    void TextLayer::setText(std::string_view& txt)
    {
        int indexCommaToLeftOfColumn = 0;
        int indexCommaToRightOfColumn = -1;

        // split the string_view into
        for (size_t i = 0; i < txt.size(); ++i)
        {
            if (txt[i] == '\n')
            {
                indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
                indexCommaToRightOfColumn = i;
                int index = indexCommaToLeftOfColumn + 1;
                int length = indexCommaToRightOfColumn - index;

                std::string_view column(txt.data() + index, length);
                _text.push_back(column);
            }
        }
        std::string_view finalColumn(txt.data() + indexCommaToRightOfColumn + 1, txt.size() - indexCommaToRightOfColumn - 1);
        _text.push_back(finalColumn);
    }
}