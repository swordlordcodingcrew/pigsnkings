// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>

namespace pnk
{
    class Hero;
    class BubbleState;
    class NoActionState;

    class ActionState
    {
    public:
        static std::shared_ptr<BubbleState> _bubble;
        static std::shared_ptr<NoActionState> _no_action;

        virtual std::shared_ptr<ActionState> update(Hero& hero, uint32_t dt) = 0;
        virtual void enter(Hero& hero, uint32_t dt) = 0;
    };

    class BubbleState : public ActionState
    {
    public:
        void enter(Hero& hero, uint32_t dt) override;
        std::shared_ptr<ActionState> update(Hero& hero, uint32_t dt) override;
        bool _bubbling{true};
    };

    class NoActionState : public ActionState
    {
    public:
        void enter(Hero& hero, uint32_t dt) override;
        std::shared_ptr<ActionState> update(Hero& hero, uint32_t dt) override;
    };
}