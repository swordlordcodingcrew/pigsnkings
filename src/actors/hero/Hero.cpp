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

    void Hero::collide(const dang::manifold &mf)
    {
        dang::spCollisionSprite sprOther = std::static_pointer_cast<CollisionSprite>(mf.me.get() == this ? mf.other : mf.me);

        // check direction
        const dang::Vector2F& normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

        if (_somatic_state == SomaticState::_normal)
        {
            // collision with enemy
            if (sprOther->_type_num > ST_ENEMIES && sprOther->_type_num < ST_ENEMIES_END)
            {
                if (!(sprOther->_type_num == ST_PIG_BOSS && normal.y > 0))
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
                else
                {
                    // pigboss hit from above
                }
            }

            /** hero hits a platform-hotrect from below*/
            if (_cr == dang::CR_CROSS)
            {
                return;
            }

            /** the other has cross as collision response (i.e. rewards) */
            uint8_t cr = sprOther->getCollisionResponse(std::dynamic_pointer_cast<CollisionObject>(shared_from_this()));
            if (cr == dang::CR_CROSS/* || cr == dang::CR_NONE*/)
            {
                return;
            }

            /** hit with something solid */
            if (normal.y > 0)
            {
//                if (mf.other->_type_num == ST_BUBBLE || mf.me->_type_num == ST_BUBBLE)
                if (sprOther->_type_num == ST_BUBBLE)
                {
//                    spBubble bubble = std::static_pointer_cast<Bubble>(mf.me->_type_num == ST_BUBBLE ? mf.me : mf.other);
                    spBubble bubble = std::static_pointer_cast<Bubble>(sprOther);
                    if (bubble->_state != Bubble::bs_enemy_catched)
                    {
                        _on_ground = true;
                        _vel.y = 0;
                    }
                }
                else
                {
                    _on_ground = true;
                    _vel.y = 0;
                }
            }
            else if (normal.y < 0 && sprOther->_type_num != ST_BUBBLE)
            {
                _top_hit = true;
                _vel.y = 0;
            }

        }
        else if (_somatic_state == SomaticState::_hit || _somatic_state == SomaticState::_life_lost)
        {
            /** hit with hotrect */
            if (normal.y > 0 && sprOther->_type_num == ST_HOTRECT)
            {
                _on_ground = true;
                _vel.y = 0;
            }
        }

    }

    uint8_t  Hero::getCollisionResponse(const dang::spCollisionObject& other)
    {
        dang::spCollisionSprite cs_other = std::static_pointer_cast<CollisionSprite>(other);

        if (_somatic_state == SomaticState::_normal)
        {
            if (cs_other->_type_num == ST_HOTRECT_PLATFORM)
            {
                if (cs_other->getHotrectG().top() - 6 >= this->_last_pos_g.y + _hotrect.h && _vel.y > 0)
                {
                    _cr = dang::CR_SLIDE;
                    return _cr;
                }

                _cr = dang::CR_CROSS;
                return _cr;

            }

            _cr = dang::CR_SLIDE;
            return _cr;
        }
        else if (_somatic_state == SomaticState::_hit || _somatic_state == SomaticState::_life_lost)
        {
            if (cs_other->_type_num == ST_HOTRECT_PLATFORM)
            {
                if (cs_other->getHotrectG().top() - 6 >= this->_last_pos_g.y + _hotrect.h && _vel.y > 0)
                {
                    _cr = dang::CR_SLIDE;
                    return _cr;
                }

                _cr = dang::CR_CROSS;
                return _cr;

            }
            else if (cs_other->_type_num == ST_HOTRECT)
            {
                _cr = dang::CR_SLIDE;
                return _cr;
            }

            _cr = dang::CR_NONE;
            return _cr;

        }

        _cr = dang::CR_NONE;
        return _cr;
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
        else if (_hit && !_pnk._gamestate.invincible)
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

    bool Hero::isInNormalState() const
    {
        return _somatic_state == SomaticState::_normal;
    }

}
