// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.h>

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;
    class Enemy;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;
    using spTwAnim = std::shared_ptr<dang::TwAnim>;

    class Bubble : public dang::CollisionSprite
    {
    public:
        Bubble();
        Bubble(const dang::tmx_spriteobject &so, spImagesheet is);
        ~Bubble() override;
        void    init();
        bool    _to_the_left{true};

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

        bool _enemy_catched{false};
        std::weak_ptr<Enemy> _catched_en;
    };
}

