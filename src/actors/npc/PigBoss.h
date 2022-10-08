// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "Enemy.h"

#include <DangFwdDecl.h>

namespace pnk
{

    class PigBoss : public Enemy
    {
    public:
        PigBoss();
        PigBoss(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~PigBoss() override;
        void initBT(dang::spNTreeState nts);

        void update(uint32_t dt) override;
        void collide(const dang::manifold &mf) override;
        uint8_t  getCollisionResponse(const dang::CollisionObject* other) const override;

        void bubble() override;
        void endBubble() override;
        bool isBubbled() override;

        void die();

        // animations depot
        dang::spTwAnim _anim_m_sleeping;
        dang::spTwAnim _anim_m_running;
        dang::spTwAnim _anim_m_landing;
        dang::spTwAnim _anim_m_jumping;
        dang::spTwAnim _anim_m_recovering;
        dang::spTwAnim _anim_m_hit;
        dang::spTwAnim _anim_m_die;

        /** path and bt functions */
        void                        startOutToWaypoint() override;
        static dang::BTNode::Status NTLurk(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTRun(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTRunToPOI(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTHit(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTRecover(dang::FullColSpr& s, uint32_t dt);


    protected:

        virtual void tellTheKingWeHitHim();

        // the state the pig is in
        uint8_t _currentState{SLEEPING};
        uint8_t _nextState{SLEEPING};

        // trying to change state, returns true if successful
        virtual void prepareChangeState(e_state wishedState);

        virtual bool onEnterSleeping();
        virtual void endSleep();

        virtual bool onEnterLoitering();
        virtual void endLoitering();

        virtual bool onEnterHiding();
        virtual void endHiding();

        virtual bool onEnterDead();

        virtual void removeSelf();

        const uint8_t _loiter_speed{7};
        const uint8_t _hiding_speed{16};

        bool _hit{false};
    };
}
