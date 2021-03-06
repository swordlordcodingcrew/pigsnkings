// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
//#include <src/actors/npc/Enemy.h>
//#include <src/actors/others/Cannon.h>
#include "HenchPig.h"

namespace pnk
{
    class Cannon;
    using spCannon = std::shared_ptr<Cannon>;

    class PigCannon : public HenchPig
    {
    public:
        PigCannon();
        PigCannon(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~PigCannon() override;
        void init();

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

        bool onEnterSleeping() override;
        bool onEnterThrowing() override;

        void endSleeping();
        virtual void matchLit();
        virtual void lightingCannon();
        virtual void cannonIsLit();

        dang::spTwAnim _anim_m_match_lit;

        spCannon _myCannon{nullptr};

        // Behaviour Tree functions
        static dang::BTNodeStatus BTFireCannon(std::shared_ptr<Sprite> s);

    protected:

    };

}
