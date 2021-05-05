// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;
    class Enemy;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;
    using spTwAnim = std::shared_ptr<dang::TwAnim>;

    class Moodies : public dang::CollisionSprite
    {
    public:
        Moodies();
        Moodies(const Moodies& bub);
        Moodies(const dang::tmx_spriteobject* so, spImagesheet is);
        ~Moodies() override;
        virtual void init();
        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

        spTwAnim _anim_m_standard;

    protected:
        void removeSelf();

    protected:
        friend class SpriteFactory;

        bool    _remove_me{false};
    };
}

