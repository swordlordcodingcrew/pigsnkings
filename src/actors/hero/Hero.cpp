// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <tween/TwAcc.hpp>
#include <tween/TwVel.hpp>
#include <Imagesheet.hpp>
#include <TmxExtruder.hpp>
#include <iostream>

#include "Hero.h"
#include "MotionState.h"
#include "SomaticState.h"
#include "src/pigsnkings.hpp"
#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/actors/throwies/Bubble.h"
#include "src/SpriteFactory.hpp"

#include "sfx/king_damage_22050.h"

namespace pnk
{
    extern PigsnKings _pnk;

    Hero::Hero() : CollisionSprite()
    {
    }

    Hero::~Hero()
    {
    }

    Hero::Hero(const dang::tmx_spriteobject* so, const std::shared_ptr<dang::Imagesheet> is)
    : dang::CollisionSprite(so, is)
    {
        _hotrect = {8, 8, 16, 24};
    }

    void Hero::activateState()
    {
        // look to the right
        _transform = blit::SpriteTransform::NONE;

        // these functions cannot be called in the constructor because it will call shared_from_this() and at the point of construction the shared ptr is not yet initialized
        _somatic_state = SomaticState::_entering;
        _somatic_state->enter(*this, 0);
        _motion_state = MotionState::_on_air;
        _action_state = ActionState::_no_action;
        setAnimation(_anim_s_blink);
    }

    void Hero::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (_somatic_state == SomaticState::_normal)
        {
            spCollisionSprite other = mf.me.get() == this ? mf.other : mf.me;

            // check direction
            const dang::Vector2F& normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            // collision with enemy
            if (other->_type_num > dang::SpriteType::ENEMIES && other->_type_num < dang::SpriteType::ENEMIES_END)
            {
                // from above
                if(other->_type_num == dang::SpriteType::PIG_BOSS && normal.y > 0)
                {
                    // tell the pig king he is hit, should be stunned for a few rounds
                    std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_BOSS_HIT));
                    e->_spr = shared_from_this();
                    pnk::_pnk._dispatcher.queueEvent(std::move(e));
                }
                else
                {
                    _hit = true;
                    float ax{0};
                    if (mf.me.get() == this)
                    {
                        if (mf.normalMe.x == 0)
                        {
                            ax = _transform == blit::SpriteTransform::NONE ? -9 : 9;
                        }
                        else
                        {
                            ax = mf.normalMe.x > 0 ? -9 : 9;
                        }
                    }
                    else
                    {
                        if (mf.normalOther.x == 0)
                        {
                            ax = _transform == blit::SpriteTransform::NONE ? -9 : 9;
                        }
                        else
                        {
                            ax = mf.normalOther.x > 0 ? -9 : 9;
                        }
                    }
                    dang::spTweenable twa = std::make_shared<dang::TwVel>(dang::Vector2F{ax,-6.0}, PigsnKings::_gravity, 1000, dang::Ease::InQuad);
                    addTween(twa);
                }
            }

            /** hero hits a platform-hotrect */
            if (_coll_response == dang::CollisionSpriteLayer::CR_CROSS)
            {
                return;
            }

            /** the other has cross as collision response (i.e. rewards) */
            if (other->getCollisionResponse(std::dynamic_pointer_cast<CollisionSprite>(shared_from_this())) == dang::CollisionSpriteLayer::CR_CROSS || other->getCollisionResponse(std::dynamic_pointer_cast<CollisionSprite>(shared_from_this())) == dang::CollisionSpriteLayer::CR_NONE)
            {
                return;
            }

            /** hit with something solid */
            if (normal.y > 0)
            {
                if (mf.other->_type_num == dang::SpriteType::BUBBLE || mf.me->_type_num == dang::SpriteType::BUBBLE)
                {
                    spBubble bubble = std::static_pointer_cast<Bubble>(mf.me->_type_num == dang::SpriteType::BUBBLE ? mf.me : mf.other);
                    if (bubble->_state != Bubble::bs_enemy_catched)
                    {
                        _on_ground = true;
                        _vel.y = 0;
                    }
                }
                else if (mf.other->_type_num == dang::SpriteType::PIG_BOSS || mf.me->_type_num == dang::SpriteType::PIG_BOSS)
                {
                    _on_ground = true;
                    _vel.y = 0;

                    // todo plus tell the pigking he was hit
                }
                else
                {
                    _on_ground = true;
                    _vel.y = 0;
                }
            }
            else if (normal.y < 0 && mf.other->_type_num != dang::SpriteType::BUBBLE && mf.me->_type_num != dang::SpriteType::BUBBLE)
            {
                _top_hit = true;
                _vel.y = 0;
            }

        }

    }

    dang::CollisionSpriteLayer::eCollisionResponse Hero::getCollisionResponse(const spCollisionSprite& other)
    {
        if (_somatic_state == SomaticState::_normal || _somatic_state == SomaticState::_hit || _somatic_state == SomaticState::_life_lost)
        {
            if (other->_type_num == dang::SpriteType::HOTRECT_PLATFORM)
            {
                spCollisionSprite cs = std::static_pointer_cast<dang::CollisionSprite>(other);

                if (cs->getHotrectAbs().top() - 6 >= this->_last_pos.y + _hotrect.h && _vel.y > 0)
                {
                    _coll_response = dang::CollisionSpriteLayer::CR_SLIDE;
                    return _coll_response;
                }

                _coll_response = dang::CollisionSpriteLayer::CR_CROSS;
                return _coll_response;

            }

            _coll_response = dang::CollisionSpriteLayer::CR_SLIDE;
            return _coll_response;
        }

        _coll_response = dang::CollisionSpriteLayer::CR_NONE;
        return _coll_response;
    }

    void Hero::update(uint32_t dt)
    {
        if (_life_lost)
        {
            _hit = false;
            _life_lost = false;
            _somatic_state = SomaticState::_life_lost;
            _somatic_state->enter(*this, dt);
        }
        else if (_hit)
        {
            _hit = false;
            _somatic_state = SomaticState::_hit;
            _somatic_state->enter(*this, dt);
        }

        std::shared_ptr<SomaticState> sst = _somatic_state->update(*this, dt);

        // state changed
        if (sst != _somatic_state)
        {
            _somatic_state = sst;
            _somatic_state->enter(*this, dt);
        }

        _on_ground = false;
        _top_hit = false;
    }

    void Hero::lifeLost(const dang::Vector2F& restart_pos)
    {
        _life_lost = true;
        SomaticState::_life_lost->_restart_pos = restart_pos;

    }


}
