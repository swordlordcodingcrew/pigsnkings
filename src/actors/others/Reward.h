// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include <sprite/FullColSpr.hpp>

namespace pnk
{
    class Reward : public dang::FullColSpr
    {
    public:
        Reward();
        Reward(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Reward() override;
        void    init();

        void update(uint32_t dt) override;
        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::CollisionObject* other) const override;

    protected:
        void removeSelf();
        void createRewardEvent(uint8_t rewardType);
        bool _collected{false}; // turns true when hit the first time
    };
}
