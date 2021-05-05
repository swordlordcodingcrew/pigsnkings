// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>
#include <src/actors/npc/Enemy.h>
#include "HenchPig.h"

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;

    class PigCrate : public HenchPig
    {
    public:
        PigCrate();
        PigCrate(const dang::tmx_spriteobject* so, spImagesheet is);
        ~PigCrate() override;
        void init() override;

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

        bool onEnterLoitering() override;
        bool onEnterThrowing() override;

        void endLoitering() override;
        virtual void throwing();
        virtual void endThrowing();

    protected:

    };

}
