// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>

namespace pnk
{
    class Hero;
    class EnteringState;
    class ExitState;
    class NormalState;
    class PauseState;

    class SomaticState
    {
    public:
        static std::shared_ptr<EnteringState> _entering;
        static std::shared_ptr<ExitState> _exit;
        static std::shared_ptr<NormalState> _normal;

        virtual std::shared_ptr<SomaticState> update(Hero& hero, uint32_t dt) = 0;
        virtual void enter(Hero& hero, uint32_t dt) = 0;
    };

    class EnteringState : public SomaticState
    {
    public:
        void enter(Hero& hero, uint32_t dt) override;
        std::shared_ptr<SomaticState> update(Hero& hero, uint32_t dt) override;
    protected:
        uint32_t _last_time{0};
    };

    class ExitState : public SomaticState
    {
    public:
        void enter(Hero& hero, uint32_t dt) override;
        std::shared_ptr<SomaticState> update(Hero& hero, uint32_t dt) override;
    };

    class NormalState : public SomaticState
    {
    public:
        void enter(Hero& hero, uint32_t dt) override;
        std::shared_ptr<SomaticState> update(Hero& hero, uint32_t dt) override;
    };

}

