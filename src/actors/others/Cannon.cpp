// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <TmxExtruder.hpp>

#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"
#include "src/pigsnkings.hpp"

#include "Cannon.h"

namespace pnk
{

    extern PigsnKings _pnk;

    Cannon::Cannon() : dang::CollisionSprite()
    {
    }

    Cannon::Cannon(const dang::tmx_spriteobject* so, spImagesheet is) : dang::CollisionSprite(so, is)
    {
    }

    void Cannon::init()
    {
        _hotrect = {10, 16, 12, 16};

        onEnterSleeping();

        setVel({0, 0});
    }

    Cannon::~Cannon()
    {
#ifdef PNK_DEBUG
        std::cout << "Cannon destructor" << std::endl;
#endif
    }

    void Cannon::update(uint32_t dt)
    {
        if(_currentState != _nextState)
        {
            // If you need some logic here, have it decided by a BehaviourTree
            switch (_nextState)
            {
                case SLEEPING:
                    onEnterSleeping();
                    break;
                case FIRING:
                    onEnterShooting();
                    break;
            }
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse Cannon::getCollisionResponse(const spCollisionSprite& other)
    {
        /** run into the king */
        if (other->_type_num == ST_KING)
        {
            _coll_response = dang::CollisionSpriteLayer::CR_BOUNCE;
        }
        /** hit a platform hotrect */
        else if (other->_type_num == ST_HOTRECT_PLATFORM)
        {
            spCollisionSprite cs = std::static_pointer_cast<dang::CollisionSprite>(other);

            if (cs->getHotrectG().top() - 6 >= this->_last_pos.y + _hotrect.h && _vel.y > 0)
            {
                _coll_response = dang::CollisionSpriteLayer::CR_SLIDE;
            }
            else
            {
                _coll_response = dang::CollisionSpriteLayer::CR_CROSS;
            }
        }
        else
        {
            _coll_response = dang::CollisionSpriteLayer::CR_SLIDE;
        }

        return _coll_response;
    }

    void Cannon::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        spCollisionSprite sprOther = mf.me.get() == this ? mf.other : mf.me;

        if (sprOther->_type_num == ST_KING)
        {
            tellTheKingWeHitHim();
        }
        else if (_coll_response == dang::CollisionSpriteLayer::CR_SLIDE)
        {
            const dang::Vector2F &normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.x != 0)
            {
                _vel.x = 0;
            }

            if (normal.y > 0)
            {
                _vel.y = 0;
            }
        }
    }

    void Cannon::fire()
    {
        prepareChangeState(FIRING);
    }

    void Cannon::prepareChangeState(e_state wishedState)
    {
        // TODO We could have some logic here as well, or in the update routine?
        _nextState = wishedState;
    }

    bool Cannon::onEnterSleeping()
    {
        if(_anim_m_sleeping != nullptr)
        {
            _anim_m_sleeping->reset();
            setAnimation(_anim_m_sleeping);
        }
        else
        {
            std::cerr << "_anim_m_sleeping is not set in Cannon" << std::endl;
        }
        _currentState = SLEEPING;

        return true;
    }

    bool Cannon::onEnterShooting()
    {
        _anim_m_shooting->reset();
        setAnimation(_anim_m_shooting);

        _currentState = FIRING;

        // Tell cannon to fire cannonball
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_FIRED_CANNON));
        e->_to_the_left = this->_transform != blit::SpriteTransform::HORIZONTAL;
        e->_pos = this->getPos();
        _pnk._dispatcher.queueEvent(std::move(e));

        // handled by subclasses
        return true;
    }

    void Cannon::cannonHasFired()
    {
        prepareChangeState(SLEEPING);
    }

    void Cannon::tellTheKingWeHitHim()
    {
        //
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_spr = shared_from_this();
        e->_payload = ST_PIG_CANNON;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void Cannon::removeSelf()
    {
        // remove throwie
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
        e->_spr = shared_from_this();
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}