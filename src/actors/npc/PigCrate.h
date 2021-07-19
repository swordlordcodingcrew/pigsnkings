// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include "HenchPig.h"

namespace pnk
{
    class PigCrate : public HenchPig
    {
    public:
        PigCrate();
        PigCrate(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~PigCrate() override;
        void init() override;

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

        bool onEnterLoitering() override;
        bool onEnterThrowing() override;

        void endLoitering() override;
        virtual void throwing();
        virtual void endThrowing();

        // Behaviour Tree functions
        static dang::BTNodeStatus BTPickUpCrate(std::shared_ptr<Sprite> s);
        static dang::BTNodeStatus BTThrowCrate(std::shared_ptr<Sprite> s);
        static dang::BTNodeStatus BTHideInCrate(std::shared_ptr<Sprite> s);

    protected:

    };

}
