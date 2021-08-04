// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>

#include "Enemy.h"

namespace pnk
{
    enum e_state
    {
        SLEEPING = 0,
        HIDING,
        LOITERING,
        THROWING,
        FIRING,
        PICKING_UP,
        BUBBLED, // this one is tricky, since it it managed by the Enemy base class
        REMOVE_SELF
    };

    class HenchPig : public Enemy
    {
    public:
        HenchPig();
        HenchPig(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~HenchPig() override;
        void init() override;

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

        void bubble() override;
        void deBubble() override;
        bool isBubbled() override;

        // path
        void startOutToWaypoint() override;
        dang::BTNode::Status sleep();

        // animations depot
        dang::spTwAnim _anim_m_sleeping;
        dang::spTwAnim _anim_m_hiding;
        dang::spTwAnim _anim_m_loitering;
        dang::spTwAnim _anim_m_throwing;
        dang::spTwAnim _anim_m_picking_up;
        dang::spTwAnim _anim_m_bubbling;

        static dang::BTNode::Status NTSleep(dang::spSprite s);


    protected:

        // inactive bt
        dang::spNTreeState _btDepot{nullptr};

        virtual void tellTheKingWeHitHim();

        // the state the pig is in
        uint8_t _currentState{SLEEPING};
        uint8_t _nextState{SLEEPING};

        // trying to change state, returns true if successful
        virtual void prepareChangeState(e_state wishedState);

        virtual bool onEnterSleeping();
        virtual bool onEnterHiding();
        virtual bool onEnterLoitering();
        virtual bool onEnterThrowing();
        virtual bool onEnterPickingUp();
        virtual bool onEnterBubbled();

        virtual void endSleep();
        virtual void endLoitering();

        virtual void poofing();

        virtual void removeSelf();

        const uint8_t _loiter_speed{2};
    };
}
