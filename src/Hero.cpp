// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <TmxExtruder.hpp>
#include <iostream>

#include "Hero.h"
#include "MotionState.h"
#include "SomaticState.h"
#include "pigsnkings.hpp"
#include "pnk_globals.h"
#include "PnkEvent.h"
#include "Bubble.h"
#include "SpriteFactory.hpp"

namespace pnk
{
    Hero::Hero() : CollisionSprite()
    {
    }

    Hero::~Hero()
    {
    }

    Hero::Hero(const dang::tmx_spriteobject &so, const std::shared_ptr<dang::Imagesheet> is)
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
        setAnimation(_anim_s_enter);
    }

    void Hero::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (_somatic_state == SomaticState::_normal)
        {
            spCollisionSprite other = mf.me.get() == this ? mf.other : mf.me;

            /** collision with enemy */
            if (other->_type_num == SpriteFactory::TN_PIG_NORMAL)
            {
                _hit = true;
            }

            /** hero hits a platform-hotrect */
            if (_coll_response == dang::CollisionSpriteLayer::CR_CROSS)
            {
                return;
            }

            /** the other has cross as collision response (i.e. rewards) */
            if (other->getCollisionResponse(shared_from_this()) == dang::CollisionSpriteLayer::CR_CROSS || other->getCollisionResponse(shared_from_this()) == dang::CollisionSpriteLayer::CR_NONE)
            {
                return;
            }

            /** hit with something solid */
            const dang::Vector2F& normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.y > 0)
            {
                if (mf.other->_type_num == SpriteFactory::TN_BUBBLE || mf.me->_type_num == SpriteFactory::TN_BUBBLE)
                {
                    spBubble bubble = std::static_pointer_cast<Bubble>(mf.me->_type_num == SpriteFactory::TN_BUBBLE ? mf.me : mf.other);
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
            else if (normal.y < 0 && mf.other->_type_num != SpriteFactory::TN_BUBBLE && mf.me->_type_num != SpriteFactory::TN_BUBBLE)
            {
                _top_hit = true;
                _vel.y = 0;
            }

        }

    }

    dang::CollisionSpriteLayer::eCollisionResponse Hero::getCollisionResponse(spSprite other)
    {
        if (_somatic_state == SomaticState::_normal)
        {
            if (other->_type_num == SpriteFactory::TN_HOTRECT_PLATFORM)
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

/*        if (other->_type_num > SpriteFactory::TN_REWARDS && other->_type_num < SpriteFactory::TN_REWARDS_END)
        {
            _coll_response = dang::CollisionSpriteLayer::CR_CROSS;
            return _coll_response;
        }
*/
        _coll_response = dang::CollisionSpriteLayer::CR_NONE;
        return _coll_response;
    }

    void Hero::update(uint32_t dt)
    {
        if (_hit)
        {
            _hit = false;
            _somatic_state = SomaticState::_hit;
            _somatic_state->enter(*this, dt);
        }

        std::shared_ptr<SomaticState> sst = _somatic_state->update(*this, dt);

        if (sst != _somatic_state)
        {
            _somatic_state = sst;
            _somatic_state->enter(*this, dt);
        }

        _on_ground = false;
        _top_hit = false;
    }


/*    void Hero::gameEventReceived(dang::Event &e)
    {
        PnkEvent& pe = static_cast<PnkEvent&>(e);
        if (pe._type == ETA_PAUSE)
        {

        }
*/
}
