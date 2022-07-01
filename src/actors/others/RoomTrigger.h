// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include <sprite/ColSpr.hpp>

namespace pnk
{

    class RoomTrigger : public dang::ColSpr
    {
    public:
        RoomTrigger();
        RoomTrigger(const dang::tmx_spriteobject* so, bool warp);

        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::CollisionObject* other) const override;

    protected:
        uint32_t _room{0};
        bool    _warp;
    };

}


