// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <memory>
#include <tween/TwAnim.hpp>
#include "src/GSPlay.h"
#include "src/actors/hero/Hero.h"

namespace pnk
{
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

        virtual std::shared_ptr<MotionState> update(Hero& hero) = 0;
        virtual void enter(Hero& hero) = 0;
        virtual void setAnimation(Hero& hero) = 0;

    protected:
        /*
         * functions to check the status of the buttons and joystick and whatnot
         */
        static bool checkLeft();
        static bool checkRight();
        static bool checkJump();

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
        uint32_t _count{GSPlay::H_JUMP_COUNT};
    };

    class OnAirState : public MotionState
    {
    public:
        void enter(Hero& hero) override;
        std::shared_ptr<MotionState> update(Hero& hero) override;
        void setAnimation(Hero& hero) override;
    };

}
