// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.h>
#include <Imagesheet.h>
#include <TmxExtruder.h>
#include <iostream>

#include "Hero.h"
#include "MotionState.h"
#include "SomaticState.h"
#include "pigsnkings.hpp"
#include "pnk_globals.h"
#include "PnkEvent.h"

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
        setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        _type_num = TN_HERO;

        // wait animation
        _anim_m_wait = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3, 4, 5}, 600, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), -1, false, 2000));
        // walk animation
        _anim_m_walk = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{8, 9, 10, 11, 12, 13}, 600, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), -1));
        // jump 'animation'
        _anim_m_jump = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{16}, 1, std::unique_ptr<dang::Ease>(new dang::EaseLinear())));
        // on air (= not touching the ground) 'animation'
        _anim_m_on_air = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{17}, 600, std::unique_ptr<dang::Ease>(new dang::EaseLinear())));
        // bubble animation
        _anim_bubble = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{16, 17, 18}, 300, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), 2));
        // entering game animation
        _anim_s_enter = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 6}, 500, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), -1));

    }

    void Hero::activateState()
    {
        // look to the right
        _transform = blit::SpriteTransform::HORIZONTAL;

        // these functions cannot be called in the constructor because it will call shared_from_this() and at the point of construction the shared ptr is not yet initialized
        _somatic_state = SomaticState::_entering;
        _somatic_state->enter(*this, 0);
        _motion_state = MotionState::_on_air;
        _action_state = ActionState::_no_action;
        setAnimation(_anim_s_enter);
    }

    void Hero::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        const dang::Vector2F& normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

        if (normal.y > 0)
        {
            _on_ground = true;
            _vel.y = 0;
        }
        else if (normal.y < 0)
        {
            _top_hit = true;
            _vel.y = 0;
        }

    }

    dang::CollisionSpriteLayer::eCollisionResponse Hero::getCollisionResponse(spSprite other)
    {
        return dang::CollisionSpriteLayer::CR_SLIDE;
//        return _coll_response;
    }

    void Hero::update(uint32_t dt)
    {

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
