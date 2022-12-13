// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "BossbattleTrigger.h"
#include "src/SpriteFactory.hpp"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/pigsnkings.hpp"

#include <TmxExtruder.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    BossbattleTrigger::BossbattleTrigger() : dang::ColSpr()
    {
        _cr = dang::CR_CROSS;
    }

    BossbattleTrigger::BossbattleTrigger(const dang::tmx_spriteobject* so) : dang::ColSpr(so)
    {
        _cr = dang::CR_CROSS;
    }

    void BossbattleTrigger::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_KING)
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_START_BOSSBATTLE));
            _pnk._dispatcher.queueEvent(std::move(e));

            // make sure not to hit anymore
            _cr = dang::CR_NONE;
//            _hotrect = {0,0,0,0};
        }
    }

    uint8_t  BossbattleTrigger::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = static_cast<const ColSpr*>(other);
        if (cs_other->typeNum() == ST_KING)
        {
            return _cr;
        }
        return dang::CR_NONE;
    }
}
