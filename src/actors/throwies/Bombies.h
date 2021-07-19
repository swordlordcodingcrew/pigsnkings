// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include "Throwies.h"

namespace pnk
{
    class Bombies : public Throwies
    {
    public:
        Bombies();
        Bombies(const Bombies& bub);
        Bombies(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Bombies() override;
        void init() override;
        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

        void setBombOnFire();
        void triggerExplosion();

    protected:
        void tellTheKingWeHitHim() override;

        dang::spTwAnim    _anim_on_fire;
        bool        _bIsOnFire{false};

    protected:
        friend class SpriteFactory;
    };
}

