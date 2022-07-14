// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game


#include "SomaticState.h"
#include "src/pigsnkings.hpp"
#include "src/PnkEvent.h"
#include "src/actors/hero/MotionState.h"
#include "src/actors/hero/Hero.h"

#include <engine/input.hpp>

#include <iostream>

namespace pnk
{

    extern PigsnKings _pnk;

    /************************************************************************
     * somatic state
     */

    std::shared_ptr<EnteringState> SomaticState::_entering = std::make_shared<EnteringState>();
    std::shared_ptr<ExitState> SomaticState::_exit = std::make_shared<ExitState>();
    std::shared_ptr<NormalState> SomaticState::_normal = std::make_shared<NormalState>();
    std::shared_ptr<HitState> SomaticState::_hit = std::make_shared<HitState>();
    std::shared_ptr<LifeLostState> SomaticState::_life_lost = std::make_shared<LifeLostState>();

    uint8_t SomaticState::getCollisionResponse(const Hero &hero, const dang::CollisionObject *other) const
    {
        const dang::ColSpr* cs_other = static_cast<const dang::ColSpr*>(other);

        if (cs_other->typeNum() == ST_HOTRECT_PLATFORM)
        {
            if (cs_other->getHotrectG().top() - 6 >= hero._co_pos.y + hero._hotrect.h && hero.getVel().y > 0)
            {
                return dang::CR_SLIDE;
            }

            return dang::CR_CROSS;
        }
        else if (cs_other->typeNum() == ST_HOTRECT)
        {
            return dang::CR_SLIDE;
        }
        else if (cs_other->typeNum() == ST_ROOM_TRIGGER || cs_other->typeNum() == ST_WARP_ROOM_TRIGGER)
        {
            return dang::CR_CROSS;
        }

        return dang::CR_NONE;

    }




    /**************************************************
     * class EnteringState
     */

    void EnteringState::enter(Hero &hero, uint32_t dt)
    {
        hero.removeAnimation();
        hero.setAnimation(hero._anim_s_blink);
        hero.setGravity({0,0});
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
        hero.setGravity(PigsnKings::_gravity);
        hero.removeAnimation();
        hero._anim_s_life_lost->reset();
        hero.setAnimation(hero._anim_s_life_lost);
        hero.setVel({0,0});
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
        hero.setGravity(PigsnKings::_gravity);

        if (hero._action_state == ActionState::_bubble)
        {
            hero.removeAnimation();
            hero.setAnimation(hero._anim_bubble);
            hero._anim_bubble->setFinishedCallback([&] ()
               {
                   ActionState::_bubble->_bubbling = false;
                   hero._anim_bubble->reset();
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
        if (hero._game_over)
        {
            hero._hit = false;
            hero._life_lost = false;
            hero._game_over = false;
            return SomaticState::_exit;
        }
        else if (hero._life_lost)
        {
            hero._hit = false;
            hero._life_lost = false;
            return SomaticState::_life_lost;
        }
        else if (hero._hit && !_pnk._gamestate.invincible)
        {
            hero._hit = false;
            return SomaticState::_hit;
        }

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

    uint8_t  NormalState::getCollisionResponse(const Hero& hero, const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = static_cast<const dang::ColSpr*>(other);

        if (cs_other->typeNum() == ST_HOTRECT_PLATFORM)
        {
            if (cs_other->getHotrectG().top() - 6 >= hero._co_pos.y + hero._hotrect.h && hero.getVel().y > 0)
            {
                return dang::CR_SLIDE;
            }
            return dang::CR_CROSS;
        }
        else if (cs_other->typeNum() > ST_TRIGGERS && cs_other->typeNum() < ST_TRIGGERS_END
                 || cs_other->typeNum() > ST_REWARDS && cs_other->typeNum() < ST_REWARDS_END)
        {
            return dang::CR_CROSS;
        }
        return dang::CR_SLIDE;
    }


    /**************************************************
     * class HitState
     */

    void HitState::enter(Hero &hero, uint32_t dt)
    {
        hero.removeAnimation();
        hero.setAnimation(hero._anim_s_blink);
        _last_time = 0;
        hero.setVel({0,0});
    }

    std::shared_ptr<SomaticState> HitState::update(Hero &hero, uint32_t dt)
    {
        _last_time += dt;
        if (_last_time > 1200)
        {
            return SomaticState::_normal;
        }

        return SomaticState::_hit;
    }


    /**************************************************
     * class LifeLostState
     */

    void LifeLostState::enter(Hero &hero, uint32_t dt)
    {
        hero.setGravity(PigsnKings::_gravity);
        hero.removeAnimation();
        hero._anim_s_life_lost->reset();
        hero.setAnimation(hero._anim_s_life_lost);
        hero.setVel({0,0});
        _duration = 0;
    }

    std::shared_ptr<SomaticState> LifeLostState::update(Hero &hero, uint32_t dt)
    {
        _duration += dt;
        if (_duration > 3000)
        {
//            hero.setPos(_restart_pos);
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_LIFE_LOST_SEQ_ENDED));
            _pnk._dispatcher.queueEvent(std::move(e));
            return SomaticState::_normal;
        }

        return SomaticState::_life_lost;
    }
}