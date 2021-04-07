// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "RoomTrigger.h"
#include "src/SpriteFactory.hpp"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/pigsnkings.hpp"

#include <TmxExtruder.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    RoomTrigger::RoomTrigger() : dang::CollisionSprite()
    {
        _room = 0;
        _warp = true;
    }

    RoomTrigger::RoomTrigger(const dang::tmx_spriteobject &so, bool warp) : dang::CollisionSprite(so, nullptr), _warp(warp)
    {
        _room = std::stoul(so.name);
    }


    void RoomTrigger::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.me->_type_num == SpriteFactory::TN_KING || mf.other->_type_num == SpriteFactory::TN_KING)
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

    dang::CollisionSpriteLayer::eCollisionResponse RoomTrigger::getCollisionResponse(spSprite other)
    {
        if (other->_type_num == SpriteFactory::TN_KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }
        return dang::CollisionSpriteLayer::CR_NONE;
    }

}
