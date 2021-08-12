// (c) 2019-1 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "ScreenPlay.h"

namespace pnk
{
    class GSPlay;

    class Level1SP : public ScreenPlay
    {
    public:
        Level1SP() = delete;
        explicit Level1SP(GSPlay& gsp) ;

    };

}

