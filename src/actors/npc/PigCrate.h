// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "HenchPig.h"

namespace pnk
{
    class PigCrate : public HenchPig
    {
    public:
        PigCrate();
        PigCrate(const dang::tmx_spriteobject* so, dang::spImagesheet& is);
        ~PigCrate() override;
        void init() override;

        void update(uint32_t dt) override;

        /** path and bt functions */
        static dang::BTNode::Status NTPickUpCrate(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTThrowCrate(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTWithCrate(dang::FullColSpr& s, uint32_t dt);
//        static dang::BTNodeStatus BTHideInCrate(dang::spSprite s);
        static dang::BTNode::Status NTDistanceOK(dang::FullColSpr& s, uint32_t dt);

    protected:

        bool            onEnterThrowing() override;
        void            endThrowing() override;
        virtual void    throwing();
        bool            _crated{true};
        void            pickupCrate();

        bool            onEnterBubbled() override;


    };

}
