// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include <sprite/FullSpr.hpp>

namespace pnk
{

    class Throwies : public dang::FullSpr
    {
    public:
        Throwies();
        Throwies(const Throwies& bub);
        Throwies(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Throwies() override;
        virtual void init();
        void update(uint32_t dt) override;
        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::CollisionObject* other) const override;

        bool    _to_the_left{true};

        // animations depot
        dang::spTwAnim _anim_flying;
        dang::spTwAnim _anim_destruction;

    protected:
        virtual void tellTheKingWeHitHim();

    protected:
        friend class SpriteFactory;

        bool    _remove_me{false};
    };
}

