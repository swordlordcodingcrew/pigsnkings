// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <CollisionSprite.hpp>

namespace pnk
{
    struct tmx_spriteobject;
    class Sprite;
    using spSprite = std::shared_ptr<dang::Sprite>;

    class LevelTrigger : public dang::CollisionSprite
    {
    public:
        LevelTrigger();
        LevelTrigger(const dang::tmx_spriteobject* so);

        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;
    protected:
        uint8_t _level{1};
    };

}


