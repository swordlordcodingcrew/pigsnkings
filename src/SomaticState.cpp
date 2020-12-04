// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include <engine/input.hpp>
#include <iostream>

#include "SomaticState.h"
#include "MotionState.h"
#include "Hero.h"
#include "pigsnkings.hpp"

namespace pnk
{

    /************************************************************************
     * somatic state
     */

    std::shared_ptr<EnteringState> SomaticState::_entering = std::make_shared<EnteringState>();
    std::shared_ptr<ExitState> SomaticState::_exit = std::make_shared<ExitState>();
    std::shared_ptr<NormalState> SomaticState::_normal = std::make_shared<NormalState>();

    /**************************************************
     * class EnteringState
     */

    void EnteringState::enter(Hero &hero, uint32_t dt)
    {
        hero.removeAnimation();
        hero.setAnimation(hero._anim_s_enter);
        hero._gravity = {0,0};
        _last_time = 0;
    }

    std::shared_ptr<SomaticState> EnteringState::update(Hero &hero, uint32_t dt)
    {
        _last_time += dt;
        if (_last_time > 800)
        {
            return SomaticState::_normal;
        }

        return SomaticState::_entering;
    }

    /**************************************************
     * class ExitState
     */

    void ExitState::enter(Hero &hero, uint32_t dt)
    {
        hero.removeAnimation();
        hero.setAnimation(hero._anim_s_enter);
        hero._gravity = {0,0};
    }

    std::shared_ptr<SomaticState> ExitState::update(Hero &hero, uint32_t dt)
    {
        return SomaticState::_exit;
    }

    /**************************************************
     * class NormalState
     */

    void NormalState::enter(Hero &hero, uint32_t dt)
    {
//        hero._last_update_time = time;
        hero._gravity = PigsnKings::_gravity;

        if (hero._action_state == ActionState::_bubble)
        {
            hero.removeAnimation();
            hero.setAnimation(hero._anim_bubble);
            hero._anim_bubble->setFinishedCallback([&] ()
               {
                   ActionState::_bubble->_bubbling = false;
               });
        }
        else
        {
            hero.removeAnimation();
            hero._motion_state->setAnimation(hero);
        }

    }

    std::shared_ptr<SomaticState> NormalState::update(Hero &hero, uint32_t dt)
    {

        std::shared_ptr<ActionState> at = hero._action_state->update(hero, dt);
        if (at != hero._action_state)
        {
            hero._action_state = at;
            hero._action_state->enter(hero, dt);

            hero.removeAnimation();
            if (at == ActionState::_bubble)
            {
                hero.setAnimation(hero._anim_bubble);
                hero._anim_bubble->setFinishedCallback([&] ()
                   {
                       ActionState::_bubble->_bubbling = false;
                       hero._anim_bubble->reset();
                   });
            }
            else
            {
                hero._motion_state->setAnimation(hero);
            }
        }
        else
        {
            // process motion state
            std::shared_ptr<MotionState> st = hero._motion_state->update(hero);
            if (st != hero._motion_state)
            {
                hero._motion_state = st;
                hero._motion_state->enter(hero);
                if (hero._action_state == ActionState::_no_action)
                {
                    hero.removeAnimation();
                    st->setAnimation(hero);
                }
            }
        }

        return SomaticState::_normal;
    }

}