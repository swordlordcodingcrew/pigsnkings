// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include <sprite/FullColSpr.hpp>
#include "ActionState.h"

namespace pnk
{
    class MotionState;
    class SomaticState;

    class Hero : public dang::FullColSpr
    {
    public:
        Hero() = default;
        Hero(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Hero() override = default;
        void activateState();

        void update(uint32_t dt) override;

        void        collide(const dang::manifold &mf) override;
        uint8_t     getCollisionResponse(const dang::CollisionObject* other) const override;

        // state stuff
        bool isOnGround() const { return _on_ground; }
        bool topHit() const { return _top_hit; }
        bool isInNormalState() const;
        void lifeLost() { _b_life_lost = true; }
        void gameOver() { _game_over = true; }

    protected:
        friend class SomaticState;
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

        bool    _on_ground{false};
        bool    _top_hit{false};

        bool    _hit{false};
        bool    _b_life_lost{false};
        bool    _game_over{false};

        // state stuff
        std::shared_ptr<SomaticState>   _somatic_state;
        std::shared_ptr<MotionState>    _motion_state;
        std::shared_ptr<ActionState>    _action_state;

        // animations depot
        dang::spTwAnim _anim_m_wait;
        dang::spTwAnim _anim_m_walk;
        dang::spTwAnim _anim_m_jump;
        dang::spTwAnim _anim_m_on_air;
        dang::spTwAnim _anim_bubble;
        dang::spTwAnim _anim_s_blink;
        dang::spTwAnim _anim_s_life_lost;

    };

}

