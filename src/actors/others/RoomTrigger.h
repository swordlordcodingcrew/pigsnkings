// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include <CollisionSprite.hpp>

namespace pnk
{

    class RoomTrigger : public dang::CollisionSprite
    {
    public:
        RoomTrigger();
        RoomTrigger(const dang::tmx_spriteobject* so, bool warp);

        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;
    protected:
        uint32_t _room{0};
        bool    _warp;
    };

}


