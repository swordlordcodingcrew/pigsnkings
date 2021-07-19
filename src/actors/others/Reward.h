// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include <CollisionSprite.hpp>

namespace pnk
{
    class Reward : public dang::CollisionSprite
    {
    public:
        Reward();
        Reward(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Reward() override;
        void    init();

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse  getCollisionResponse(const dang::spCollisionSprite& other) override;

    protected:
        void removeSelf();
        void createRewardEvent(dang::SpriteType rewardType);
        bool _collected{false}; // turns true when hit the first time
    };
}
