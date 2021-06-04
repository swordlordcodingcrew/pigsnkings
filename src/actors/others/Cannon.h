// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>
#include <src/actors/npc/Enemy.h>
#include <src/actors/npc/HenchPig.h>

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;
    using spTwAnim = std::shared_ptr<dang::TwAnim>;

    class Cannon : public dang::CollisionSprite
    {
    public:
        Cannon();
        Cannon(const dang::tmx_spriteobject* so, spImagesheet is);
        ~Cannon() override;
        virtual void init();

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

        void fire();
        void cannonHasFired();

        // animations depot
        spTwAnim _anim_m_sleeping;
        spTwAnim _anim_m_shooting;
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
