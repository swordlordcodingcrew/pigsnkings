// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Enemy.h"

namespace pnk
{

    class HenchPig : public Enemy
    {
    public:
        HenchPig();
        HenchPig(const dang::tmx_spriteobject* so, const dang::spImagesheet& is);
        ~HenchPig() override;
        void init() override;

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(const dang::spCollisionSprite& other) override;

        void bubble() override;
        void endBubble() override;
        bool isBubbled() override;


        /** animations depot */
        dang::spTwAnim _anim_m_sleeping{nullptr};
        dang::spTwAnim _anim_m_hiding{nullptr};
        dang::spTwAnim _anim_m_loitering{nullptr};
        dang::spTwAnim _anim_m_throwing{nullptr};
        dang::spTwAnim _anim_m_picking_up{nullptr};
        dang::spTwAnim _anim_m_bubbling{nullptr};
        dang::spTwAnim _anim_empty_sleeping{nullptr};
        dang::spTwAnim _anim_empty_loitering{nullptr};

        /** path and bt functions */
        static dang::BTNode::Status NTNap(dang::spSprite s);                //!< 0.5 - 1.5 sec of sleep state
        static dang::BTNode::Status NTSleep(dang::spSprite s);              //!< 2 - 4 sec of sleep state
        static dang::BTNode::Status NTNarcolepsy(dang::spSprite s);         //!< 5 - 10 sec of sleep state

        dang::BTNode::Status        sleep();
        void                        startOutToWaypoint() override;

        void setNTreeBerserk(dang::spNTreeState berserk) { _nTreeStateBerserk = berserk; }

    protected:

        // inactive bts
        dang::spNTreeState _nTreeStateDefault{nullptr};
        dang::spNTreeState _nTreeStateBerserk{nullptr};

        virtual void tellTheKingWeHitHim();

        // the state the pig is in
        uint8_t _currentState{SLEEPING};
        uint8_t _nextState{SLEEPING};

        // trying to change state, returns true if successful
        virtual void prepareChangeState(e_state wishedState);

        virtual bool onEnterSleeping();
        virtual void endSleep();

        virtual bool onEnterHiding();

        virtual bool onEnterLoitering();
        virtual void endLoitering();

        virtual bool onEnterBerserk();
        virtual void endBerserk();

        virtual bool onEnterThrowing();
        virtual void endThrowing();

        virtual bool onEnterPickingUp();
        virtual bool onEnterBubbled();

        virtual void poofing();

        virtual void removeSelf();

        const uint8_t _loiter_speed{2};
        const uint8_t _berserk_speed{6};
    };
}
