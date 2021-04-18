// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <cstdint>
#include <libs/32blit-sdk/32blit/types/size.hpp>
#include <libs/32blit-sdk/32blit/graphics/surface.hpp>


namespace pnk
{
    class ImageFactory
    {
        struct img_params
        {
            blit::Size bounds;
            uint8_t alpha;
            blit::Pen* palette;
            uint8_t* data;
        };

        static blit::Surface* createSurface(const img_params &p);
    };

}


