// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include <CollisionSprite.hpp>

namespace pnk
{
    class BossbattleTrigger : public dang::CollisionSprite
    {
    public:
        BossbattleTrigger();
        BossbattleTrigger(const dang::tmx_spriteobject* so);

        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;
    protected:

    };

}

