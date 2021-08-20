// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>

#include "Enemy.h"

namespace pnk
{

    class PigBoss : public Enemy
    {
    public:
        PigBoss();
        PigBoss(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~PigBoss() override;
        void init() override;

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

        void bubble() override;
        void endBubble() override;
        bool isBubbled() override;

        void die();

        // animations depot
        dang::spTwAnim _anim_m_sleeping;
        dang::spTwAnim _anim_m_running;
        dang::spTwAnim _anim_m_landing;
        dang::spTwAnim _anim_m_jumping;
        dang::spTwAnim _anim_m_hit;
        dang::spTwAnim _anim_m_die;

        /** path and bt functions */
        static dang::BTNode::Status NTSleep(dang::spSprite s);
        dang::BTNode::Status        sleep();
        void                        startOutToWaypoint() override;

    protected:

        // inactive bt
        dang::spNTreeState _nTreeStateDepot{nullptr};

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

        virtual bool onEnterRaging();
        virtual void endRaging();

        virtual void removeSelf();

        const uint8_t _loiter_speed{2};
        const uint8_t _raging_speed{6};
    };
}
