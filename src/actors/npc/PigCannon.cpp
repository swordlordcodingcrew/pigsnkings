// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game


#include "PigCannon.h"
#include "../others/Cannon.h"
#include "src/pigsnkings.hpp"

//#include "TmxExtruder.hpp"

#include "src/pnk_globals.h"
//#include "Enemy.h"
#include "src/PnkEvent.h"
//#include "src/GSPlay.h"
//#include "src/SpriteFactory.hpp"

#include <tween/TwAnim.hpp>
#include <tween/TwNull.hpp>
#include <Imagesheet.hpp>

#include <iostream>

namespace pnk
{

    extern PigsnKings _pnk;

    PigCannon::PigCannon() : pnk::HenchPig()
    {
    }

    PigCannon::PigCannon(const dang::tmx_spriteobject* so, dang::spImagesheet is) : pnk::HenchPig(so, is)
    {
    }

    void PigCannon::init()
    {
        _hotrect = {10, 16, 12, 16};
        _walkSpeed = 0; // cannoneers do not walk
        setVel({0, 0});

        // entering the state directly is OK on the first run
        onEnterSleeping();
    }

    PigCannon::~PigCannon()
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("PigCannon destructor\n");
#endif
    }

    void PigCannon::update(uint32_t dt)
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

    bool PigCannon::onEnterThrowing()
    {
        _anim_m_picking_up->reset();
        setAnimation(_anim_m_picking_up);
        _currentState = THROWING;

        removeTweens(true);

        /*
        spTwNull twThrown = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 0));
        twThrown->setFinishedCallback(std::bind(&PigCannon::cannonIsLit, this));
        addTween(twThrown);

        spTwNull twPrepare = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 0));
        twPrepare->setFinishedCallback(std::bind(&PigCannon::lightingCannon, this));
        addTween(twPrepare);

        spTwNull twLightingMatch = std::make_shared<dang::TwNull>(dang::TwNull(1000, dang::Ease::Linear, 0));
        twLightingMatch->setFinishedCallback(std::bind(&PigCannon::matchLit, this));
        addTween(twLightingMatch);
         */

        return true;
    }

    void PigCannon::endSleeping()
    {
        prepareChangeState(THROWING);
    }

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
        std::cout << "lighting cannon " << std::endl;
    }

    void PigCannon::cannonIsLit()
    {
        _myCannon->fire();

        prepareChangeState(SLEEPING);
    }

/*    dang::BTNodeStatus PigCannon::BTFireCannon(std::shared_ptr<Sprite> s)
    {
        std::cout << "firing cannon: " << s->getPos().x << std::endl;
        return dang::BTNodeStatus::FAILURE;
    }
*/}