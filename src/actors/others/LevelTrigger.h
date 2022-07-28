// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include <sprite/ColSpr.hpp>

namespace pnk
{
    class LevelTrigger : public dang::ColSpr
    {
    public:
        LevelTrigger();
        LevelTrigger(const dang::tmx_spriteobject* so);

        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::CollisionObject* other) const override;

        void activateTrigger();

    protected:
        uint8_t _level{1};
        bool _is_activated{false};
    };

}


