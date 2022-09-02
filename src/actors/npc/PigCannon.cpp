// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game


#include "PigCannon.h"
#include "actors/others/Cannon.h"
#include "pigsnkings.hpp"
#include "pnk_globals.h"
#include "PnkEvent.h"

#include <bt/NTreeState.h>
#include <tween/TwAnim.hpp>
#include <tween/TwNull.hpp>
#include <Imagesheet.hpp>
#include <Rand.hpp>

namespace pnk
{

    extern PigsnKings _pnk;

    PigCannon::PigCannon() : pnk::HenchPig()
    {
    }

    PigCannon::PigCannon(const dang::tmx_spriteobject* so, dang::spImagesheet is) : pnk::HenchPig(so, is)
    {
        _hotrect = {10, 16, 12, 16};
        _walkSpeed = 0; // cannoneers do not walk
        setVel({0, 0});
    }


    PigCannon::~PigCannon()
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("PigCannon destructor\n");
#endif
    }

    bool PigCannon::onEnterThrowing()
    {
        _anim_m_picking_up->reset();
        setAnimation(_anim_m_picking_up);
        _currentState = THROWING;

        removeTweens(true);

        return true;
    }

    /**
     * 1. lighting match ("picking up")
     * 2. matchLit
     * 3. lightingCannon
     * 4. cannonIsLit
     */
    void PigCannon::matchLit()
    {
        // make sure to reset the animation
        _anim_m_match_lit->reset();
        setAnimation(_anim_m_match_lit);
    }

    void PigCannon::lightingCannon()
    {
        // make sure to reset the animation
        _anim_m_throwing->reset();
        setAnimation(_anim_m_throwing);
//        printf("lighting cannon\n");
    }

    void PigCannon::cannonIsLit()
    {
        spCannon cannon = std::static_pointer_cast<Cannon>(getChild());
        cannon->fire();

        prepareChangeState(SLEEPING);
    }

    dang::BTNode::Status PigCannon::NTFireCannon(dang::FullColSpr& s, uint32_t dt)
    {
        PigCannon& spr = static_cast<PigCannon&>(s);

        if (spr._currentState != THROWING)
        {
            spr.prepareChangeState(THROWING);
//            printf("(%u) bt lighting cannon\n", spr.id());
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr._currentState == THROWING && spr._nextState == THROWING)
        {
            return dang::BTNode::Status::RUNNING;
        }
        else if (spr._currentState == THROWING && spr._nextState != THROWING)
        {
            return dang::BTNode::Status::SUCCESS;
        }

        return dang::BTNode::Status::FAILURE;

    }
}