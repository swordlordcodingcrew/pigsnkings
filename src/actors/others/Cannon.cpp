// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Cannon.h"

#include "src/pnk_globals.h"
#include "src/PnkEvent.h"
#include "src/pigsnkings.hpp"

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>


namespace pnk
{
    extern PigsnKings _pnk;

    Cannon::Cannon() : dang::FullColSpr()
    {
    }

    Cannon::Cannon(const dang::tmx_spriteobject* so, dang::spImagesheet is) : dang::FullColSpr(so, is)
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
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("Cannon destructor\n");
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

    uint8_t  Cannon::getCollisionResponse(const dang::CollisionObject* other) const
    {

        const dang::ColSpr* cs_other = dynamic_cast<const ColSpr*>(other);
        if (cs_other->typeNum() == ST_KING || cs_other->typeNum() == ST_HOTRECT || cs_other->typeNum() == ST_HOTRECT_PLATFORM)
        {
            return dang::CR_SLIDE;
        }

        return dang::CR_NONE;

    }

    void Cannon::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_KING)
        {
            tellTheKingWeHitHim();
        }
        else
        {
            const dang::Vector2F &normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.x != 0)
            {
                setVelX(0);
            }

            if (normal.y > 0)
            {
                setVelY(0);
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
            std::printf("_anim_m_sleeping is not set in Cannon\n");
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
        e->_to_the_left = getTransform() != blit::SpriteTransform::HORIZONTAL;
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
        pnk::_pnk._dispatcher.queueEvent(PnkEvent::createGE(ETG_KING_HIT, ST_PIG_CANNON));

//        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
//        e->_spr = shared_from_this();
//        e->_payload = ST_PIG_CANNON;
//        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void Cannon::removeSelf()
    {
        markRemove();
        pnk::_pnk._dispatcher.queueEvent(PnkEvent::createGE(ETG_REMOVE_SPRITE, id()));

//        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
//        e->_spr = shared_from_this();
//        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}