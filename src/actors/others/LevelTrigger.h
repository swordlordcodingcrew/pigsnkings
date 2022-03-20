// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include <CollisionSprite.hpp>

namespace pnk
{
    class LevelTrigger : public dang::CollisionSprite
    {
    public:
        LevelTrigger();
        LevelTrigger(const dang::tmx_spriteobject* so);

        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::spCollisionObject& other) override;

        void activateTrigger();

    protected:
        uint8_t _level{1};
        bool _is_activated{false};
    };

}


