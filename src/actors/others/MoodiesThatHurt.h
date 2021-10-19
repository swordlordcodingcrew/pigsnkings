// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include "Moodies.h"

namespace pnk
{
    class MoodiesThatHurt : public Moodies
    {
    public:
        MoodiesThatHurt();
        explicit MoodiesThatHurt(const Moodies& master);
        MoodiesThatHurt(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~MoodiesThatHurt() override;
        void init() override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

    protected:
        bool _has_hurt{false};
        void tellTheKingWeHitHim();
    };
}

