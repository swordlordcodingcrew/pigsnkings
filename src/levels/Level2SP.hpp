// (c) 2019-1 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "ScreenPlay.h"

namespace pnk
{
    class GSPlay;

    class Level2SP : public ScreenPlay
    {
    public:
        Level2SP() = delete;
        explicit Level2SP(GSPlay& gsp) ;

    };

}

