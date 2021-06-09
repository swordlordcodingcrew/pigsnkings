// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Moodies.h"

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;
    class Enemy;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;
    using spTwAnim = std::shared_ptr<dang::TwAnim>;

    class MoodiesThatHurt : public Moodies
    {
    public:
        MoodiesThatHurt();
        explicit MoodiesThatHurt(const Moodies& master);
        MoodiesThatHurt(const dang::tmx_spriteobject* so, spImagesheet is);
        ~MoodiesThatHurt() override;
        void init() override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

    protected:
        bool _has_hurt{false};
    };
}

