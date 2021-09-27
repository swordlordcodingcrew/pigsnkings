// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>
#include "HenchPig.h"

namespace pnk
{
    class PigBomb : public HenchPig
    {
    public:
        PigBomb();
        PigBomb(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~PigBomb() override;
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
//        static dang::BTNodeStatus BTPickUpBomb(std::shared_ptr<Sprite> s);
//        static dang::BTNodeStatus BTThrowBomb(std::shared_ptr<Sprite> s);


    protected:

    };

}
