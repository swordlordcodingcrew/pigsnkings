// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "ImageFactory.h"

namespace pnk
{
    blit::Surface *pnk::ImageFactory::createSurface(const pnk::ImageFactory::img_params &p)
    {
        blit::Surface* s = new blit::Surface(p.data, blit::PixelFormat::P, p.bounds);
        s->alpha = p.alpha;
        s->palette = p.palette;
        return s;
    }

}

