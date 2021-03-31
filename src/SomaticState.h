// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>
#include <Vector2T.hpp>

namespace pnk
{
    class Hero;
    class EnteringState;
    class ExitState;
    class HitState;
    class NormalState;
    class LifeLostState;

    class SomaticState
    {
    public:
        static std::shared_ptr<EnteringState> _entering;
        static std::shared_ptr<ExitState> _exit;
        static std::shared_ptr<NormalState> _normal;
        static std::shared_ptr<HitState> _hit;
        static std::shared_ptr<LifeLostState> _life_lost;

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

    class HitState : public SomaticState
    {
    public:
        void enter(Hero& hero, uint32_t dt) override;
        std::shared_ptr<SomaticState> update(Hero& hero, uint32_t dt) override;
    protected:
        uint32_t _last_time{0};
    };

    class LifeLostState : public SomaticState
    {
    public:
        void enter(Hero& hero, uint32_t dt) override;
        std::shared_ptr<SomaticState> update(Hero& hero, uint32_t dt) override;
        dang::Vector2F  _restart_pos{0,0};
    protected:
        uint32_t _duration{0};

    };

}

