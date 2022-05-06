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

    BossbattleTrigger::BossbattleTrigger() : dang::CollisionSprite()
    {
    }

    BossbattleTrigger::BossbattleTrigger(const dang::tmx_spriteobject* so) : dang::CollisionSprite(so, nullptr)
    {
    }

    void BossbattleTrigger::collide(const dang::manifold &mf)
    {
        dang::spCollisionSprite sprOther = getOther(mf, this);
//        dang::spCollisionSprite sprOther = std::static_pointer_cast<CollisionSprite>(mf.me.get() == this ? mf.other : mf.me);

        if (sprOther->_type_num == ST_KING)
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_START_BOSSBATTLE));
            _pnk._dispatcher.queueEvent(std::move(e));

            // make sure not to hit anymore
            _hotrect = {0,0,0,0};
        }
    }

    uint8_t  BossbattleTrigger::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::CollisionSprite* cs_other = dynamic_cast<const CollisionSprite*>(other);
//        dang::spCollisionSprite cs_other = std::static_pointer_cast<CollisionSprite>(other);
        if (cs_other->_type_num == ST_KING)
        {
            return dang::CR_CROSS;
        }
        return dang::CR_NONE;
    }
}
