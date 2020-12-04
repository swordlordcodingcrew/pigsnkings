// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "PnkEvent.h"
#include "Event.h"

namespace pnk
{
    PnkEvent::PnkEvent(uint16_t filter, int32_t type)
    {
        _filter = filter;
        _type = type;
    }

}
