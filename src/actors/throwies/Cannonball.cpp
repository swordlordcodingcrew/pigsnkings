// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <cassert>

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>
#include <tween/TwVel.hpp>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"
#include "Throwies.h"
#include "Cannonball.h"
#include "src/actors/npc/Enemy.h"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{
    using spTwSeq = std::shared_ptr<dang::TwSequence>;
    using spTwVel = std::shared_ptr<dang::TwVel>;

    extern PigsnKings _pnk;

    Cannonball::Cannonball()
    {

    }

    Cannonball::Cannonball(const dang::tmx_spriteobject* so, dang::spImagesheet is) : Throwies(so, is)
    {

    }

    Cannonball::Cannonball(const Throwies &can) : Throwies(can)
    {
#ifdef PNK_DEBUG_PRINT
        std::cout << "Cannonball copy constructor" << std::endl;
#endif
        _to_the_left = can._to_the_left;
        _anim_flying = std::make_shared<dang::TwAnim>(*(can._anim_flying));

        _anim_flying->reset();

        removeTweens(true);
        removeAnimation(true);
    }

    Cannonball::~Cannonball()
    {
#ifdef PNK_DEBUG_PRINT
        std::cout << "Cannonball destructor" << std::endl;
#endif
    }

    void Cannonball::init()
    {
        _hotrect = {10, 10, 8, 8};
    }


    void Cannonball::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_HOTRECT)
        {
            // have the animation sequence triggered
            triggerExplosion();

            // me destroys in the next cycle, we need the pointer in this cycle for the event
            _remove_me = true;
        }
        else if (sprOther->typeNum() == ST_KING)
        {
            // King hurt
            tellTheKingWeHitHim();

            // me destroys in the next cycle, we need the pointer in this cycle for the event
            _remove_me = true;
        }
    }

    uint8_t  Cannonball::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = static_cast<const ColSpr*>(other);
        if (cs_other->typeNum() == ST_KING || cs_other->typeNum() == ST_HOTRECT)
        {
            return dang::CR_TOUCH;
        }

        return dang::CR_NONE;
    }

    void Cannonball::tellTheKingWeHitHim()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_payload = ST_FLYING_CANNONBALL;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void Cannonball::triggerExplosion()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_CANNONBALL_EXPLODES));
        e->_pos = this->getPos();
        e->_payload = ST_FLYING_CANNONBALL;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}
