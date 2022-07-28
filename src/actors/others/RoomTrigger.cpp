// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "RoomTrigger.h"

#include "PnkEvent.h"
#include "pnk_globals.h"
#include "pigsnkings.hpp"

#include <TmxExtruder.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    RoomTrigger::RoomTrigger() : dang::ColSpr()
    {
        _room = 0;
        _warp = true;
    }

    RoomTrigger::RoomTrigger(const dang::tmx_spriteobject* so, bool warp) : dang::ColSpr(so), _warp(warp)
    {
        // name of the sprite object from the tiled level will be used as the room number to jump to
        _room = std::stoul(so->name);
    }


    void RoomTrigger::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_KING)
        {
            if (_warp)
            {
                std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_WARP_ROOM));
                e->_payload = _room;
                _pnk._dispatcher.queueEvent(std::move(e));

            }
            else
            {
                std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_CHANGE_ROOM));
                e->_payload = _room;
                _pnk._dispatcher.queueEvent(std::move(e));
            }
        }
    }

    uint8_t  RoomTrigger::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = static_cast<const ColSpr*>(other);

        if (cs_other->typeNum() == ST_KING)
        {
            return dang::CR_CROSS;
        }
        return dang::CR_NONE;
    }

}
