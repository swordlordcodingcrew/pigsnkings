// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game


#include "Bombies.h"

#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "pnk_globals.h"

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>
#include <tween/TwVel.hpp>

#include <cassert>

namespace pnk
{
    using spTwSeq = std::shared_ptr<dang::TwSequence>;
    using spTwVel = std::shared_ptr<dang::TwVel>;

    extern PigsnKings _pnk;

    Bombies::Bombies()
    {

    }

    Bombies::Bombies(const dang::tmx_spriteobject* so, dang::spImagesheet is) : Throwies(so, is)
    {

    }

    Bombies::Bombies(const Bombies &bomb) : Throwies(bomb)
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("Bombies copy constructor\n");
#endif
        _to_the_left = bomb._to_the_left;
        _anim_flying = std::make_shared<dang::TwAnim>(*(bomb._anim_flying));
        _anim_flying->reset();

        _anim_on_fire = std::make_shared<dang::TwAnim>(*(bomb._anim_on_fire));
        _anim_on_fire->setFinishedCallback(std::bind(&Bombies::triggerExplosion, this));
        _anim_on_fire->reset();
    }

    Bombies::~Bombies()
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("Bombies destructor\n");
#endif
    }

    void Bombies::init()
    {
        this->Throwies::init();

        _hotrect = {11, 20, 10, 10};
    }


    void Bombies::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (!_bIsOnFire && (sprOther->typeNum() == ST_HOTRECT || sprOther->typeNum() == ST_HOTRECT_PLATFORM))
        {
            // have the animation sequence triggered
            setBombOnFire();
        }
        else if (_bIsOnFire && (sprOther->typeNum() == ST_KING))
        {
            // King hurt
            tellTheKingWeHitHim();
        }
    }

    uint8_t  Bombies::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = static_cast<const ColSpr*>(other);

        if (cs_other->typeNum() == ST_KING || cs_other->typeNum() == ST_HOTRECT || cs_other->typeNum() == ST_HOTRECT_PLATFORM)
        {
            return dang::CR_BOUNCE;
        }

        return dang::CR_NONE;
    }

    void Bombies::tellTheKingWeHitHim()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_payload = ST_FLYING_BOMB;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void Bombies::setBombOnFire()
    {
        // no reset of animation needed, will only be called once and reset is called after initialisation
        setAnimation(_anim_on_fire);
        _bIsOnFire = true;
    }

    void Bombies::triggerExplosion()
    {
        setAnimation(_anim_flying);
        _anim_on_fire = nullptr;

        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_BOMB_EXPLODES));
        e->_pos = this->getPos();
        e->_payload = ST_FLYING_BOMB;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));

        // me destroys in the next cycle, we need the pointer in this cycle for the event
        _remove_me = true;
    }
}
