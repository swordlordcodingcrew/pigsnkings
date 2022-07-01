// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "actors/npc/Enemy.h"

#include <DangFwdDecl.h>
#include <sprite/FullSpr.hpp>

namespace pnk
{

    class Cannon : public dang::FullSpr
    {
    public:
        Cannon();
        Cannon(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Cannon() override;
        virtual void init();

        void update(uint32_t dt) override;
        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::CollisionObject* other) const override;

        void fire();
        void cannonHasFired();

        // animations depot
        dang::spTwAnim _anim_m_sleeping;
        dang::spTwAnim _anim_m_shooting;
    protected:

        virtual void tellTheKingWeHitHim();

        // the state the pig is in
        uint8_t _currentState{SLEEPING};
        uint8_t _nextState{SLEEPING};

        // trying to change state, returns true if successful
        virtual void prepareChangeState(e_state wishedState);

        virtual bool onEnterSleeping();
        virtual bool onEnterShooting();

        virtual void removeSelf();
    };
}
