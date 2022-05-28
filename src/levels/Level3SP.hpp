// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "ScreenPlay.h"

namespace pnk
{
    class GSPlay;

    class Level3SP : public ScreenPlay
    {
    public:
        Level3SP() = delete;
        explicit Level3SP(GSPlay& gsp) ;
    };

}

