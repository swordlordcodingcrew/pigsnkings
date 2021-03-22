// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>
#include <src/Enemy.h>

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;

    enum e_state
    {
        SLEEPING = 0,
        HIDING,
        LOITERING,
        THROWING,
        PICKING_UP
    };

    class HenchPig : public Enemy
    {
    public:
        HenchPig();
        HenchPig(const dang::tmx_spriteobject &so, spImagesheet is);
        ~HenchPig() override;
        void init() override;

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

    protected:

        // the state the pig is in
        uint8_t selectedModule{SLEEPING};

        // change state
        virtual void TryToChangeState(uint8_t wishedState);

    };

}
