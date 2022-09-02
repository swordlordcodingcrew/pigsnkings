// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include "Throwies.h"

namespace pnk
{
    class Cannonball : public Throwies
    {
    public:
        Cannonball();
        Cannonball(const Cannonball& can);
        Cannonball(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Cannonball() override;
//        void init() override;
        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::CollisionObject* other) const override;

    protected:
        void tellTheKingWeHitHim() override;
        void triggerExplosion();

    };
}

