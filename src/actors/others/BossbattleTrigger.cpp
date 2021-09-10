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

    void BossbattleTrigger::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.me->_type_num == ST_KING || mf.other->_type_num == ST_KING)
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_START_BOSSBATTLE));
            _pnk._dispatcher.queueEvent(std::move(e));

            // make sure not to hit anymore
            _hotrect = {0,0,0,0};
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse BossbattleTrigger::getCollisionResponse(const spCollisionSprite& other)
    {
        if (other->_type_num == ST_KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }
        return dang::CollisionSpriteLayer::CR_NONE;
    }
}
