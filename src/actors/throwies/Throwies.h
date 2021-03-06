// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include <CollisionSprite.hpp>

namespace pnk
{

    class Throwies : public dang::CollisionSprite
    {
    public:
        Throwies();
        Throwies(const Throwies& bub);
        Throwies(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Throwies() override;
        virtual void init();
        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

        bool    _to_the_left{true};

        // animations depot
        dang::spTwAnim _anim_flying; //spTwAnim twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{41, 42, 43, 44, 45}, 600, &dang::Ease::OutQuad, 0);

    protected:
        void removeSelf();
        virtual void tellTheKingWeHitHim();

    protected:
        friend class SpriteFactory;

        bool    _remove_me{false};
    };
}

