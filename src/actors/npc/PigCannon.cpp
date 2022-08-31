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

    void PigCannon::init()
    {
        if (_nTreeState != nullptr)
        {
            _nTreeStateDefault = _nTreeState;
            _nTreeState->_payload["sleep_duration"] = dang::Rand::get(uint32_t(500), uint32_t(1500));
            onEnterSleeping();
        }
        else
        {
            onEnterSleeping();
        }
    }

    PigCannon::~PigCannon()
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("PigCannon destructor\n");
#endif
    }

/*    void PigCannon::update(uint32_t dt)
    {
        if(_currentState != _nextState)
        {
            // If you need some logic here, have it decided by a BehaviourTree
            switch (_nextState)
            {
                case SLEEPING:
                    onEnterSleeping();
                    break;
                case THROWING:
                    onEnterThrowing();
                    break;
                case PICKING_UP:
                    onEnterPickingUp();
                    break;
                case REMOVE_SELF:
                    removeSelf();
                    break;
            }
        }
    }
*/
/*
    bool PigCannon::onEnterSleeping()
    {
        _anim_m_sleeping->reset();
        setAnimation(_anim_m_sleeping);

        _currentState = SLEEPING;

        // TODO have a behaviour tree exit sleeping
        removeTweens(true);
        dang::spTwNull nullTw = std::make_shared<dang::TwNull>(2000, dang::Ease::Linear, 1);
        nullTw->setFinishedCallback(std::bind(&PigCannon::endSleeping, this));
        addTween(nullTw);

//        std::cout << "sleep entered " << std::endl;

        return true;
    }
*/
    bool PigCannon::onEnterThrowing()
    {
        _anim_m_picking_up->reset();
        setAnimation(_anim_m_picking_up);
        _currentState = THROWING;

        removeTweens(true);

        return true;
    }

/*    void PigCannon::endSleeping()
    {
        prepareChangeState(THROWING);
    }
*/

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