// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <32blit.hpp>
#include <ImageImport.h>


namespace pnk
{
    class ImageFactory
    {
    public:
        static blit::Surface* createSurface(dang::image_import &ii);
    };

}


