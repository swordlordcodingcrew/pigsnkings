// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "PnkEvent.h"
#include "Event.hpp"

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

}
