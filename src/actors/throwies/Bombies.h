// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>
#include "Throwies.h"

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;
    class Enemy;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;
    using spTwAnim = std::shared_ptr<dang::TwAnim>;

    class Bombies : public Throwies
    {
    public:
        Bombies();
        Bombies(const Throwies& bub);
        Bombies(const dang::tmx_spriteobject* so, spImagesheet is);
        ~Bombies() override;
        void init() override;
        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

    protected:
        void tellTheKingWeHitHim() override;
        void triggerExplosion();

    protected:
        friend class SpriteFactory;
    };
}

