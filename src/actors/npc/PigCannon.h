// (c) 2019-21 by SwordLord - the coding crew
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

    class PigCannon : public HenchPig
    {
    public:
        PigCannon();
        PigCannon(const dang::tmx_spriteobject* so, spImagesheet is);
        ~PigCannon() override;
        void init();

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

        bool onEnterSleeping() override;
        bool onEnterThrowing() override;

        void endSleeping();
        virtual void matchLit();
        virtual void lightingCannon();
        virtual void cannonIsLit();

        spTwAnim _anim_m_match_lit;

        // Behaviour Tree functions
        static dang::BTNodeStatus BTFireCannon(std::shared_ptr<Sprite> s);

    protected:

    };

}
