// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "PnkEvent.h"
#include "Event.hpp"
#include "pnk_globals.h"

namespace pnk
{
    PnkEvent::PnkEvent(uint16_t filter, int32_t type)
    {
        _filter = filter;
        _type = type;
    }

    PnkEvent::PnkEvent(uint16_t filter, int32_t type, uint16_t payload)
    {
        _filter = filter;
        _type = type;
        _payload = payload;
    }

    PnkEvent::~PnkEvent()
    {
//        std::cout << "PnkEvent destructor" << std::endl;
        _spr.reset();
    }

    std::unique_ptr<PnkEvent> PnkEvent::createGE(int32_t type, uint16_t payload)
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, type, payload));
        return std::move(e);
    }

    std::unique_ptr<PnkEvent> PnkEvent::createGE(int32_t type, std::weak_ptr<dang::Sprite> spr)
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, type));
        e->_spr = spr;
        return std::move(e);
    }

    std::unique_ptr<PnkEvent> PnkEvent::createGE(int32_t type, uint16_t payload, std::weak_ptr<dang::Sprite> spr)
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, type, payload));
        e->_spr = spr;
        return std::move(e);
    }

}
