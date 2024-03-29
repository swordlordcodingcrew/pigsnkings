// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include <sprite/ColSpr.hpp>

namespace pnk
{
    class BossbattleTrigger : public dang::ColSpr
    {
    public:
        BossbattleTrigger();
        BossbattleTrigger(const dang::tmx_spriteobject* so);

        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::CollisionObject* other) const override;

        void    restoreTrigger() {_cr = dang::CR_CROSS; }

    protected:

    };

}


