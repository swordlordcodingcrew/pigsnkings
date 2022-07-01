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

        // Behaviour Tree functions
        static dang::BTNode::Status NTPickUpBomb(dang::FullSpr& s, uint32_t dt);
        static dang::BTNode::Status NTThrowBomb(dang::FullSpr& s, uint32_t dt);
        static dang::BTNode::Status NTWithBomb(dang::FullSpr& s, uint32_t dt);
        static dang::BTNode::Status NTDistanceOK(dang::FullSpr& s, uint32_t dt);

    protected:
        bool            onEnterThrowing() override;
        virtual void    endThrowing() override;
        bool            onEnterBubbled() override;
        virtual void    throwing();
        void            pickupBomb();

        bool            _with_bomb{true};


    };

}
