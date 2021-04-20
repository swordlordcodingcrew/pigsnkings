// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "ImageFactory.h"

namespace pnk
{
    blit::Surface *pnk::ImageFactory::createSurface(dang::image_import &ii)
    {
        blit::Surface* s = new blit::Surface(ii.data.data(), blit::PixelFormat::P, ii.bounds);
        s->alpha = ii.alpha;
        s->palette = ii.palette.data();
        return s;
    }

}

