// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game

#include "Hero.h"
#include "MotionState.h"
#include "SomaticState.h"

#include "pigsnkings.hpp"
#include "pnk_globals.h"
#include "actors/throwies/Bubble.h"

#include <tween/TwVel.hpp>
#include <Imagesheet.hpp>

namespace pnk
{
    using spBubble = std::shared_ptr<Bubble>;

    extern PigsnKings _pnk;

    Hero::Hero(const dang::tmx_spriteobject* so, const dang::spImagesheet is)
    : dang::FullColSpr(so, is)
    {
        _hotrect = {8, 8, 16, 24};
    }

    void Hero::activateState()
    {
        // look to the right
        setTransform(blit::SpriteTransform::NONE);

        _somatic_state = SomaticState::_entering;
        _somatic_state->enter(*this, 0);
        _motion_state = MotionState::_on_air;
        _action_state = ActionState::_no_action;
        setAnimation(_anim_s_blink);
    }

    void Hero::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        const dang::CollisionObject* co_this = static_cast<dang::CollisionObject*>(this);
        const dang::Vector2F& normal = mf.me.get() == co_this ? mf.normalMe : mf.normalOther;

        if (_somatic_state == SomaticState::_normal)
        {
            /** collision with enemy */
            if (sprOther->typeNum() > ST_ENEMIES && sprOther->typeNum() < ST_ENEMIES_END)
            {
                if (!(sprOther->typeNum() == ST_PIG_BOSS && normal.y > 0))
                {
                    _hit = true;
                    float ax{0};
                    if (mf.me.get() == this)
                    {
                        if (mf.normalMe.x == 0)
                        {
                            ax = getTransform() == blit::SpriteTransform::NONE ? -9 : 9;
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
                            ax = getTransform() == blit::SpriteTransform::NONE ? -9 : 9;
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
            /** stand on bubble */
            else if (sprOther->typeNum() == ST_BUBBLE && normal.y > 0)
            {
                spBubble bubble = std::static_pointer_cast<Bubble>(sprOther);
                if (bubble->_state != Bubble::bs_enemy_catched)
                {
                    _on_ground = true;
                    setVelY(0);
                }
            }
            /** platform specific collision */
            else if (sprOther->typeNum() == ST_HOTRECT_PLATFORM)
            {
                if (sprOther->getHotrectG().top() - 6 >= _co_pos.y + _hotrect.h > 0 && getVel().y >= 0)
                {
                    _on_ground = true;
                    setVelY(0);
                }
            }
            /** stand on something solid */
            else if (sprOther->typeNum() == ST_HOTRECT)
            {
                if (normal.y > 0)
                {
                    _on_ground = true;
                    setVelY(0);
                }
                else if (normal.y < 0)
                {
                    _top_hit = true;
                    setVelY(0);
                }
                else if (normal.x != 0)
                {
                    setVelX(0);
                }
            }
        }
        else if (_somatic_state == SomaticState::_hit || _somatic_state == SomaticState::_life_lost || _somatic_state == SomaticState::_exit)
        {
            /** hit with hotrect */
            if (normal.y > 0 && sprOther->typeNum() == ST_HOTRECT)
            {
                _on_ground = true;
                setVelY(0);
            }
        }

    }

    uint8_t  Hero::getCollisionResponse(const dang::CollisionObject* other) const
    {
        return _somatic_state->getCollisionResponse(const_cast<Hero&>(*this), other);
    }

    void Hero::update(uint32_t dt)
    {
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

    void Hero::lifeLost()
    {
        _life_lost = true;
    }

    bool Hero::isInNormalState() const
    {
        return _somatic_state == SomaticState::_normal;
    }

}
