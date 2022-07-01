// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "SavepointTrigger.h"

#include "PnkEvent.h"
#include "pnk_globals.h"
#include "pigsnkings.hpp"


namespace pnk
{
    extern PigsnKings _pnk;

    SavepointTrigger::SavepointTrigger() : dang::ColSpr()
    {
        _cr = dang::CR_CROSS;
    }

    SavepointTrigger::SavepointTrigger(const dang::tmx_spriteobject* so) : dang::ColSpr(so)
    {
        _cr = dang::CR_CROSS;
    }

    void SavepointTrigger::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_KING)
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_SAVEPOINT_TRIGGERED));
            _pnk._dispatcher.queueEvent(std::move(e));

            // make sure not to hit anymore
            _consumed = true;
            _cr = dang::CR_NONE;
        }
    }

    uint8_t  SavepointTrigger::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = dynamic_cast<const ColSpr*>(other);

        if (cs_other->typeNum() == ST_KING)
        {
            return _cr;
        }
        return dang::CR_NONE;
    }
}
