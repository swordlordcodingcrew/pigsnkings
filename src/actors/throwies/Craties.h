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
        Craties(const Throwies& bub);
        Craties(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Craties() override;
        void init() override;
        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

    protected:
        void tellTheKingWeHitHim() override;
        void triggerExplosion();

    protected:
        friend class SpriteFactory;
    };
}

