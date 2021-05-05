// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>
#include <Event.hpp>
#include "ActionState.h"

namespace pnk
{
    class MotionState;
    class SomaticState;
    struct tmx_spriteobject;
    class Imagesheet;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;
    using spTwAnim = std::shared_ptr<dang::TwAnim>;

    class Hero : public dang::CollisionSprite
    {
    public:
        Hero();
        Hero(const dang::tmx_spriteobject* so, std::shared_ptr<dang::Imagesheet> is);
        ~Hero() override;
        void activateState();

        void update(uint32_t dt) override;

        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

        // state stuff
        bool isOnGround() const { return _on_ground; }
        bool topHit() const { return _top_hit; }

        void lifeLost(const dang::Vector2F& restart_pos);

    protected:
        friend class NormalState;
        friend class EnteringState;
        friend class ExitState;
        friend class HitState;
        friend class LifeLostState;
        friend class WaitState;
        friend class WalkState;
        friend class JumpState;
        friend class OnAirState;
        friend class SpriteFactory;

//        void setLastUpdateTime(uint32_t time) { _last_update_time = time; }

        bool    _on_ground{false};
        bool    _top_hit{false};
        bool    _hit{false};
        bool    _life_lost{false};

        // state stuff
        std::shared_ptr<SomaticState>   _somatic_state;
        std::shared_ptr<MotionState>    _motion_state;
        std::shared_ptr<ActionState>    _action_state;


        // animations depot
        spTwAnim _anim_m_wait;
        spTwAnim _anim_m_walk;
        spTwAnim _anim_m_jump;
        spTwAnim _anim_m_on_air;
        spTwAnim _anim_bubble;
        spTwAnim _anim_s_blink;
        spTwAnim _anim_s_life_lost;

    };

}

