// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <Vector2T.h>
#include <Event.h>
#include <memory>

namespace dang
{
    class Sprite;
}

namespace pnk
{
    class PnkEvent : public dang::Event
    {
    public:
        PnkEvent(uint16_t filter, int32_t type);
        dang::Vector2F _pos{0,0};
        bool            _to_the_left{false};
        std::weak_ptr<dang::Sprite> _spr;
    };

}
