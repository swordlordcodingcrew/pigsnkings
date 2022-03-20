// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "Moodies.h"

#include <DangFwdDecl.h>

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
        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::spCollisionObject& other) override;

    protected:
        bool _has_hurt{false};
        void tellTheKingWeHitHim();
    };
}

