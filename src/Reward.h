// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    //using spTweenable = std::shared_ptr<dang::Tweenable>;

    class Reward : public dang::CollisionSprite
    {
    public:
        Reward();
        Reward(const dang::tmx_spriteobject &so, spImagesheet is);
        ~Reward() override;
        void    init();

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse  getCollisionResponse(spSprite other) override;

    protected:
        // none for now
    };
}
