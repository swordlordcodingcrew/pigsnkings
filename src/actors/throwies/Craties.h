// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include "Throwies.h"

namespace pnk
{

    class Craties : public Throwies
    {
    public:
        Craties();
        Craties(const Craties& bub);
        Craties(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Craties() override;
        void init() override;
        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::spCollisionObject& other) override;

    protected:
        void tellTheKingWeHitHim() override;
        void triggerExplosion();

    protected:
        friend class SpriteFactory;
    };
}

