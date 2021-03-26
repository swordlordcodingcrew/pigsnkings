// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "RoomTrigger.h"
#include "SpriteFactory.hpp"
#include "PnkEvent.h"
#include "pnk_globals.h"
#include "pigsnkings.hpp"

#include <TmxExtruder.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    RoomTrigger::RoomTrigger() : dang::CollisionSprite()
    {
    }

    RoomTrigger::RoomTrigger(const dang::tmx_spriteobject &so) : dang::CollisionSprite(so, nullptr)
    {
        _room = std::stoul(so.name);
    }


    void RoomTrigger::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.me->_type_num == SpriteFactory::TN_KING || mf.other->_type_num == SpriteFactory::TN_KING)
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_CHANGE_ROOM));
            e->_pos.x = _room;
            _pnk._dispatcher.queueEvent(std::move(e));
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse RoomTrigger::getCollisionResponse(spSprite other)
    {
        if (other->_type_num == SpriteFactory::TN_KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }
        return dang::CollisionSpriteLayer::CR_NONE;
    }

}
