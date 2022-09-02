// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include "HenchPig.h"

namespace pnk
{
    class Cannon;
    using spCannon = std::shared_ptr<Cannon>;

    class PigCannon : public HenchPig
    {
    public:
        PigCannon();
        PigCannon(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~PigCannon() override;

        bool onEnterThrowing() override;

        virtual void matchLit();
        virtual void lightingCannon();
        virtual void cannonIsLit();

        dang::spTwAnim _anim_m_match_lit;

        // Behaviour Tree functions
        static dang::BTNode::Status NTFireCannon(dang::FullColSpr& s, uint32_t dt);

    protected:

    };

}
