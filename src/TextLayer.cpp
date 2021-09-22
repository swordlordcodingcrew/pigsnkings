// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "TextLayer.hpp"
#include "fonts/barcadebrawl.h"

#include <engine/engine.hpp>
#include <engine/input.hpp>
#include <libs/32blit-sdk/32blit/engine/api.hpp>

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
        if (blit::buttons.pressed & blit::Button::B)
        {
            if (_ttl_cb != nullptr)
            {
                _ttl_cb();
            }
        }

        if (_ttl > 0 && blit::now() - _start_ms > _ttl)
        {
            if (_ttl_cb != nullptr)
            {
                _ttl_cb();
            }
        }
    }

    void TextLayer::render(const dang::Gear &gear)
    {
        // lovely whitish background
        blit::screen.pen = blit::Pen(255, 255, 255, 150);;
        blit::screen.rectangle(_whitishRect);

        // text in black
        blit::screen.pen = blit::Pen(0, 0, 0, 255);;
        for (size_t i = 0; i < _text.size(); ++i)
        {
            blit::screen.text(_text[i], barcadebrawl, blit::Point(blit::screen.bounds.w / 2, 30 + i * 10), true, blit::TextAlign::center_h);
        }

    }

    void TextLayer::setText(const std::string_view& txt)
    {
        _text.clear();

        // split the string_view into vector-entries
        int indexCommaToLeftOfColumn = 0;
        int indexCommaToRightOfColumn = -1;
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

        _whitishRect = {34, 26, blit::screen.bounds.w - 68, int32_t(8 + _text.size() * 10) };
    }

    void TextLayer::setTtl(uint32_t ttl_ms, std::function<void (void)> cb)
    {
        _start_ms = blit::now();
        _ttl = ttl_ms;
        _ttl_cb = cb;
    }
}