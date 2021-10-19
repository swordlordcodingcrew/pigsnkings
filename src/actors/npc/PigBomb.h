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
        PigBomb(const dang::tmx_spriteobject* so, dang::spImagesheet& is);
        ~PigBomb() override;
        void init() override;

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

        // Behaviour Tree functions
        static dang::BTNode::Status NTPickUpBomb(dang::spSprite s);
        static dang::BTNode::Status NTThrowBomb(dang::spSprite s);
        static dang::BTNode::Status NTWithBomb(dang::spSprite s);
        static dang::BTNode::Status NTDistanceOK(dang::spSprite s);

    protected:
        bool            onEnterThrowing() override;
        virtual void    endThrowing() override;
        bool            onEnterBubbled() override;
        virtual void    throwing();
        bool            _with_bomb{true};
        void            pickupBomb();


    };

}
