// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include <sprite/FullSpr.hpp>

namespace pnk
{
    class Moodies : public dang::FullSpr
    {
    public:
        Moodies();
        Moodies(const Moodies& bub);
        Moodies(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Moodies() override;
        virtual void init();

        dang::spTwAnim _anim_m_standard;

    protected:
        friend class SpriteFactory;
    };
}

