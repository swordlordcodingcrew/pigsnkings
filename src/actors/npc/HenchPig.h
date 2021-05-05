// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>
#include <src/actors/npc/Enemy.h>

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;
    using spTwAnim = std::shared_ptr<dang::TwAnim>;

    enum e_state
    {
        SLEEPING = 0,
        HIDING,
        LOITERING,
        THROWING,
        PICKING_UP,
        BUBBLED, // this one is tricky, since it it managed by the Enemy base class
        REMOVE_SELF
    };

    class HenchPig : public Enemy
    {
    public:
        HenchPig();
        HenchPig(const dang::tmx_spriteobject* so, spImagesheet is);
        ~HenchPig() override;
        void init() override;

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

        void bubble() override;
        void deBubble() override;
        bool isBubbled() override;

        // animations depot
        spTwAnim _anim_m_sleeping;
        spTwAnim _anim_m_hiding;
        spTwAnim _anim_m_loitering;
        spTwAnim _anim_m_throwing;
        spTwAnim _anim_m_picking_up;
        spTwAnim _anim_m_bubbling;
    protected:

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
