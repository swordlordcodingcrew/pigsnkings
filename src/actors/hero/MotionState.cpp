// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

//#include <engine/input.hpp>
//#include <graphics/sprite.hpp>
#include <libs/DANG/src/snd/SndGear.hpp>
#include "src/pigsnkings.hpp"

#include "MotionState.h"
#include "src/actors/hero/Hero.h"
#include "sfx/jump_22050_mono.h"

#ifdef PNK_DEBUG
#include <iostream>
#endif

namespace pnk
{

    extern PigsnKings _pnk;

    /************************************************************************
     * class Herostate base class
     */

    std::shared_ptr<WaitState> MotionState::_waiting = std::make_shared<WaitState>();
    std::shared_ptr<WalkState> MotionState::_walking = std::make_shared<WalkState>();
    std::shared_ptr<JumpState> MotionState::_jumping = std::make_shared<JumpState>();
    std::shared_ptr<OnAirState> MotionState::_on_air = std::make_shared<OnAirState>();

    bool MotionState::checkLeft()
    {
        return pressed(blit::Button::DPAD_LEFT) || blit::joystick.x < 0;
    }

    bool MotionState::checkRight()
    {
        return pressed(blit::Button::DPAD_RIGHT) || blit::joystick.x > 0;
    }

    bool MotionState::checkJump()
    {
        return pressed(blit::Button::A) || blit::joystick.y < 0;
    }


    /************************************************************************
     * class WaitState
     */

    void WaitState::enter(Hero &hero)
    {
        hero.setVelX(0);
#ifdef PNK_DEBUG
        std::cout << "enter WaitState" << std::endl;
#endif
    }

    std::shared_ptr <MotionState> WaitState::update(Hero &hero)
    {
        if (!hero.isOnGround())
        {
            return MotionState::_on_air;
        }

        if (checkJump())
        {
            if (checkLeft())
            {
                hero._transform = blit::SpriteTransform::HORIZONTAL;
            }
            else if (checkRight())
            {
                hero._transform = blit::SpriteTransform::NONE;
            }

            return MotionState::_jumping;
        }
        else if (checkLeft())
        {
            hero._transform = blit::SpriteTransform::HORIZONTAL;
            return MotionState::_walking;
        }
        else if (checkRight())
        {
            hero._transform = blit::SpriteTransform::NONE;
            return MotionState::_walking;
        }

        return MotionState::_waiting;
    }

    void WaitState::setAnimation(Hero &hero)
    {
        hero.setAnimation(hero._anim_m_wait);
    }

    /************************************************************************
     * class WalkState
     */

    void WalkState::enter(Hero &hero)
    {

        if (hero._transform == blit::SpriteTransform::HORIZONTAL)
        {
            hero.setVelX(-H_WALK_VEL);
        }
        else
        {
            hero.setVelX(H_WALK_VEL);
        }
#ifdef PNK_DEBUG

        std::cout << "enter WalkState" << std::endl;
#endif
    }

    std::shared_ptr<MotionState> WalkState::update(Hero &hero)
    {
        if (!hero.isOnGround())
        {
            return MotionState::_on_air;
        }

        if (checkJump())
        {
            if (checkLeft())
            {
                hero._transform = blit::SpriteTransform::HORIZONTAL;
            }
            else if (checkRight())
            {
                hero._transform = blit::SpriteTransform::NONE;
            }
            else
            {
                hero.setVelX(0);
            }

            return MotionState::_jumping;
        }
        else if (checkLeft())
        {
            if (!(hero._transform == blit::SpriteTransform::HORIZONTAL))
            {
                hero._transform = blit::SpriteTransform::HORIZONTAL;
                hero.setVelX(-H_WALK_VEL);
            }
            return MotionState::_walking;
        }
        else if (checkRight())
        {
            if (!(hero._transform == blit::SpriteTransform::NONE))
            {
                hero._transform = blit::SpriteTransform::NONE;
                hero.setVelX(H_WALK_VEL);
            }
            return MotionState::_walking;
        }

        return MotionState::_waiting;

    }

    void WalkState::setAnimation(Hero &hero)
    {
        hero.setAnimation(hero._anim_m_walk);
    }

    /************************************************************************
    * class JumpState
    */

    void JumpState::enter(Hero &hero)
    {
        _count = H_JUMP_COUNT;
        hero.setVelY(H_JUMP_VEL);

        dang::SndGear::playSfx(jump_22050_mono, jump_22050_mono_length, _pnk._prefs.volume_sfx);
#ifdef PNK_DEBUG
        std::cout << "enter JumpState" << std::endl;
#endif
    }

    std::shared_ptr<MotionState> JumpState::update(Hero &hero)
    {
        if (hero.isOnGround())
        {
            if (checkLeft())
            {
                hero._transform = blit::SpriteTransform::HORIZONTAL;
                return MotionState::_walking;
            }
            else if (checkRight())
            {
                hero._transform = blit::SpriteTransform::NONE;
                return MotionState::_walking;
            }
            else
            {
                return MotionState::_waiting;
            }
        }

        // hero is in jump cycle
        if (_count > 0)
        {
            if (hero.topHit())
            {
                _count = 0;
                return MotionState::_on_air;
            }
            else if (checkJump())
            {
                _count--;
                hero.setVelY(H_JUMP_VEL);

                if (checkLeft())
                {
                    hero.setVelX(-H_WALK_VEL);
                    hero._transform = blit::SpriteTransform::HORIZONTAL;
                }
                else if (checkRight())
                {
                    hero.setVelX(H_WALK_VEL);
                    hero._transform = blit::SpriteTransform::NONE;
                }
                else
                {
                    hero.setVelX(0);
                }

                return MotionState::_jumping;
            }
            else
            {
                // hero ist not jumping anymore and not on ground
                return MotionState::_on_air;
            }
        }

        // hero ist not jumping anymore and not on ground
        return MotionState::_on_air;

    }

    void JumpState::setAnimation(Hero &hero)
    {
        hero.setAnimation(hero._anim_m_jump);
    }

    /************************************************************************
    * class OnAirState
    */

    void OnAirState::enter(Hero &hero)
    {
#ifdef PNK_DEBUG
        std::cout << "enter OnAirState" << std::endl;
#endif
    }

    std::shared_ptr<MotionState> OnAirState::update(Hero &hero)
    {
        if (hero.isOnGround())
        {
            if (checkLeft())
            {
                hero.setVelX(-H_WALK_VEL);
                hero._transform = blit::SpriteTransform::HORIZONTAL;
                return MotionState::_walking;
            }
            else if (checkRight())
            {
                hero.setVelX(H_WALK_VEL);
                hero._transform = blit::SpriteTransform::NONE;
                return MotionState::_walking;
            }
            else
            {
                hero.setVelX(0);
                return MotionState::_waiting;
            }
        }

        if (checkLeft())
        {
            hero.setVelX(-H_WALK_VEL);
            hero._transform = blit::SpriteTransform::HORIZONTAL;
        }
        else if (checkRight())
        {
            hero.setVelX(H_WALK_VEL);
            hero._transform = blit::SpriteTransform::NONE;
        }
        else
        {
            hero.setVelX(0);
        }

        return MotionState::_on_air;
    }

    void OnAirState::setAnimation(Hero &hero)
    {
        hero.setAnimation(hero._anim_m_on_air);
    }
}