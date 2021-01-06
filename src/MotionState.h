// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <memory>
#include <tween/TwAnim.hpp>
#include "pnk_globals.h"

#define HERO_JUMP blit::Button::A

#define HERO_WALK_L blit::Button::DPAD_LEFT
#define HERO_WALK_R blit::Button::DPAD_RIGHT

namespace pnk
{
//    enum E_HERO_MOTION {WAIT, WALK, JUMP, ON_AIR, BUBBLE};

    class Hero;
    class WaitState;
    class WalkState;
    class JumpState;
    class OnAirState;

    class MotionState
    {
    public:
        static std::shared_ptr<WaitState> _waiting;
        static std::shared_ptr<WalkState> _walking;
        static std::shared_ptr<JumpState> _jumping;
        static std::shared_ptr<OnAirState> _on_air;

//        const float WALK_VEL = 4;
//        const float JUMP_VEL = -8;
//        const uint32_t JUMP_COUNT = 30;

        virtual std::shared_ptr<MotionState> update(Hero& hero) = 0;
        virtual void enter(Hero& hero) = 0;
        virtual void setAnimation(Hero& hero) = 0;

    };

    class WaitState : public MotionState
    {
    public:
        void enter(Hero& hero) override;
        std::shared_ptr<MotionState> update(Hero& hero) override;
        void setAnimation(Hero& hero) override;
    };

    class WalkState : public MotionState
    {
    public:
        void enter(Hero& hero) override;
        std::shared_ptr<MotionState> update(Hero& hero) override;
        void setAnimation(Hero& hero) override;
    };

    class JumpState : public MotionState
    {
    public:
        void enter(Hero& hero) override;
        std::shared_ptr<MotionState> update(Hero& hero) override;
        void setAnimation(Hero& hero) override;
    protected:
        uint32_t _count{H_JUMP_COUNT};
    };

    class OnAirState : public MotionState
    {
    public:
        void enter(Hero& hero) override;
        std::shared_ptr<MotionState> update(Hero& hero) override;
        void setAnimation(Hero& hero) override;
    };

}
