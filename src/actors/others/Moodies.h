// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include <CollisionSprite.hpp>

namespace pnk
{
    class Moodies : public dang::CollisionSprite
    {
    public:
        Moodies();
        Moodies(const Moodies& bub);
        Moodies(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Moodies() override;
        virtual void init();
        void update(uint32_t dt) override;
        uint8_t  getCollisionResponse(const dang::spCollisionObject& other) override;

        dang::spTwAnim _anim_m_standard;

    protected:

    protected:
        friend class SpriteFactory;

        bool    _remove_me{false};
    };
}

