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
        if (mf.me->_type_num == dang::SpriteType::KING || mf.other->_type_num == dang::SpriteType::KING)
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_START_BOSSBATTLE));
                _pnk._dispatcher.queueEvent(std::move(e));
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse BossbattleTrigger::getCollisionResponse(const spCollisionSprite& other)
    {
        if (other->_type_num == dang::SpriteType::KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }
        return dang::CollisionSpriteLayer::CR_NONE;
    }
}
